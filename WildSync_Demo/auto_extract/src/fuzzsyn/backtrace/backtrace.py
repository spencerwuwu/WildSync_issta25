#!/usr/bin/env python3
import re
from comex.codeviews.combined_graph.combined_driver import CombinedDriver
from comex.codeviews.SDFG.SDFG_csharp import recursively_get_children_of_types
from comex.utils.preprocessor import remove_empty_lines, remove_comments
from comex.codeviews.AST.AST_driver import ASTDriver
from fuzzsyn.utils import codeviews
from fuzzsyn.utils import search_function_call, get_target_function_node
from fuzzsyn.utils import get_function_parameters, handle_cpp_function_parameter
from fuzzsyn.utils import remove_if_def_include
from fuzzsyn.utils import is_primitive_type, is_known_function, is_substitute_function
from fuzzsyn.utils import replace_function_existing_arguments, reconstruct_statement

from fuzzsyn.utils import timeout

from loguru import logger

from .utils import *


def backtrace_dfg(DFG_driver, statement_id, arguments):

    rda_table = DFG_driver.rda_table

    def _traverse_up(dfg, source_node_id, key):
        marked_node_ids = []
        marked_nodes = []
        seen_nodes = [source_node_id]
        new_nodes = [source_node_id]
        while new_nodes:
            cur_node_id = new_nodes.pop()
            seen_nodes.append(cur_node_id)
            for in_edge in dfg.in_edges(cur_node_id, data=True):
                source_node = dfg.nodes[in_edge[0]]
                source_node_id = in_edge[0]
                if source_node_id not in seen_nodes:
                    related_varables = in_edge[2]["used_def"].split(',')
                    if key in related_varables:
                        marked_nodes.append(source_node)
                        marked_node_ids.append(source_node_id)
                        new_nodes.append(source_node_id)
                else:
                    continue
        return marked_node_ids, marked_nodes

    def _traverse_down(dfg, mark_node_ids, key, target_line):
        related_nodes = []
        related_node_ids = []
        seen_nodes = []
        new_nodes = [node for node in mark_node_ids]
        while new_nodes:
            cur_node_id = new_nodes.pop()
            seen_nodes.append(cur_node_id)
            for out_edge in dfg.out_edges(cur_node_id, data=True):
                sink_node = dfg.nodes[out_edge[1]]
                sink_node_id = out_edge[1]
                sink_lineno = sink_node["lineno"]
                if sink_node_id not in seen_nodes and sink_lineno < target_line:
                    # Cheeck if key is defined as a field name
                    def_set = rda_table[sink_node_id]["def"]
                    related_varables = out_edge[2]["used_def"].split(',')
                    if key in related_varables:
                        found = False
                        for item in def_set:
                            if item.field_parent_name and item.field_parent_name == key[1:-1]:
                                marked_nodes.append(sink_node)
                                marked_node_ids.append(sink_node_id)
                                new_nodes.append(sink_node_id)
                                found = True
                                break
                        if not found:
                            related_nodes.append(sink_node)
                            related_node_ids.append(sink_node_id)
                            new_nodes.append(sink_node_id)
                else:
                    continue
        return related_node_ids, related_nodes

    dfg = DFG_driver.graph
    statment_node = dfg.nodes[statement_id]
    final_results = {}
    for argument in arguments:
        if not argument:
            final_results[argument] = {}
            continue
        marked_node_ids, marked_nodes = _traverse_up(dfg, statement_id, argument)
        related_node_ids, related_nodes = _traverse_down(dfg, marked_node_ids, argument, statment_node['lineno'])
        final_results[argument] = {
            "source_node": statment_node,
            "source_node_id": statement_id,
            "marked_nodes": marked_nodes,
            "marked_node_ids": marked_node_ids,
            "related_nodes": related_nodes,
            "related_node_ids": related_node_ids,
            "critical_node_ids": [],
            "critical_nodes": [],
            "passby_node_ids": [],
            "passby_nodes": [],
        }

    return final_results


def backtrace_cfg_from_dfg_traces(CFG_driver, dfg_result):
    if not dfg_result:
        return
    """
    dfg_result = {
            "source_node": statement_node,
            "source_node_id": statement_node_id,
            "marked_nodes": marked_nodes,
            "marked_node_ids": marked_node_ids,
            "related_nodes": related_nodes,
            "related_node_ids": related_node_ids,
            "critical_node_ids": [],
            "critical_nodes": [],
            "passby_node_ids": [],
            "passby_nodes": [],
    }
    """
    def _traverse_up(cfg, start_node_id, seen_node_ids, 
                     critical_node_ids=[], critical_nodes=[], passby_node_ids=[], passby_nodes=[]):
        critical_edges = ["pos_next", "neg_next", "loop_update", "loop_control", "switch_case"]
        #passby_edges   = ["next_line", "jump_next", "fall_through", "switch_out"]
        new_node_ids = [start_node_id]
        while new_node_ids:
            cur_node_id = new_node_ids.pop()
            if cur_node_id in seen_node_ids:
                continue
            seen_node_ids.append(cur_node_id)
            for in_edge in cfg.in_edges(cur_node_id, data=True):
                edge_type = in_edge[2]["controlflow_type"]
                if edge_type == "loop_update" or edge_type =="loop_control":
                    continue
                cur_lineno = cfg.nodes[cur_node_id]["lineno"]
                sink_node_id = in_edge[0]
                sink_node_lineno = cfg.nodes[sink_node_id]["lineno"]
                if cur_lineno < sink_node_lineno:
                    continue
                if edge_type in critical_edges:
                    # Check if there are data depeneded lines before this edge
                    for marked_node in dfg_result["marked_nodes"]:
                        if marked_node["lineno"] < sink_node_lineno:
                            critical_nodes.append(cfg.nodes[sink_node_id])
                            critical_node_ids.append(sink_node_id)
                            break
                            print(cfg.nodes[sink_node_id])
                else:
                    passby_nodes.append(cfg.nodes[sink_node_id])
                    passby_node_ids.append(sink_node_id)
                if sink_node_id not in seen_node_ids: 
                    new_node_ids.append(sink_node_id)

    cfg = CFG_driver.graph
    # Backtrace from each data-dependency makred statements
    seen_node_ids = []
    critical_nodes = []
    critical_node_ids = []
    passby_nodes = []
    passby_node_ids = []
    for idx in range(len(dfg_result["marked_nodes"])):
        marked_node_id = dfg_result["marked_node_ids"][idx]
        #cfg_node = cfg.nodes[marked_node_id]
        if marked_node_id < 0:
            continue
        _traverse_up(cfg, marked_node_id, seen_node_ids, critical_node_ids, critical_nodes, passby_node_ids, passby_nodes)
    # Also traverse up from source node
    _traverse_up(cfg, dfg_result["source_node_id"], seen_node_ids, critical_node_ids, critical_nodes, passby_node_ids, passby_nodes)
    dfg_result["critical_node_ids"] = critical_node_ids
    dfg_result["critical_nodes"] = critical_nodes
    dfg_result["passby_node_ids"] = passby_node_ids
    dfg_result["passby_nodes"] = passby_nodes
    

