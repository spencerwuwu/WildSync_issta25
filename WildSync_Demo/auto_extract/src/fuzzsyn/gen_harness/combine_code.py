#!/usr/bin/env python3

import os
import sys, traceback, re
from loguru import logger

from fuzzsyn.utils import is_primitive_type, reconstruct_statement
from fuzzsyn.backtrace.backtrace import backtrace_function_in_file, backtrace_from_code
from fuzzsyn.gen_harness.consume_harness import consume_original_code
from fuzzsyn.backtrace.utils import recursively_get_self_n_children_of_types
from comex.utils.cpp_nodes import control_statements

from fuzzsyn.utils import timeout

def add_libraries(code):
    libraries = {"stdio.h": "#include <stdio.h>", 
                 "stdlib.h": "#include <stdlib.h>", 
                 "string.h": "#include <string.h>",
                 }
    for lib in libraries:
        if lib not in code:
            code = f"{libraries[lib]}\n{code}"
    return code


def my_sub(r, s, t):
    new_t = re.sub(fr"^{r}\s", s + " ", t)
    new_t = re.sub(fr"([\(=\s\{{]){r}([\)\s])", fr"\1{s}\2", new_t)
    return new_t


def get_target_argument_types(target_args):
    arg_types = []
    for target_arg in target_args:
        arg_type = target_arg[0]
        is_prim = is_primitive_type(arg_type)
        pointer_stars = arg_type.count("*") 
        type_name = arg_type.replace("*", "").strip()
        arg_types.append({"type": type_name, 
                          "pointer_stars": "*" * pointer_stars, 
                          "is_primitive": is_prim})
    return arg_types


def match_function_args(target_args, existing_args):
    existing_types = [arg[0] for arg in existing_args]
    arg_sets = []
    for index in range(len(target_args)):
        target_arg = target_args[index]
        arg_type = target_arg[0]
        # Add only if name is also the same
        if arg_type not in existing_types:
            arg_sets.append(-1)
            continue

        existing_index = existing_types.index(arg_type)
        if is_primitive_type(arg_type):
            arg_name = target_arg[1]
            existing_arg_name = existing_args[existing_index][1]
            if arg_name == existing_arg_name:
                arg_sets.append(existing_index)
            else:
                arg_sets.append(-1)
        else:
            arg_sets.append(existing_index)
    return arg_sets


