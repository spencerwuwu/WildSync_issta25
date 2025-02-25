from loguru import logger
from comex.codeviews.combined_graph.combined_driver import CombinedDriver
import signal

########################
###  Indexs:
###   - General logic
###   - Preprocessing
###   - comex utilitiles
###   - export feature
###   - Processing
########################

codeviews = {
    "AST": {
        "exists": True,
        "collapsed": False,
        "minimized": False,
        "blacklisted": []
    },
    "DFG": {
        "exists": True,
        "collapsed": False,
        "minimized": False,
        "statements": True,
        "last_def": False,
        "last_use": False
    },
    "CFG": {
        "exists": True,
    }
}

class timeout:
    def __init__(self, seconds=1, error_message='Timeout'):
        self.seconds = seconds
        self.error_message = error_message
    def handle_timeout(self, signum, frame):
        raise TimeoutError(self.error_message)
        #raise Exception(self.error_message)
    def __enter__(self):
        signal.signal(signal.SIGALRM, self.handle_timeout)
        signal.alarm(self.seconds)
    def __exit__(self, type, value, traceback):
        signal.alarm(0)

########################
#  General logic
########################

def is_primitive_type(var_type_):
    var_type = var_type_.replace("*", "").strip()
    primitive_types = [
        "int", "char", "float", "double", "long", "short", "void",
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "size_t", "ssize_t", "off_t", "time_t", "clock_t",
        "unsigned",
        # glib
        "gchar",
        # libharu
        "HPDF_INT",
        "HPDF_UINT",
        "HPDF_INT32",
        "HPDF_UINT32",
        "HPDF_INT16",
        "HPDF_UINT16",
        "HPDF_INT8",
        "HPDF_UINT8",
        "HPDF_BYTE",
        "HPDF_REAL",
        "HPDF_DOUBLE",
        "HPDF_BOOL",
        "HPDF_STATUS",
        # leptonica specific
        "l_ok",
        "l_int8",
        "l_uint8",
        "l_int16",
        "l_uint16",
        "l_int32",
        "l_uint32",
        "l_float32",
        "l_float64",
        "l_int64",
        "l_uint64",
        "l_int64", 
        "l_uint64",
        # lcms
        "cmsUInt8Number",
        "cmsInt8Number", 
        "cmsFloat32Number",
        "cmsFloat64Number",
        "cmsUInt16Number",
        "cmsInt16Number",
        "cmsUInt32Number",
        "cmsInt32Number",
        "cmsUInt64Number",
        "cmsInt64Number",
        "cmsSignature",
        "cmsU8Fixed8Number",
        "cmsS15Fixed16Number",
        "cmsU16Fixed16Number",
        "cmsBool",
        # libpng
        "png_int_32",
        "png_alloc_size_t",
        "png_fixed_point",
        "png_voidp",
        "png_const_voidp",
        "png_bytep",
        "png_const_bytep",
        "png_uint_32p",
        "png_const_uint_32p",
        "png_int_32p",
        "png_const_int_32p",
        "png_uint_16p",
        "png_const_uint_16p",
        "png_int_16p",
        "png_const_int_16p",
        "png_charp",
        "png_const_charp",
        "png_fixed_point_p",
        "png_const_fixed_point_p",
        "png_size_tp",
        "png_const_size_tp",
        "png_bytepp",
        "png_uint_32pp",
        "png_int_32pp",
        "png_uint_16pp",
        "png_int_16pp",
        "png_const_charpp",
        "png_charpp",
        "png_fixed_point_pp",
        "png_charppp",
        ]
    for item in var_type.split():
        if item not in primitive_types:
            return False
    return True


def is_known_function(func_name, lib_symbols):
    # TODO: manually create some entries
    known_functions = ["memset", 
                       "memcpy", 
                       "malloc", 
                       "calloc", 
                       "realloc", 
                       "strlen", 
                       "free",
                       "av_malloc_array", # HACK
                       ]
    if (func_name in known_functions
        or func_name in lib_symbols):
        return True
    return False


def is_substitute_function(func_name):
    # TODO: manually create some entries
    substitute_functions = {
        "av_malloc_array": "calloc"
    }
    if func_name in substitute_functions:
        return substitute_functions[func_name]
    return None

########################
#  Preprocessing
########################
def remove_if_def_include(src_code):
    new_lines = []
    if_directrives = ["#if", "#elif", "#else", "#endif", "#ifdef", "#ifndef", "#include"]
    for line in src_code.splitlines():
        found = False
        for directive in if_directrives:
            if line.strip().startswith(directive):
                new_lines.append(f"// {line}")
                found = True
                break
        if not found:
            new_lines.append(line)
    return "\n".join(new_lines)