def _get_track_variables_from_function(AST_driver, target_func_node, target_arg_types, arg_indexes, matched_function_args):
    variable_types = ["identifier", "literal", "field_expression", "subscript_expression"]
    track_variable_list = set()
    arguments = target_func_node.named_children[-1]
    argument_track_map = []
    argument_type_map = {}
    argu_index = -1
    # HACK: purely to get fribidi working
    rewrite_names = {"av_malloc_array": "calloc"}
    for argument_node in arguments.named_children:
        argu_index += 1
        single_track_list = set()
        if argu_index not in arg_indexes:
            argument_track_map.append({"ast_node": argument_node, "track_list": []})
            if argument_node.type in variable_types:
                rewrite_names[argument_node.text.decode()] = matched_function_args[argu_index]
            continue
        argu_nodes = recursively_get_self_n_children_of_types(argument_node, ["identifier", "field_expression", "subscript_expression"])
        for argu_node in argu_nodes:
            # TODO: Skip function call here???
            #if argu_node.parent.type == "call_expression":
            #    continue
            var_name = f"'{argu_node.text.decode()}'"
            track_variable_list.add(var_name)
            single_track_list.add(var_name)
            if len(target_arg_types) > 0 and len(argu_nodes) == 1:
                argument_type_map[var_name[1:-1]] = target_arg_types[argu_index]
        argument_track_map.append({
            "ast_node": argument_node, 
            "track_list": list(single_track_list)
        })
    return argument_track_map, track_variable_list, argument_type_map, rewrite_names


def find_and_backtrace_target_function(combined_driver, target_function, target_arg_types, arg_indexes, matched_function_args):
    AST_driver = combined_driver.results["AST"]
    CFG_driver = combined_driver.results["CFG"]
    DFG_driver = combined_driver.results["DFG"]

    function_searches = search_function_call(AST_driver, target_function,
                                             DFG_driver=DFG_driver)
    search_results = []
    for statement_id in function_searches:
        func_node = AST_driver.orig_nodes[statement_id]
        func_node = get_target_function_node(func_node, target_function)
        argument_track_map, track_list, argument_type_map, rewrite_names = _get_track_variables_from_function(AST_driver, func_node, target_arg_types, arg_indexes, matched_function_args)
        search_results.append((statement_id, track_list, argument_track_map, argument_type_map, rewrite_names))
    if len(search_results) == 0:
        logger.error("Target function not found")
        return {}

    final_results = {}
    # start backtrace
    for statement_id, track_variable, argument_track_map, argument_type_map, rewrite_names in search_results:
        statement_node = DFG_driver.graph.nodes[statement_id]
        logger.info(f"\nBacktrace: {statement_node['label']}")
        logger.info(f"\nVariables: {track_variable}")
        track_results = backtrace_dfg(DFG_driver, statement_id, track_variable)
        for _, result in track_results.items():
            backtrace_cfg_from_dfg_traces(CFG_driver, result)
        final_results[statement_id] = {
            "track_results": track_results,
            "argument_track_map": argument_track_map,
            "argument_type_map": argument_type_map,
            "rewrite_names": rewrite_names
        }

    return final_results


def summarize_essential_nodes(trace_result, source_id, source_node, keep_related=False):
    backtrace_results = trace_result["track_results"]
    data = {
        "track_variables": [],
        "extracted_nodes": [],
        "extracted_node_ids": [],
        "marked_node_ids": [],
    }
    extracted_nodes = {}
    mark_node_ids = []
    related_node_ids = []
    for arg, content in backtrace_results.items():
        data["track_variables"].append(arg)
        nodes = content["marked_nodes"] + content["critical_nodes"]
        node_ids = content["marked_node_ids"] + content["critical_node_ids"]
        if keep_related:
            nodes += content["related_nodes"]
            node_ids += content["related_node_ids"]
        for idx in range(len(nodes)):
            lineno = nodes[idx]["lineno"]
            if lineno not in extracted_nodes:
                extracted_nodes[lineno] = {"node": nodes[idx], "id": node_ids[idx]}
        for node_id in content["marked_node_ids"]:
            if node_id not in mark_node_ids:
                mark_node_ids.append(node_id)
        if keep_related:
            for node_id in content["related_node_ids"]:
                if node_id not in related_node_ids:
                    related_node_ids.append(node_id)
    source_line = source_node["lineno"]
    if source_line not in extracted_nodes:
        extracted_nodes[source_line] = {"node": source_node, "id": source_id}
    linenos = sorted(extracted_nodes.keys())
    for lineno in linenos:
        data["extracted_nodes"].append(extracted_nodes[lineno]["node"])
        data["extracted_node_ids"].append(extracted_nodes[lineno]["id"])
    data["marked_node_ids"] = mark_node_ids
    if keep_related:
        data["marked_node_ids"] += related_node_ids
    return data


