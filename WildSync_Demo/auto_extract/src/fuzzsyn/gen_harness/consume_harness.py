from loguru import logger
from comex.codeviews.combined_graph.combined_driver import CombinedDriver
from comex.utils.preprocessor import remove_empty_lines, remove_comments
from fuzzsyn.utils import codeviews
from fuzzsyn.utils import search_function_call,  get_target_function_node, search_function_definition, search_function_call_from_node
from fuzzsyn.utils import get_function_parameters, handle_cpp_function_parameter
    


def _consume_original_code(combined_driver, replace_function_name):
    AST_Driver = combined_driver.results["AST"]
    cfg = combined_driver.results["CFG"].graph

    # Find LLVMFuzzerTestOneInput entry
    search_llvm = search_function_definition(combined_driver, "LLVMFuzzerTestOneInput")
    if not search_llvm:
        logger.error("LLVMFuzzerTestOneInput not found")
        raise NotImplementedError("LLVMFuzzerTestOneInput not found")
    fuzzer_entry = search_llvm[0]
    # extract and replace fuzzing parameters
    fuzzer_parameters = get_function_parameters(fuzzer_entry)
    if len(fuzzer_parameters) != 2:
        raise NotImplementedError("LLVMFuzzerTestOneInput parameters wrong")
    buffer_node = handle_cpp_function_parameter(fuzzer_parameters[0])
    buflen_node = handle_cpp_function_parameter(fuzzer_parameters[1])
    fuzzer_statement_id = AST_Driver.parser.index[(fuzzer_entry.start_point, fuzzer_entry.end_point, fuzzer_entry.type)]
    fuzzer_statement = fuzzer_entry
    while not cfg.has_node(fuzzer_statement_id):
        fuzzer_statement = fuzzer_statement.parent
        fuzzer_statement_id = AST_Driver.parser.index[
            (fuzzer_statement.start_point, fuzzer_statement.end_point, fuzzer_statement.type)
        ]
    debug_msg  = f"\nFuzzer located, {cfg.nodes[fuzzer_statement_id]['label']}"
    debug_msg += f"\n                parameters to be replace: {buffer_node['name']} {buflen_node['name']}"
    logger.info(debug_msg)

    #search_results = search_function_call(combined_driver.results["AST"], replace_function_name,DFG_driver=combined_driver.results["DFG"])
    search_results = []
    found_replace = search_function_call_from_node(AST_Driver, fuzzer_entry, replace_function_name, search_results, 
                                                   DFG_driver=combined_driver.results["DFG"])
    if not found_replace:
        msg = f"Function {replace_function_name} not found in LLVMFuzzerTestOneInput"
        logger.error(msg)
        raise NotImplementedError(msg)
    # TODO: handle multiple results, currently using the location of first found
    replace_statement_id = min(search_results)
    root_node = AST_Driver.orig_nodes[replace_statement_id]
    func_node = get_target_function_node(root_node, replace_function_name)
    if not func_node:
        raise NotImplementedError(f"Function {replace_function_name} not found")
    #arguments = [node.text.decode() for node in func_node.named_children[-1].named_children]
    arguments = func_node.named_children[-1].named_children
    while not cfg.has_node(replace_statement_id):
        root_node = root_node.parent
        replace_statement_id = AST_Driver.parser.index[
            (root_node.start_point, root_node.end_point, root_node.type)
        ]
    cfg_node = cfg.nodes[replace_statement_id]
    logger.info(f"\nReplace statement located: {cfg_node['label']}")

    # Get declared names
    declared_names = []
    for v in combined_driver.results["DFG"].rda_table[-1]["declared"]:
        if v.is_array:
            declared_names.append(v.field_parent_name)
        else:
            declared_names.append(v.name)
    return {
        "fuzzer_entry_node": fuzzer_statement,
        "buffer_node": buffer_node,
        "buflen_node": buflen_node,
        "replace_statement_line": cfg_node["lineno"],
        "arguments": arguments,
        "declared_names": declared_names,
        "combined_driver": combined_driver,
    }


def consume_original_code(harness_file, input_data):
    logger.info("Consume harness file: {}".format(harness_file))
    file_handle = open(harness_file, "r")
    src_code = file_handle.read()
    file_handle.close()
    #src_code = remove_if_def_include(src_code)
    src_code = remove_empty_lines(src_code)
    src_code = remove_comments("cs", src_code)
    combined_driver = CombinedDriver(src_language="cpp", src_code=src_code, output_file="",
                                     graph_format="dot", codeviews=codeviews)

    return _consume_original_code(combined_driver, input_data)