def combine_orig_and_extracted_code(handled_original_code, fuzz_init_code, extract_code):
    Index = handled_original_code["combined_driver"].results["AST"].parser.index
    CFG = handled_original_code["combined_driver"].results["CFG"].graph
    def _get_node_id(node):
        return Index.get((node.start_point, node.end_point, node.type), -1)

    def _reconstruct_fuzz_entry(node, replace_items):
        string = ""
        func_node = node
        if node.type == "linkage_specification":
            for child in node.children:
                if child.type == "function_definition":
                    func_node = node.named_children[-1]
                    continue
                else:
                    string += reconstruct_statement(handled_original_code["combined_driver"], child, replace_items) + " "
        else:
            func_node = node
        for child in func_node.children[:-1]:
            string += reconstruct_statement(handled_original_code["combined_driver"], child, replace_items)
        return string

    def _reconstruct_with_inseration(node, insert_line, content, has_rewritten):
        node_id = _get_node_id(node)
        string = ""
        if not has_rewritten and CFG.has_node(node_id):
            line_no = CFG.nodes[node_id]["lineno"]
            # HACK
            #if line_no > insert_line:
            if line_no == insert_line:
                string += "/**** Start of extracted external code ****/\n"
                string += content
                string += "\n/**** End of extracted external code ****/\n"
                has_rewritten = True
        if CFG.has_node(node_id):
            except_group = ["return_statement", "goto_statement"]
            if node.type not in control_statements or node.type in except_group:
                string += node.text.decode() + "\n"
                return string, has_rewritten
        if node.type in ["identifier", "literal", "pointer_type"]:
            return string + " " + node.text.decode(), has_rewritten
        if not node.children:
            content = node.text.decode()
            if content in ["{", "}", ":", ";"]:
                content += "\n"
            elif content in ["#endif"]:
                content += "\n"
            elif "#" in content:
                content += " "
            elif "else" in content:
                content += " "
            return string + content, has_rewritten
        for child in node.children:
            new_string, has_rewritten = _reconstruct_with_inseration(child, insert_line, content, has_rewritten)
            string += new_string
        return string, has_rewritten

    return_code = []
    has_rewritten = False
    for node in handled_original_code["combined_driver"].results["AST"].root_node.children:
        if node.type not in ["function_definition", "linkage_specification"]:
            content = node.text.decode()
            if content.endswith(";") or content.endswith("}") or content.endswith("#endif"):
                content += "\n"
            return_code.append(content)
        else:
            if node.type == "linkage_specification":
                func_node = node.named_children[-1]
            else:
                func_node = node
            if func_node != handled_original_code["fuzzer_entry_node"]:
                return_code.append(node.text.decode())
                return_code.append("\n")
                continue
            # NOTE: assuming the rewrite function is also in the same function definition
            replace_items = {
                _get_node_id(handled_original_code["buffer_node"]["name_node"]): "_IN_BUFFER",
                _get_node_id(handled_original_code["buflen_node"]["name_node"]): "_IN_BUFFER_SIZE"
            }
            return_code.append(_reconstruct_fuzz_entry(node, replace_items))
            compound = func_node.named_children[-1]
            return_code.append("{\n")
            return_code.append('/**** Start of fuzz variables initialization code ****/\n')
            return_code.append(fuzz_init_code)
            return_code.append("\n/**** End of fuzz variables initialization code ****/\n")
            for child in compound.named_children:
                code, has_rewritten = _reconstruct_with_inseration(child, 
                                                    handled_original_code["replace_statement_line"], 
                                                    extract_code, has_rewritten)
                return_code.append(code)
            return_code.append("}\n")
    if not has_rewritten:
        logger.error("Failed to rewrite extracted code")
        return ""
    return "".join(return_code)