def reconstruct_rhs_node(combined_driver, node, replace_items={}):
    AST_driver = combined_driver.results["AST"]
    node_id = AST_driver.parser.index.get(
        (node.start_point, node.end_point, node.type), -1)
    if node_id == -1:
        return node.text.decode()
    if node_id in replace_items:
        return replace_items[node_id]
    string = ""
    if node.type in ["identifier", "literal", "pointer_type"]:
        return node.text.decode()
    if not node.children:
        return node.text.decode()
    string = ""
    for child in node.children:
        string += reconstruct_rhs_node(combined_driver, child ,replace_items)
    return string


def reconstruct_fuzz_statement(combined_driver, node, replace_items={}):
    if node.type == "expression_statement":
        node = node.children[0]
    if node.type == "local_declaration_statement":
        declaration_node = [c for c in node.children if c.type == "variable_declaration"]
        declaration_node = declaration_node[0]
        lhs_type_node = declaration_node.children[0]
        lhs_value_node = declaration_node.children[1].children[0]
        rhs_node = declaration_node.children[1].children[1]
        rhs = reconstruct_rhs_node(combined_driver, rhs_node, replace_items)
        lhs = f"{lhs_type_node.text.decode()} {lhs_value_node.text.decode()}"
        statement = f"{lhs} {rhs};\n"
    elif node.type == "assignment_expression":
        lhs_value_node, assignment_operator, rhs_node = node.children
        rhs = reconstruct_rhs_node(combined_driver, rhs_node, replace_items)
        lhs = f"{lhs_value_node.text.decode()} {assignment_operator.text.decode()}"
        statement = f"{lhs} {rhs};\n"
    else:
        logger.error(f"\nNotImplementedError: reconstruction of {node.text.decode()}")
        raise NotImplementedError(f"\nNotImplementedError: reconstruction of {node.text.decode()}")
    return statement


