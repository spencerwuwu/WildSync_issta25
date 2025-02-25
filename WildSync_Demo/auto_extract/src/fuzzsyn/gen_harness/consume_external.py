#!/usr/bin/env python3
from loguru import logger
from comex.codeviews.combined_graph.combined_driver import CombinedDriver
from comex.utils.preprocessor import remove_empty_lines, remove_comments
from ..utils import remove_if_def_include
from ..utils import codeviews
from ..utils import search_function_call

import copy


def try_consume_original_code(code_file, target_func):
    try:
        file_handle = open(code_file, "r")
        src_code = file_handle.read()
        file_handle.close()

        src_code = remove_if_def_include(src_code)
        src_code = remove_empty_lines(src_code)
        src_code = remove_comments("cs", src_code)
        setting = copy.deepcopy(codeviews)
        setting["DFG"]["exists"] = False
        setting["CFG"]["exists"] = False
        combined_driver = CombinedDriver(src_language="cpp", src_code=src_code, output_file="", graph_format="dot", codeviews=setting)
    except:
        logger.debug("Failed to parse code file: {}".format(code_file))
        return {}

    AST_driver = combined_driver.results["AST"]
    function_searches = search_function_call(AST_driver, target_func,
                                             DFG_driver=None, to_cfg_node=False)
    functions_with_target = {}
    for function_search in function_searches:
        node = AST_driver.orig_nodes[function_search]
        no_found = False
        while node.type != "function_definition":
            if not node.parent:
                no_found = True
                break
            node = node.parent
        if no_found:
            continue
        if node.parent.type == "linkage_specification":
            node = node.parent
        lineno = node.start_point[0]
        if lineno not in functions_with_target:
            functions_with_target[lineno] = node.text.decode()
            logger.info("Located usage in {}:{}".format(code_file, lineno))
    return functions_with_target


def find_functions_in_original_code(code_file, target_func):
    try:
        file_handle = open(code_file, "r")
        src_code = file_handle.read()
        file_handle.close()

        src_code = remove_if_def_include(src_code)
        src_code = remove_empty_lines(src_code)
        src_code = remove_comments("cs", src_code)
        setting = copy.deepcopy(codeviews)
        setting["DFG"]["exists"] = False
        setting["CFG"]["exists"] = False
        combined_driver = CombinedDriver(src_language="cpp", src_code=src_code, output_file="", graph_format="dot", codeviews=setting)
    except:
        logger.debug("Failed to parse code file: {}".format(code_file))
        return {}

    AST_driver = combined_driver.results["AST"]
    function_searches = search_function_call(AST_driver, target_func,
                                             DFG_driver=None, to_cfg_node=False)
    rets = [AST_driver.orig_nodes[idx] for idx in function_searches]
    return rets