def do_combine(codefile_to_extract, original_harness, input_data, output_dest, lib_symbols=[], keep_related=False):
    handled_original_code = consume_original_code(original_harness, 
                                                  input_data["replace_function"]["function_name"])
    # -----------------------
    # TODO: next: match replace function variables with target function variables
    orig_buffer_name = handled_original_code["buffer_node"]["name_node"].text.decode()
    orig_buffer_len = handled_original_code["buflen_node"]["name_node"].text.decode()
    target_function = input_data["target_function"]["function_name"]
    matched_arg_sets = match_function_args(input_data["target_function"]["args"],
                                           input_data["replace_function"]["args"])
    matched_function_args = []
    replace_func_arguments = [arg.text.decode() for arg in handled_original_code["arguments"]] 
    matched_func_variables = []

    declared_names = handled_original_code["declared_names"]

    target_arg_types = get_target_argument_types(input_data["target_function"]["args"])

    for idx in range(len(matched_arg_sets)):
        if matched_arg_sets[idx] != -1:
            matched_function_args.append(replace_func_arguments[matched_arg_sets[idx]])
            var_types = ["identifier", "field_expression"]
            cur_arg = handled_original_code["arguments"][matched_arg_sets[idx]]
            for n in recursively_get_self_n_children_of_types(cur_arg, var_types):
                matched_func_variables.append(n.text.decode())
        else:
            matched_function_args.append("")
    logger.info(f"Matched function args: {matched_function_args}")
    # -----------------------
    try:
        with timeout(seconds=60):
            extract_results = backtrace_function_in_file(codefile_to_extract, target_function,
                                                         matched_function_args,
                                                         orig_buffer_name,  orig_buffer_len,
                                                         save_graph=False,
                                                         lib_symbols=lib_symbols,
                                                         matched_func_variables=matched_func_variables,
                                                         target_arg_types=target_arg_types,
                                                         declared_names=declared_names,
                                                         keep_related=keep_related)
    except TimeoutError:
        logger.error(f"Timeout tracing {target_function}")
        exit(1)
    except NotImplementedError as e:
        logger.error(f"Failed to backtrace {target_function} in {codefile_to_extract}")
        exit(1)
    except:
        #logger.error(f"Failed to backtrace {target_function} in {codefile_to_extract}")
        logger.exception(f"Failed to backtrace {target_function} in {codefile_to_extract}")
        #exc = sys.exception()
        #traceback.print_exception(exc)
        exit(1)
    if not extract_results:
        logger.error(f"No {target_function} found or extracted in {codefile_to_extract}")
        exit(1)
    # -----------------------
    logger.info("\nFinished handling original code, combining...")
    # TODO: handle multiple results
    extract_result = extract_results[0]
    fuzz_init_code = extract_result["init_code"]
    extract_code = extract_result["extracted_code"]
    #extract_code = gen_init_for_extracted_code(extract_code, fuzz_init_code, target_function, matched_function_args)
    # -----------------------
    #fuzz_init_code = "\n"
    #extract_code = "\n"
    combined_code = combine_orig_and_extracted_code(handled_original_code, fuzz_init_code, extract_code)
    if not combined_code:
        logger.error(f"Failed to combine {codefile_to_extract} with original harness {original_harness}")
        return

    combined_code = add_libraries(combined_code)
    # HACK
    combined_code = combined_code.replace("const ", "")
    combined_code = my_sub("Image", "MagickCore::Image", combined_code)
    combined_code = my_sub("ImageInfo", "MagickCore::ImageInfo", combined_code)
    combined_code = my_sub("StringInfo", "Magick::StringInfo", combined_code)
    combined_code = my_sub("StorageType", "Magick::StorageType", combined_code)
    combined_code = my_sub("BlobToStringInfo", "MagickCore::BlobToStringInfo", combined_code)
    combined_code = my_sub("QuantizeInfo", "MagickCore::QuantizeInfo", combined_code)
    combined_code = my_sub("GetQuantizeInfo", "MagickCore::GetQuantizeInfo", combined_code)
    combined_code = my_sub("QuantizeImages", "MagickCore::GetQuantizeImages", combined_code)
    with open(output_dest, "w") as f:
        f.write(combined_code)