def handle_variable_declaration(variable, combined_driver, func_param_dict, extracted_node_ids, argument_type_map, lib_symbols):
    rda_table = combined_driver.results["DFG"].rda_table
    cfg = combined_driver.results["CFG"].CFG.graph
    AST_driver = combined_driver.results["AST"]

    def _handle_rhs_node(rhs_node, cur_statement_id):
        token_types = ["call_expression", "field_expression", "subscript_expression","identifier"]
        fuzz_here = set()
        replace_items = {}
        for element in recursively_get_self_n_children_of_types(rhs_node, token_types):
            element_id = AST_driver.parser.index[
                (element.start_point, element.end_point, element.type)
            ]
            if element.type == "call_expression":
                method_cs = element.children[0]
                if is_known_function(method_cs.text.decode(), lib_symbols):
                    # TODO: handle know functions
                    # TODO: hand it to the second iteration for now
                    msg = f"\nTODO: recursively handle known functions {method_cs.text.decode()}, currently keep in place"
                    logger.warning(msg)
                    #raise NotImplementedError(msg)
                elif is_substitute_function(method_cs.text.decode()):
                    # TODO: handle know functions
                    replace_items[element_id] = is_substitute_function(method_cs)
                    msg = f"\nTODO: recursively handle substitute functions {method_cs.text.decode()}, currently keep in place"
                    logger.warning(msg)
                    #raise NotImplementedError(msg)
                else:
                    replace_items[element_id] = "<UNKNOWN FUNC>"
                    logger.debug(f"\nUnknown function '{method_cs.text.decode()}' accessed at {cur_statement_id}")
                    fuzz_here.add(element_id)
            else:
                name = element.text.decode()
                found = False
                for used in rda_table[cur_statement_id]["use"]: 
                    if name != used.name:
                        continue
                    found = True
                    if not used.line and name not in func_param_dict:
                        if used.method_call:
                            replace_items[element_id] = "<UNKNOWN FUNC>"
                        else:
                            if re.match(r"[A-Z0-9_]+", name):
                                logger.debug(f"\nKeep var '{name}' as constant at {cur_statement_id}")
                                continue
                            replace_items[element_id] = "<UNKNOWN VAR>"
                        logger.debug(f"\nUnknown var '{name}' accessed at {cur_statement_id}")
                        fuzz_here.add(element_id)
                    else:
                        # Defined variable, do nothing
                        pass
                if not found:
                    # TODO
                    replace_items[element_id] = "<UNDEFINED VAR>"
                    logger.debug(f"\nUndefined var '{name}' accessed at {cur_statement_id}")
                    fuzz_here.add(element_id)
        return sorted(list(fuzz_here)), replace_items

    def _handle_assignment_node(node, cur_statement_id):
        result = {
            "lhs_type_node": None,
            "rhs_node": None,
            #"rhs_replace_items": {},
            #"rewrite_statement": "",
            "fuzz_locations": []
        }
        if len(node.children) != 3:
            logger.error(f"\nNotImplementedError: len({node.text.decode()}) != 3")
            raise NotImplementedError(f"\nNotImplementedError: len({node.text.decode()}) != 3")
        lhs_value_node, assignment_operator, rhs_node = node.children
        result["rhs_node"] = rhs_node
        fuzz_here_ids, replace_items = _handle_rhs_node(rhs_node, cur_statement_id)
        #result["rhs_replace_items"] = replace_items
        result["fuzz_locations"] = fuzz_here_ids
        
        #rhs = reconstruct_rhs_node(combined_driver, result["rhs_node"], replace_items)
        #lhs = f"{lhs_value_node.text.decode()} {assignment_operator.text.decode()}"
        #result["rewrite_statement"] = f"{lhs} {rhs};"
        return result

    def _get_type(declare_node, name):
        type_node = None
        pointer_stars = 0
        init_node = None
        found = False
        start_idx = 1
        if declare_node.named_children[0].type == "type_qualifier":
            type_node = declare_node.named_children[1]
            start_idx = +1
        else:
            type_node = declare_node.named_children[0]
        idx = start_idx
        while idx < len(declare_node.named_children):
            child = declare_node.named_children[idx]
            if child.type == "init_declarator":
                init_node = child.named_children[-1]
                child = child.named_children[0]
            if re.match(r"\**{}$".format(name), child.text.decode()):
                pointer_stars = len(child.text.decode()) - len(name)
                found = True
                break
            idx += 1
        if found:
            return type_node, "*" * pointer_stars, init_node
        return None, None, None


    def _handle_init_declaration_node(node, cur_statement_id):
        result = {
            "lhs_type_node": None,
            "pointer_stars": "",
            "rhs_node": None,
            #"rhs_replace_items": {},
            #"rewrite_statement": "",
            "fuzz_locations": []
        }
        lhs_type_node, pointer_stars, init_node = _get_type(node, variable)
        if not lhs_type_node:
            msg = "NotImplementedError: misunderstanding in _handle_init_declaration_node for {}".format(node.text.decode())
            logger.error(msg)
            raise NotImplementedError(msg)
        result["lhs_type_node"] = lhs_type_node

        result["pointer_stars"] = pointer_stars

        rhs_node = init_node
        result["rhs_node"] = rhs_node
        fuzz_here_ids, replace_items = _handle_rhs_node(rhs_node, cur_statement_id)
        #result["rhs_replace_items"] = replace_items
        result["fuzz_locations"] = fuzz_here_ids
        
        #rhs = reconstruct_rhs_node(combined_driver, result["rhs_node"], replace_items)
        #lhs = f"{lhs_type_node.text.decode()} {"*"*pointer_cnt} {lhs_value_node.text.decode()}"
        #result["rewrite_statement"] = f"{lhs} {rhs};"
        return result

    def _get_lhs_type(node):
        if node.type == "declaration":
            lhs_type_node, pointer_stars, init_node = _get_type(node, variable)
            if not lhs_type_node:
                raise NotImplementedError("NotImplementedError: misunderstanding in _get_lhs_type for {}".format(node.text.decode()))
            return lhs_type_node, pointer_stars
        else:
            msg = f"\nNotImplementedError: _get_lhs_type with type '{node.type}' in {node.text.decode()}"
            logger.error(msg)
            raise NotImplementedError(msg)

    def _find_declare_n_first_assign_node_id(variable):
        declare_node_id = -2
        first_assign_node_id = -2
        for node_id in extracted_node_ids:
            rda_result = rda_table[node_id]
            for v in rda_result["def"]:
                if v.name == variable:
                    if node_id == -1:
                        declare_node_id = node_id
                    else:
                        if declare_node_id == -2:
                            declare_node_id = node_id
                        ast_node = AST_driver.orig_nodes[node_id]
                        # Check if the declaration node has init value
                        if ast_node.type == "declaration":
                            _, _, init_node = _get_type(ast_node, variable)
                            if not init_node:
                                break
                        first_assign_node_id = node_id
                    break
            if first_assign_node_id != -2:
                break
        return declare_node_id, first_assign_node_id

    return_value = {
        "rewrite_node_id": -1,
        "require_fuzz": True,
        "require_declare": False,
        "require_assign": False,
        "variable_type_node": None, 
        "pointer_stars": "",
        "rewrite_name": "",
        #"rewrite_statement": "",
        "data": None
    }

    declare_node_id, first_assign_node_id = _find_declare_n_first_assign_node_id(variable)

    # Need to declare the variable
    if declare_node_id == -1:
        return_value["require_declare"] = True
        # First see if varaible is a function parameter
        if variable in func_param_dict:
            var_type_node = func_param_dict[variable]["type"]
            pointer_stars = func_param_dict[variable]["pointer_stars"]
            return_value["variable_type_node"] = var_type_node
            return_value["pointer_stars"] = pointer_stars
            if first_assign_node_id < 0:
                return_value["require_assign"] = True
                # TODO: what to do with non-primitive type
                #if is_primitive_type(var_type_node.text.decode()):
                #    pass
                logger.debug(f"\n'{var_type_node} {variable}' is a function parameter, need declared and init")
                #new_line = f"{var_type_node.text.decode()} {variable} = <UNKNOWN VAR>;"
                #return_value["rewrite_statement"] = new_line
                return return_value
            else:
                # Hanlde assign later
                pass
        else:
            # TODO: need more process for don't-know-type variabels
            if first_assign_node_id == -1:
                logger.error(f"{variable} is not declared, need to find its type and declared")
                raise NotImplementedError(f"{variable} is not declared, need to find its type and declared")
            else:
                return_value["require_declare"] = True

    # Check if field_expression parrent 
    parent_name = ""
    if "." in variable:
        parent_name =  variable.rsplit(".")[0]
    elif "->" in variable:
        parent_name = variable.rsplit("->")[0]
    elif "[" in variable:
        parent_name = variable.rsplit("[")[0]

    if not parent_name:
        first_assign_parent_id = -1
    else:
        _, first_assign_parent_id = _find_declare_n_first_assign_node_id(parent_name)
        if first_assign_parent_id < 0:
            new_name = f"__{variable.replace('.', '_').replace('->', '__').replace('[', '').replace(']', '')}" 
            return_value["rewrite_name"] = new_name
            return_value["require_declare"] = True
            logger.debug(f"Rewriting {variable} to {new_name}")
        if variable in argument_type_map and argument_type_map[variable]["is_primitive"]:
            # TODO
            return_value["variable_type_node"] = argument_type_map[variable]["type"]
            return_value["pointer_stars"] = argument_type_map[variable]["pointer_stars"]

    # The variable is init at this node
    if first_assign_node_id < 0:
        analysis_node_id = declare_node_id
    else:
        analysis_node_id = first_assign_node_id
    if analysis_node_id < 0:
        if "." not in variable and "->" not in variable and "[" not in variable:
            # TODO: if no declaration or assignment found, assume it's global variable that can be directly used 
            return_value["require_fuzz"] = False
            return_value["require_declare"] = False
            return_value["rewrite_name"] = variable
            return return_value
        if first_assign_parent_id < 0:
            # Check if this is a primitve type that we can fuzz to
            if variable in argument_type_map and argument_type_map[variable]["is_primitive"]:
                # TODO
                return_value["require_assign"] = True
                return return_value
            else:
                msg = f"\nNotImplementedError: '{variable}' nor field '{parent_name}' are assigned with value"
                logger.error(msg)
                raise NotImplementedError(msg)
        else:
            return None
    target_cfg_node = cfg.nodes[analysis_node_id]
    target_ast_node = AST_driver.orig_nodes[analysis_node_id]
    if target_ast_node.type == "expression_statement":
        target_ast_node = target_ast_node.children[0]
    if target_ast_node.type not in ["assignment_expression", "declaration", "call_expression", "for_statement", "if_statement", "while_statement", "switch_statement"]:
        msg = f"\nNotImplementedError: '{variable}' first access '{target_cfg_node['label']}' is '{target_ast_node.type}'"
        logger.error(msg)
        raise NotImplementedError(msg)

    if target_ast_node.type == "declaration":
        ret = _handle_init_declaration_node(target_ast_node, first_assign_node_id)
        return_value["rewrite_node_id"] = analysis_node_id
        return_value["variable_type_node"] = ret["lhs_type_node"]
        return_value["pointer_stars"] = ret["pointer_stars"]
        #return_value["rewrite_statement"] = ret["rewrite_statement"]
        return_value["data"] = ret
    elif target_ast_node.type == "assignment_expression":
        ret = _handle_assignment_node(target_ast_node, first_assign_node_id)
        # TODO: if var_type is none, find declaration ...
        if not ret["lhs_type_node"]:
            if return_value["variable_type_node"]:
                ret["lhs_type_node"], ret["pointer_stars"] = return_value["variable_type_node"], return_value["pointer_stars"]
            else:
                if declare_node_id == -1:
                    # TODO: leave the variable as it is as well
                    return None
                ret["lhs_type_node"], ret["pointer_stars"] = _get_lhs_type(AST_driver.orig_nodes[declare_node_id])
        return_value["rewrite_node_id"] = analysis_node_id
        return_value["variable_type_node"] = ret["lhs_type_node"]
        return_value["pointer_stars"] = ret["pointer_stars"]
        #return_value["rewrite_statement"] = ret["rewrite_statement"]
        return_value["data"] = ret
    elif target_ast_node.type in ["call_expression", "for_statement", "if_statement", "while_statement", "switch_statement"]:
        # For example: info in GetQuantizeInfo(&info);
        # For example: i in for ( int i = 0; i < npixes ; ++i )
        # TODO: feels like we don't need to do anything here
        return None
    else:
        # TODO
        msg = f"\nNotImplementedError: '{variable}' first access '{target_cfg_node['label']}' is '{target_ast_node.type}'"
        logger.error(msg)
        raise NotImplementedError(msg)
    if not return_value["data"]["fuzz_locations"]:
        return None
    return return_value