########################
#  comex utilitiles
########################
def search_function_call(AST_driver, function_name, 
                         DFG_driver=None,
                         to_cfg_node=True,
                         method_calls=["call_expression", "invocation_expression"]):
    search_results = []
    call_nodes = [node for node in AST_driver.orig_nodes.values() if node.type in method_calls]
    for func_node in call_nodes:
        label = func_node.children[0].text.decode()
        if label == function_name:
            # Back to expression level (to find node easier later)
            statement_id = -1
            statement_node = func_node
            statement_id = AST_driver.parser.index[
                (statement_node.start_point, statement_node.end_point, statement_node.type)
            ]
            if to_cfg_node and DFG_driver:
                while statement_node:
                    if statement_id in DFG_driver.rda_table:
                        break
                    statement_node = statement_node.parent
                    statement_id = AST_driver.parser.index[
                        (statement_node.start_point, statement_node.end_point, statement_node.type)
                    ]
            search_results.append(statement_id)
    return search_results


def search_function_call_from_node(AST_driver, root_node, function_name, search_results,
                                   DFG_driver=None, to_cfg_node=True):
    found = False
    if root_node.type == "call_expression":
        label = root_node.children[0].text.decode()
        if label == function_name:
            statement_id = -1
            statement_node = root_node
            statement_id = AST_driver.parser.index[
                (statement_node.start_point, statement_node.end_point, statement_node.type)
            ]
            if to_cfg_node and DFG_driver:
                while statement_node:
                    if statement_id in DFG_driver.rda_table:
                        break
                    statement_node = statement_node.parent
                    statement_id = AST_driver.parser.index[
                        (statement_node.start_point, statement_node.end_point, statement_node.type)
                    ]
            if statement_id not in search_results:
                search_results.append(statement_id)
            found = True
    for node in root_node.named_children:
        ret = search_function_call_from_node(AST_driver, node, function_name, search_results,
                                             DFG_driver=DFG_driver, to_cfg_node=to_cfg_node)
        if ret:
            found = True
    return found


def get_target_function_node(root_node, target_function_name):
    method_calls=["call_expression", "invocation_expression"]
    if root_node.type in method_calls:
        label = root_node.children[0].text.decode()
        if label == target_function_name:
            return root_node
    for node in root_node.children:
        if node.type in method_calls:
            label = node.children[0].text.decode()
            if label == target_function_name:
                return node
        ret = get_target_function_node(node, target_function_name)
        if ret:
            return ret
    return None


def get_function_declarator_node(func_node):
    decl_id = -1
    declarator = func_node.named_children[decl_id]
    st_types = ["function_declarator", "pointer_declarator", "qualified_identifier"]
    while declarator.type not in st_types and -decl_id < len(func_node.named_children):
        decl_id -= 1
        declarator = func_node.named_children[decl_id]
    while declarator.type in ["pointer_declarator", "qualified_identifier", "pointer_type_declarator"]:
        declarator = declarator.named_children[-1]
    return declarator


def search_function_definition(combined_driver, function_name, 
                               function_defs=["function_definition", "local_function_statement"]):
    AST_driver = combined_driver.results["AST"]
    def_nodes = [node for node in AST_driver.orig_nodes.values() if node.type in function_defs]
    searches = []
    for func_node in def_nodes:
        declarator = get_function_declarator_node(func_node)
        # Get function name
        if declarator.type != "function_declarator":
            logger.debug("Cannot find function declarator")
            continue
        label = declarator.named_children[0]
        if label.type == "qualified_identifier":
            label = label.named_children[-1]
        label = label.text.decode()
        if label == function_name:
            searches.append(func_node)
    return searches


def get_function_parameters(func_node):
    declarator = get_function_declarator_node(func_node)
    if declarator.type != "function_declarator":
        logger.debug("Cannot find function declarator")
        return []
    parameter_list = declarator.named_children[-1]
    parameters = []
    for param_dec in parameter_list.named_children:
        if param_dec.type != "parameter_declaration":
            logger.error(f"Unknown type in parameter_list {param_dec.type}")
            raise NotImplementedError(f"Unknown type in parameter_list {param_dec.type}")
        parameters.append(param_dec)
    return parameters


def handle_cpp_function_parameter(param_node):
    if param_node.text.decode() == "void":
        return None
    if len(param_node.named_children) < 2:
        param_type = param_node.named_children[-1]
        pointer_stars = 0
        value_node = None
        name = f"__fuzzsyn_{param_type.text.decode()}"
    else:
        param_type = param_node.named_children[-2]
        pointer_stars = 0
        value_node = param_node.named_children[-1]
        while value_node.type == "pointer_declarator":
            pointer_stars += 1
            value_node = value_node.named_children[0]
        name = value_node.text.decode()
    return {
        "type": param_type.text.decode(),
        "pointer_stars": "*" * pointer_stars,
        "name": name,
        "name_node": value_node
    }


def replace_function_existing_arguments(combined_driver, func_node, matched_function_args):
    rewrite_items = {}
    Index = combined_driver.results["AST"].parser.index
    arguments = func_node.named_children[-1].named_children
    for idx in range(len(arguments)):
        if not matched_function_args[idx]:
            continue
        if arguments[idx].type == "call_expression" and arguments[idx].named_children[0].text.decode() == "strlen":
            # TODO: very nitched case, only to make strlen('var') work
            argu = arguments[idx].named_children[0]
        else:
            argu = arguments[idx]
        node_idx = Index[argu.start_point, argu.end_point, argu.type]
        rewrite_items[node_idx] = matched_function_args[idx]
    return rewrite_items