def try_do_combines(original_harness, input_data, output_dir, external_code_sets, 
                    failed_dir=None, 
                    lib_name="_",
                    lib_symbols=[],
                    keep_related=False,
                    worth_retries={}):
    try:
        handled_original_code = consume_original_code(original_harness, 
                                                  input_data["replace_function"]["function_name"])
    except:
        return False, worth_retries
    # -----------------------
    # TODO: next: match replace function variables with target function variables
    orig_buffer_name = handled_original_code["buffer_node"]["name_node"].text.decode()
    orig_buffer_len = handled_original_code["buflen_node"]["name_node"].text.decode()
    target_function = input_data["target_function"]["function_name"]
    matched_arg_sets = match_function_args(input_data["target_function"]["args"],
                                           input_data["replace_function"]["args"])
    matched_function_args = []
    replace_func_arguments = [arg.text.decode() for arg in handled_original_code["arguments"]] 
    matched_func_variables = []
    for idx in range(len(matched_arg_sets)):
        if matched_arg_sets[idx] != -1:
            matched_function_args.append(replace_func_arguments[matched_arg_sets[idx]])
            var_types = ["identifier", "field_expression"]
            cur_arg = handled_original_code["arguments"][matched_arg_sets[idx]]
            for n in recursively_get_self_n_children_of_types(cur_arg, var_types):
                matched_func_variables.append(n.text.decode())
        else:
            matched_function_args.append("")
    logger.info(f"Matched function args: {matched_function_args}")

    declared_names = handled_original_code["declared_names"]

    target_arg_types = get_target_argument_types(input_data["target_function"]["args"])

    syn_idx = 0
    for src_name, external_code in external_code_sets.items():
        if syn_idx > 10:
            logger.warning(f"Capping combined_code synthesis of {syn_idx} at 10")
            break
        if src_name in worth_retries:
            if not worth_retries[src_name]:
                logger.info(f"Skip combining with {src_name} for previous failure.")
                continue
        logger.info(f"Try combining with {src_name}...")
        try:
            with timeout(seconds=60):
                extract_results = backtrace_from_code(external_code, 
                                                      target_function,
                                                      matched_function_args,
                                                      orig_buffer_name,
                                                      orig_buffer_len,
                                                      save_graph=False,
                                                      lib_symbols=lib_symbols, 
                                                      matched_func_variables=matched_func_variables,
                                                      target_arg_types=target_arg_types,
                                                      declared_names=declared_names,
                                                      keep_related=keep_related)
        except TimeoutError:
            logger.error(f"Timeout for backtracing {target_function} in {src_name}")
            worth_retries[src_name] = False
            continue
        except NotImplementedError as e:
            logger.error(f"Failed to backtrace {target_function} in {src_name}")
            if failed_dir and os.path.exists(failed_dir):
                with open(f"{failed_dir}/{lib_name}__{target_function}__{syn_idx}.cpp", "w") as fd:
                    fd.write(external_code)
            worth_retries[src_name] = False
            continue
        except:
            #logger.error(f"Failed to backtrace {target_function} in {src_name}")
            #exc = sys.exception()
            #traceback.print_exception(exc)
            logger.exception(f"Failed to backtrace {target_function} in {src_name}")
            worth_retries[src_name] = False
            continue
        if not extract_results:
            logger.error(f"No result backtrace {target_function} in {src_name}")
            if failed_dir and os.path.exists(failed_dir):
                with open(f"{failed_dir}/{lib_name}__{target_function}__{syn_idx}.cpp", "w") as fd:
                    fd.write(external_code)
            worth_retries[src_name] = False
            continue
        logger.info(f"\nFinished handling {src_name}, combining...")
        # -----------------------
        for extract_result in extract_results:
            fuzz_init_code = extract_result["init_code"]
            extract_code = extract_result["extracted_code"]
            extract_code = f"// Extracted from: {src_name}\n" + extract_code
            #extract_code = gen_init_for_extracted_code(extract_code, fuzz_init_code, target_function, matched_function_args)
            ## -----------------------
            try:
                combined_code = combine_orig_and_extracted_code(handled_original_code, fuzz_init_code, extract_code)
                if not combined_code:
                    logger.error(f"Failed to combine {src_name} with original harness {original_harness}")
                    return False, worth_retries
                output_dest = f"{output_dir}/{lib_name}__{target_function}__{syn_idx}.cpp"
                combined_code = add_libraries(combined_code)
                # HACK
                if "tesseract" in src_name:
                    combined_code = combined_code.replace(" Image ", " PIX* ")
                if lib_name == "imagemagick":
                    combined_code = my_sub("Image", "MagickCore::Image", combined_code)
                    combined_code = my_sub("ImageInfo", "MagickCore::ImageInfo", combined_code)
                    combined_code = my_sub("StringInfo", "Magick::StringInfo", combined_code)
                    combined_code = my_sub("StorageType", "Magick::StorageType", combined_code)
                    combined_code = my_sub("BlobToStringInfo", "MagickCore::BlobToStringInfo", combined_code)
                    combined_code = my_sub("QuantizeInfo", "MagickCore::QuantizeInfo", combined_code)
                    combined_code = my_sub("GetQuantizeInfo", "MagickCore::GetQuantizeInfo", combined_code)
                    combined_code = my_sub("QuantizeImages", "MagickCore::GetQuantizeImages", combined_code)
                    

                # HACK
                if lib_name not in ["libxml2", "libwebp", "speex", 'libheif', "libpcap", "lcms", "libplacebo"]:
                    combined_code = combined_code.replace("const ", "")
                if lib_name == "libwebp":
                    if "WebPAnimDecoderGetDemuxer" in combined_code:
                        new_code = ""
                        for line in combined_code.split("\n"):
                            if "WebPAnimDecoderGetDemuxer" in line:
                                new_code += line + "\n"
                            else:
                                new_code += line.replace("const", "") + "\n"
                        combined_code = new_code
                    else:
                        combined_code = combined_code.replace("const ", "")
                if lib_name == "speex":
                    new_code = ""
                    for line in combined_code.split("\n"):
                        if "SpeexMode" in line:
                            new_code += line + "\n"
                        else:
                            new_code += line.replace("const", "") + "\n"
                    combined_code = new_code
                if lib_name == "libheif":
                    combined_code = combined_code.replace("uint8_t* data = _IN_BUFFER", "const uint8_t* data = _IN_BUFFER")
                if lib_name == "libpcap":
                    combined_code = combined_code.replace("uint8_t* Data = _IN_BUFFER", "const uint8_t* Data = _IN_BUFFER")
                if lib_name == "lcms":
                    new_code = ""
                    for line in combined_code.split("\n"):
                        if "gchar" in line:
                            new_code += line + "\n"
                        else:
                            new_code += line.replace("const", "") + "\n"
                    combined_code = new_code

                if lib_name == "libplacebo":
                    new_code = ""
                    for line in combined_code.split("\n"):
                        if "hook" in line:
                            new_code += line + "\n"
                        else:
                            new_code += line.replace("const", "") + "\n"
                    combined_code = new_code

                    if "pl_opengl" in combined_code:
                        combined_code = "#include \"libplacebo/opengl.h\"\n" + combined_code


                # HACK
                if lib_name == "gdk-pixbuf":
                    libs  = "#include <gdk-pixbuf/gdk-pixbuf.h>\n"
                    libs += "#include <gdk-pixbuf/gdk-pixdata.h>\n"
                    combined_code = libs + combined_code

                # HACK
                if lib_name == "libxml2":
                    combined_code = "#include <stdint.h>\n" + combined_code

                # HACK
                if lib_name == "cjson":
                    pre = """
#ifdef __cplusplus
extern "C" {
#endif
                    """
                    end = """
#ifdef __cplusplus
}
#endif
                    """
                    combined_code = pre + combined_code + end

                if lib_name == "libpng":
                    sub = """
#include "png.h"

#define PNG_CLEANUP \
  if(png_handler.png_ptr) \
  { \
    if (png_handler.row_ptr) \
      png_free(png_handler.png_ptr, png_handler.row_ptr); \
    if (png_handler.end_info_ptr) \
      png_destroy_read_struct(&png_handler.png_ptr, &png_handler.info_ptr,\
        &png_handler.end_info_ptr); \
    else if (png_handler.info_ptr) \
      png_destroy_read_struct(&png_handler.png_ptr, &png_handler.info_ptr,\
        nullptr); \
    else \
      png_destroy_read_struct(&png_handler.png_ptr, nullptr, nullptr); \
    png_handler.png_ptr = nullptr; \
    png_handler.row_ptr = nullptr; \
    png_handler.info_ptr = nullptr; \
    png_handler.end_info_ptr = nullptr; \
  }
                    """
                    combined_code = combined_code.replace('#include "png.h"', sub)

                if lib_name == "lcms":
                    patches = """
#include "lcms2.h"
#include <stdbool.h>
#define gchar char
#define gboolean bool
#define guchar unsigned char
#define guint unsigned int
typedef struct {
	guint			 idx;
	cmsFloat32Number	*data;
} CdIccUtilsGamutCheckHelper;
                    """
                    combined_code = combined_code.replace('#include "lcms2.h"', patches)

                    

                with open(output_dest, "w") as f:
                    f.write(combined_code)
                syn_idx += 1
                if syn_idx > 10:
                    logger.warning(f"Capping combined_code synthesis of {syn_idx} at 10")
                    break
            except:
                #logger.error(f"Failed to combine {src_name} with original harness")
                #exc = sys.exception()
                #traceback.print_exception(exc)
                logger.exception(f"Failed to combine {src_name} with original harness")
                continue
    if syn_idx > 0:
        return True, worth_retries
    return False, worth_retries