def gather_fuzz_statements(combined_driver, var_handle_results, orig_buffer_name="data", orig_length_name="size", more_fuzz_variables={}):
    AST_driver = combined_driver.results["AST"]
    fuzz_variables = {}
    pointers = 0
    for track_variable, var_handle_result in var_handle_results.items():
        if not var_handle_result["require_fuzz"]:
            continue
        fuzz_variable = {
            "type": "",
            "is_pointer": False,
            "ast_node_id": -1,
            "require_declare" : var_handle_result["require_declare"],
            "require_assign" : var_handle_result["require_assign"],
            "fuzz_variable_name": "",
        }
        if var_handle_result["require_assign"]:
            if not var_handle_result["variable_type_node"]:
                msg = f"\nNotImplementedError: no var type for {track_variable}"
                logger.error(msg)
                raise NotImplementedError(msg)
            type_node_name = var_handle_result["variable_type_node"]
            if var_handle_result["pointer_stars"]:
                fuzz_variable["is_pointer"] = True
                pointers += 1
            fuzz_variable["type"] = type_node_name + var_handle_result["pointer_stars"]
            fuzz_variable["fuzz_variable_name"] = f"__fuzz_{len(fuzz_variables)}"
            if var_handle_result["rewrite_name"]:
                fuzz_variables[var_handle_result["rewrite_name"]] = fuzz_variable
            else:
                fuzz_variables[track_variable] = fuzz_variable
            var_handle_results[track_variable]["fuzz_variable"] = fuzz_variable
        else:
            if not var_handle_result["data"]["fuzz_locations"]:
                orig_node = AST_driver.orig_nodes[var_handle_result["rewrite_node_id"]]
                msg = f"\nNo fuzz locations for {orig_node.text.decode()}"
                logger.error(msg)
                raise NotImplementedError(msg)
            lhs_type_node = var_handle_result["data"]["lhs_type_node"]
            if var_handle_result["pointer_stars"]:
                fuzz_variable["is_pointer"] = True
                pointers += 1
            if hasattr(lhs_type_node, "text"):
                fuzz_variable["type"] = lhs_type_node.text.decode() + var_handle_result["pointer_stars"] 
            else:
                fuzz_variable["type"] = lhs_type_node + var_handle_result["pointer_stars"] 
            # TODO: only stop and replace at very simple cases
            fuzz_variable["ast_node_id"] = min(var_handle_result["data"]["fuzz_locations"])
            fuzz_variable["fuzz_variable_name"] = f"__fuzz_{len(fuzz_variables)}"
            if var_handle_result["rewrite_name"]:
                fuzz_variables[var_handle_result["rewrite_name"]] = fuzz_variable
            else:
                fuzz_variables[track_variable] = fuzz_variable
            var_handle_results[track_variable]["fuzz_variable"] = fuzz_variable
    for var_name, var_handle_result in more_fuzz_variables.items():
        if not var_handle_result["require_fuzz"]:
            continue
        fuzz_variable = {
            "type": "",
            "is_pointer": False,
            "ast_node_id": -1,
            "require_declare" : True,
            "require_assign" : True,
            "fuzz_variable_name": "",
        }
        type_node_name = var_handle_result["type_node"]
        if var_handle_result["pointer_stars"]:
            fuzz_variable["is_pointer"] = True
            pointers += 1
        fuzz_variable["type"] = type_node_name + var_handle_result["pointer_stars"]
        fuzz_variable["fuzz_variable_name"] = f"__fuzz_{len(fuzz_variables)}"
        fuzz_variables[var_name] = fuzz_variable
    # Synthesize init code
    # NOTE: Min_length = length of required variables + pointers with length=1 + save orig length (sizeof(char))
    #       essengially, the length of everything
    # TODO: analyze the required length of original buffer
    size_constraint_parts = []
    for track_var, v in fuzz_variables.items():
        size_constraint_parts.append(f"sizeof({v['type']})")
        #size_constraint += f" + sizeof({v['type']})"
        part = ""
        if not v["is_pointer"]:
            #part += f"{v['type']} {v['fuzz_variable_name']} = 0;\n"
            part += f"{v['type']} {v['fuzz_variable_name']};\n"
            part += f"memcpy(&{v['fuzz_variable_name']}, _IN_BUFFER + _BUF_POINTER, sizeof({v['type']}));\n"
            part += f"_BUF_POINTER += sizeof({v['type']});\n\n"
        else:
            part += f"{v['type']} {v['fuzz_variable_name']} = ({v['type']}) (_IN_BUFFER + _BUF_POINTER);\n"
            part += f"size_t {v['fuzz_variable_name']}_LEN = _IN_REMAIN_LEN;\n"
            part += f"_BUF_POINTER += {v['fuzz_variable_name']}_LEN;\n\n"
        if v["require_declare"]:
            part += f"{v['type']} {track_var};\n"
        if v["require_assign"]:
            if v['type'] == "char*":
                part += f"{track_var} = malloc({v['fuzz_variable_name']}_LEN + 1);\n"
                part += f"memcpy({track_var}, {v['fuzz_variable_name']}, {v['fuzz_variable_name']}_LEN);\n"
                part += f"{track_var}[{v['fuzz_variable_name']}_LEN] = 0;\n"
            elif v['type'] == "gchar*":
                part += f"{track_var} = malloc({v['fuzz_variable_name']}_LEN + 1);\n"
                part += f"memcpy({track_var}, {v['fuzz_variable_name']}, {v['fuzz_variable_name']}_LEN);\n"
                part += f"{track_var}[{v['fuzz_variable_name']}_LEN] = 0;\n"
            else:
                part += f"{track_var} = {v['fuzz_variable_name']};\n"
        v["init_code"] = part
    code = ""
    code += "size_t _BUF_POINTER = 0;\n"
    size_constraint = " + ".join(size_constraint_parts)
    if size_constraint:
        code += f"if (_IN_BUFFER_SIZE < {size_constraint} + 1) {{\n\treturn 0;\n}}\n"
    for v in fuzz_variables.values():
        if not v["is_pointer"]:
            code += v["init_code"]
    code += f"size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / {pointers + 1};\n\n"
    for v in fuzz_variables.values():
        if v["is_pointer"]:
            code += v["init_code"]

    code += f"uint8_t* {orig_buffer_name} = _IN_BUFFER + _BUF_POINTER;\n"
    code += f"size_t {orig_length_name} = _IN_REMAIN_LEN;\n\n"

    for track_variable, var_handle_result in var_handle_results.items():
        if var_handle_result["require_fuzz"]:
            continue
        if not var_handle_result["require_declare"]:
            continue
        if var_handle_result["rewrite_name"]:
            track_variable = var_handle_result["rewrite_name"]
        code += f"{var_handle_result['type_node']} {var_handle_result['pointer_stars']} {track_variable};\n"
    for var_name, var_handle_result in more_fuzz_variables.items():
        if var_handle_result["require_fuzz"]:
            continue
        if not var_handle_result["require_declare"]:
            continue
        code += f"{var_handle_result['type_node']} {var_handle_result['pointer_stars']} {var_name};\n"
    return fuzz_variables, code