########################
#  export
########################
def reconstruct_statement(combined_driver, node, replace_items, replace_names={}, no_space=False):
    # HACK: for 'if', 'while', only the condition is needed
    #       affecting backtrace/utils _reconstruct_with_select
    if node.type in ["if_statement", "while_statement"]:
        return reconstruct_statement(combined_driver, node.child_by_field_name("condition"), replace_items, replace_names, no_space=no_space)
    # HACK: for 'for', similiar 
    if node.type == "for_statement":
        init_node = node.child_by_field_name("initializer")
        condition = node.child_by_field_name("condition")
        update = node.child_by_field_name("update")
        init_statement = reconstruct_statement(combined_driver, init_node, replace_items, replace_names, no_space=no_space)
        cond_statement = reconstruct_statement(combined_driver, condition, replace_items, replace_names, no_space=no_space)
        update_statement = reconstruct_statement(combined_driver, update, replace_items, replace_names, no_space=no_space)
        return f"for ({init_statement}; {cond_statement}; {update_statement})"

    variable_types = ["identifier", "literal", "field_expression", "subscript_expression"]

    Index = combined_driver.results["AST"].parser.index
    node_id = Index.get(
        (node.start_point, node.end_point, node.type), -1)
    if node_id == -1:
        content = node.text.decode()
        if content not in ['"', "'", "(", ")"]:
            content = " " + content 
        return content
    if node_id in replace_items:
        if no_space:
            return replace_items[node_id]
        return " " + replace_items[node_id]
    string = ""
    if node.type in variable_types:
        var_name = node.text.decode()
        if var_name in replace_names:
            if no_space:
                return replace_names[var_name]
            return " " + replace_names[var_name]
        if no_space:
            return var_name
        return " " + var_name
    if not node.children:
        content = node.text.decode()
        if not no_space and content not in ["}", ":", ";", ",", "C", '"', "'", "(", "::"]:
            content = " " + content
        if not no_space and content not in ["*", "{", "C", '"', "'", "::"]:
            content += " "
        return content
    string = ""
    in_string_stage = 0
    for child in node.children:
        c_str = child.text.decode()
        if c_str in ['"', "'"]:
            in_string_stage = 1 - in_string_stage
            string += c_str
            continue
        if in_string_stage:
            string += reconstruct_statement(combined_driver, child ,replace_items, no_space=True)
        else:
            string += reconstruct_statement(combined_driver, child ,replace_items)
    return string

########################
#  Processing
########################
def gen_init_for_extracted_code(extract_code, fuzz_init_code, target_func, matched_func_args):
    var_types = ["identifier", "literal", "field_expression", "subscript_expression"]
    def _reconstruct_for_variables(combined_driver, node, rewrite_variables):
        if node.type == "call_expression" and node.named_children[0].text.decode() == target_func:
            return node.text.decode()
        string = ""
        if node.type in var_types:
            if node.parent.type == "call_expression":
                return node.text.decode()
            var_name = node.text.decode()
            if var_name in rewrite_variables:
                return " " + rewrite_variables[var_name]
        if not node.children:
            content = node.text.decode()
            if content not in ["{", "}", ":", ";", ",", "C", "("]:
                content = " " + content
            if content in ["{", "}", ":", ";"]:
                content += "\n"
            return content
        for child in node.children:
            string += _reconstruct_for_variables(combined_driver, child, rewrite_variables)
        return string

    temp_code = "int LLVMFuzzerTestOneInput(uint8_t* _IN_BUFFER, size_t _IN_BUFFER_SIZE){\n"
    temp_code += fuzz_init_code
    temp_code += "\n"
    temp_code += "__EXTRACT_CODE();\n{\n"
    temp_code += extract_code
    temp_code += "\n}\n"
    temp_code += "}\n"
    combined_driver = CombinedDriver(src_language="cpp", src_code=temp_code, output_file="",
                                     graph_format="dot", codeviews=codeviews)
    rda_table = combined_driver.results["DFG"].rda_table
    AST_driver = combined_driver.results["AST"]
    DFG_driver = combined_driver.results["DFG"]
    rewrite_variables = {}
    for var in rda_table[-1]["def"]:
        if var.name in ["_IN_BUFFER", "_IN_BUFFER_SIZE"] + matched_func_args:
            continue
        new_name = f"__{var.name.replace('.', '_')}" 
        rewrite_variables[var.name] = new_name
    if not rewrite_variables:
        return extract_code
    pre_extract_id = search_function_call(AST_driver, "__EXTRACT_CODE",
                                            DFG_driver=DFG_driver)
    extract_node = AST_driver.orig_nodes[pre_extract_id[0]].next_sibling
    new_extract_code = ""
    # TODO: Very hacky
    new_extract_code += "// TODO: very hacky. Who knows whether this will work\n"
    for var in rewrite_variables:
        new_extract_code += f"int {rewrite_variables[var]} = 5;\n"
    for node in extract_node.named_children:
        new_extract_code += _reconstruct_for_variables(combined_driver, node, rewrite_variables)
    return new_extract_code