def backtrace_function_in_file(code_file, target_function, 
                               matched_function_args,
                               orig_buffer_name="data", orig_length_name="size",
                               save_graph=False,
                               lib_symbols=[],
                               matched_func_variables=[],
                               target_arg_types=[],
                               declared_names=[],
                               keep_related=False):

    file_handle = open(code_file, "r")
    src_code = file_handle.read()
    file_handle.close()

    src_code = remove_if_def_include(src_code)
    src_code = remove_empty_lines(src_code)
    src_code = remove_comments("cs", src_code)

    # First, load teh whole program with AST only
    ast = ASTDriver(
               "cpp", src_code, "", codeviews["AST"]
            )
    function_searches = search_function_call(ast, target_function, to_cfg_node=False)
    code_to_extract = []
    for function_search in function_searches:
        node = ast.orig_nodes[function_search]
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
        code_to_extract.append(node.text.decode())
    code_to_extract = "\n".join(code_to_extract)
    if not code_to_extract:
        return []

    return backtrace_from_code(code_to_extract, target_function, 
                               matched_function_args,
                               orig_buffer_name, orig_length_name, save_graph,
                               lib_symbols,
                               matched_func_variables,
                               target_arg_types,
                               declared_names,
                               keep_related=keep_related)


def backtrace_from_code(code_to_extract, target_function, 
                        matched_function_args,
                        orig_buffer_name="data", orig_length_name="size",
                        save_graph=False,
                        lib_symbols=[],
                        matched_func_variables=[],
                        target_arg_types=[],
                        declared_names=[],
                        keep_related=False):
    if save_graph:
        output_file = "output.json"
        graph_format = "all"
    else:
        output_file = ""
        graph_format = "dot"

    try:
        with timeout(seconds=20):
            combined_driver = CombinedDriver(src_language="cpp", src_code=code_to_extract, output_file=output_file,
                                             graph_format=graph_format, codeviews=codeviews)
    except TimeoutError:
        msg = f"Timeout when parsing for {target_function}"
        logger.error(msg)
        raise TimeoutError(msg)
    except:
        logger.error(f"Error in parsing for {target_function}")
        exit(1)


    #function_params = get_function_parameters(combined_driver)
    it_print(combined_driver.results["DFG"])

    arg_indexes = []
    for index in range(len(matched_function_args)):
        if not matched_function_args[index]:
            arg_indexes.append(index)
    backtrace_sets = find_and_backtrace_target_function(combined_driver, target_function,target_arg_types, arg_indexes, matched_function_args)

    # Print backtrace for each vraible
    for results in backtrace_sets.values():
        backtrace_results = results["track_results"]
        #argument_track_map = results["argument_track_map"]
        for arg, content in backtrace_results.items():
            if not content:
                continue
            print_backtrace(code_to_extract, arg, content)

    export_results = []
    for source_statement_id, trace_result in backtrace_sets.items():
        source_node = combined_driver.results["DFG"].graph.nodes[source_statement_id]
        summarized_result = summarize_essential_nodes(trace_result, source_statement_id, source_node, keep_related=keep_related)
        debug_print = []
        for node in summarized_result["extracted_nodes"]:
            debug_print.append(node["label"])
        debug_print = "\n" + "\n".join(debug_print)
        logger.debug(f"\nExtracted nodes: {debug_print}")
        #return [debug_print]
        rewrite_statements = {}
        var_handle_results = {}
        cur_func_def_node = combined_driver.results["AST"].orig_nodes[source_statement_id]
        while cur_func_def_node.type not in ["function_definition"]:
            cur_func_def_node = cur_func_def_node.parent
        func_param_dict = {}
        for c in get_function_parameters(cur_func_def_node):
            ret = handle_cpp_function_parameter(c)
            if not ret:
                continue
            func_param_dict[ret["name"]] = ret
        for track_variable in summarized_result["track_variables"]:
            track_variable = track_variable[1:-1]
            if track_variable in declared_names:
                logger.debug(f"\nHACK: skip declared variable {track_variable} in handle_variable_declaration")
                continue
            var_handle_result = handle_variable_declaration(track_variable, combined_driver, 
                                        func_param_dict,
                                        summarized_result["extracted_node_ids"],
                                        trace_result["argument_type_map"],
                                                            lib_symbols)
            if not var_handle_result:
                continue
            #rewrite_id = var_handle_result["rewrite_node_id"]
            #rewrite_statement = var_handle_result["rewrite_statement"]
            #rewrite_statements[rewrite_id] = rewrite_statements.get(rewrite_id, "") + "\n" + rewrite_statement
            var_handle_results[track_variable] = var_handle_result
        fuzz_variables, init_code = gather_fuzz_statements(combined_driver, var_handle_results, orig_buffer_name, orig_length_name)

        rewrite_names = trace_result["rewrite_names"]
        for track_variable, var_handle_result in var_handle_results.items():
            if var_handle_result["rewrite_name"]:
                rewrite_names[track_variable] = var_handle_result["rewrite_name"]
            rewrite_id = var_handle_result["rewrite_node_id"]
            if rewrite_id == -1:
                continue
            fuzz_variable = var_handle_result["fuzz_variable"]
            replace_items = {fuzz_variable["ast_node_id"]: fuzz_variable["fuzz_variable_name"]}
            rewrite_node = combined_driver.results["AST"].orig_nodes[rewrite_id]
            #rewrite_statement = reconstruct_fuzz_statement(combined_driver, rewrite_node, replace_items)
            rewrite_statement = reconstruct_statement(combined_driver, rewrite_node, replace_items)
            #if rewrite_node.type == "if_statement":
            #    rewrite_statement = f"if ({rewrite_statement})"
            #    rewrite_node += rewrite_node.text.decode().rsplit(")", 1)[1]
            #if rewrite_node.type == "while_statement":
            #    rewrite_statement = f"while ({rewrite_statement})"
            #    rewrite_node += rewrite_node.text.decode().rsplit(")", 1)[1]
            rewrite_statements[rewrite_id] = rewrite_statements.get(rewrite_id, "") + rewrite_statement
        # TODO
        source_ast_node = combined_driver.results["AST"].orig_nodes[source_statement_id]
        target_func_node = get_target_function_node(source_ast_node, target_function)
        target_func_replaces = replace_function_existing_arguments(combined_driver, target_func_node, matched_function_args)
        rewrite_statement = reconstruct_statement(combined_driver, source_ast_node, target_func_replaces)
        #if source_ast_node.type == "while_statement":
        #    rewrite_statement = f"while ({rewrite_statement});\n"
        rewrite_statements[source_statement_id] = rewrite_statement
        logger.debug("\nFinish first round of extract and rewrite")
        extracted_code = export_extracted_code(combined_driver, 
                                  summarized_result["extracted_node_ids"], 
                                  summarized_result["marked_node_ids"],
                                  rewrite_statements)
        if target_function not in extracted_code:
            msg = f"Extracted code null for {target_function}"
            logger.error(msg)
            raise ValueError(msg)

        # Now, combine code and initilize all variables
        orig_var_types = combined_driver.results["DFG"].var_types
        new_extracted_code, new_handled_variables = complete_extracted_code(extracted_code, init_code, 
                                                                          target_function, matched_function_args, 
                                                                          func_param_dict, orig_var_types,
                                                                          matched_func_variables,
                                                                          declared_names, rewrite_names,
                                                                            arg_indexes)
        logger.debug("\nFinish second round of extract and rewrite")
        new_fuzz_variables, new_init_code = gather_fuzz_statements(combined_driver, var_handle_results, orig_buffer_name, orig_length_name, new_handled_variables)

        init_code = new_init_code
        fuzz_variables = new_fuzz_variables
        extracted_code = new_extracted_code
        #logger.debug("\nInit code:\n" + init_code)
        if not len(extracted_code.strip()):
            logger.warning(f"Extracted code null for {target_function}")
            continue

        export_result = {
            "summzied_result": summarized_result,
            "extracted_code": extracted_code,
            "init_code": init_code,
            "fuzz_variables": fuzz_variables,
        }
        export_results.append(export_result)

    return export_results


def complete_extracted_code(extracted_code, fuzz_init_code, target_func, matched_func_args,
                            orig_param_dict, orig_var_types, matched_func_variables,
                            existing_declared_names,
                            rewrite_names,
                            arg_indexes):
    var_types = ["identifier", "literal", "field_expression", "subscript_expression"]
    def _reconstruct_for_variables(combined_driver, node, rewrite_variables):
        if node.type == "call_expression" and node.named_children[0].text.decode() == target_func:
            string = ""
            for child in node.children:
                if child.type == "argument_list":
                    string += "("
                    params = []
                    arg_index = 0
                    for param in child.named_children:
                        if arg_index not in arg_indexes:
                            # Rewrite handled in target_func argument substitution
                            params.append(param.text.decode())
                        else:
                            params.append(_reconstruct_for_variables(combined_driver, param, rewrite_variables))
                        arg_index += 1
                    string += ", ".join(params)
                    string += ")"
                else:
                    string += _reconstruct_for_variables(combined_driver, child, rewrite_variables)
            return string
        string = ""
        if node.type in var_types:
            if node.parent.type == "call_expression":
                content = node.text.decode()
                if content in rewrite_names:
                    return rewrite_names[content]
                return node.text.decode()
            var_name = node.text.decode()
            if var_name in rewrite_names:
                return " " + rewrite_names[var_name]
            if var_name in rewrite_variables:
                return " " + rewrite_variables[var_name]
            return " " + var_name
        if node.type in ["string_content", "char_literal"]:
            return node.text.decode()
        if not node.children:
            content = node.text.decode()
            if content not in ["{", "}", ":", ";", ",", "C", '"', "'"]:
                content = " " + content
            if content in ["{", "}", ":", ";"]:
                content += "\n"
            return content
        for child in node.children:
            string += _reconstruct_for_variables(combined_driver, child, rewrite_variables)
        return string

    def _var_def_in_code(var_name, rd_table):
        for node_id, table in rd_table.items():
            if node_id < 0:
                continue
            for v in table["def"]:
                if v.name == var_name:
                    return True
        return False

    temp_code = "int LLVMFuzzerTestOneInput(uint8_t* _IN_BUFFER, size_t _IN_BUFFER_SIZE){\n"
    temp_code += fuzz_init_code
    temp_code += "\n"
    temp_code += "__EXTRACT_CODE();\n{\n"
    temp_code += extracted_code
    temp_code += "\n}\n"
    temp_code += "}\n"
    #with open("temp.cpp", "w") as f:
    #    f.write(temp_code)
    #exit(0)
    combined_driver = CombinedDriver(src_language="cpp", src_code=temp_code, output_file="",
                                     graph_format="dot", codeviews=codeviews)
    rda_table = combined_driver.results["DFG"].rda_table
    #it_print(combined_driver.results["DFG"])

    declared_names = [v.name.replace("'", "") for v in rda_table[-1]["declared"]]
    new_handled_variables = {}

    rewrite_variables = {}
    new_extract_code = ""
    for var in rda_table[-1]["def"]:
        if var.name in ["_IN_BUFFER", "_IN_BUFFER_SIZE"] + matched_func_args \
            + matched_func_variables:
            continue
        if var.name in rewrite_names:
            continue
        if var.name in orig_param_dict:
            var_type = orig_param_dict[var.name]["type"]
            if is_primitive_type(var_type):
                new_handled_variables[var.name] = {
                    "rewrite_node_id": -1,
                    "require_fuzz": True,
                    "require_declare": True,
                    "require_assign": True,
                    "type_node": orig_param_dict[var.name]["type"], 
                    "pointer_stars": orig_param_dict[var.name]["pointer_stars"],
                    "data": None
                }
            else:
                # See if the variable is initialized, if yes, declare it
                handled = True
                for node_id, table in rda_table.items():
                    if node_id < 0:
                        continue
                    search = False
                    for v in table["def"]:
                        if v.name == var.name:
                            search = True
                            break
                    if search:
                        new_extract_code += f"// initializing {var.name} from parameter\n"
                        new_extract_code += f"{var_type}{orig_param_dict[var.name]['pointer_stars']} {var.name};\n"
                        declared_names.append(var.name)
                        handled = True
                        break
                if handled:
                    continue
                msg = f"Don't know how to handle {var_type}"
                logger.error(msg)
                raise NotImplementedError(msg)
            continue
        # TODO: not sure if we should do the same as original function parameters
        if var.name in orig_var_types:
            if var.name in existing_declared_names:
                continue
            var_type = orig_var_types[var.name]["type"]
            # Simiply declare the variable
            if _var_def_in_code(var.name, rda_table):
                new_handled_variables[var.name] = {
                    "rewrite_node_id": -1,
                    "require_fuzz": False,
                    "require_declare": True,
                    "require_assign": True,
                    "type_node": orig_var_types[var.name]["type"], 
                    "pointer_stars": orig_var_types[var.name]["pointer_stars"],
                    "data": None
                }
                continue
            # Will need to fuzz and initialize the variable
            if is_primitive_type(var_type):
                new_handled_variables[var.name] = {
                    "rewrite_node_id": -1,
                    "require_fuzz": True,
                    "require_declare": True,
                    "require_assign": True,
                    "type_node": orig_var_types[var.name]["type"], 
                    "pointer_stars": orig_var_types[var.name]["pointer_stars"],
                    "data": None
                }
            else:
                msg = f"Don't know how to handle {var_type} for {var.name}"
                logger.error(msg)
                raise NotImplementedError(msg)
            continue
        if var.field_parent_name:
            if var.field_parent_name in declared_names:
                continue
        # Potentail macros, just keep
        if re.compile('[A-Z_0-9]+').fullmatch(var.name):
            continue
        if var.name == "static_cast":
            continue

        new_name = f"__{var.name.replace('.', '_').replace('->', '__').replace('[', '').replace(']', '')}" 
        rewrite_variables[var.name] = new_name
        logger.debug(f"\nRewrite {var.name} to {new_name}")

    AST_driver = combined_driver.results["AST"]
    DFG_driver = combined_driver.results["DFG"]
    pre_extract_id = search_function_call(AST_driver, "__EXTRACT_CODE",
                                            DFG_driver=DFG_driver)
    extract_node = AST_driver.orig_nodes[pre_extract_id[0]].next_sibling
    if rewrite_variables or rewrite_names:
        # TODO: Very hacky
        if rewrite_variables:
            new_extract_code += "// TODO: very hacky. Who knows whether this will work\n"
        for var in rewrite_variables:
            new_extract_code += f"int {rewrite_variables[var]} = 5;\n"
        for node in extract_node.named_children:
            new_extract_code += _reconstruct_for_variables(combined_driver, node, rewrite_variables)
    else:
        new_extract_code += extracted_code

    return new_extract_code, new_handled_variables
