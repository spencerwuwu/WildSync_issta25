import json
from loguru import logger
from comex.codeviews.SDFG.SDFG_csharp import recursively_get_children_of_types

########################
###  Indexs:
###   - For debug prints
###   - For exports
########################

def recursively_get_self_n_children_of_types(
    node, st_types, check_list=None, index=None, result=None, stop_types=None
):
    if node.type in st_types:
        return [node]
    else:
        return recursively_get_children_of_types(node, st_types, check_list, index, result, stop_types)

########################
#  For debug prints
########################
def read_index(index, idx):
    return list(index.keys())[list(index.values()).index(idx)]
def it_print(DFG_driver):
    index = DFG_driver.parser.index
    rda_result = DFG_driver.rda_table
    table = []
    for key, value in rda_result.items():
        try:
            table.append("Lineno: " + str(read_index(index, key)[0][0] + 1) + "\n")
        except:
            table.append("*Index: " + str(key) + "\n")
        for key2, value2 in value.items():
            table.append(f"\t{key2}:")
            for entry in value2:
                table.append(str(entry) + ",")
            table.append("\n")
    logger.debug("\n" + "".join(table))


def print_backtrace(src_code, arg, content):
    source_node = content["source_node"]
    marked_nodes = content["marked_nodes"]
    related_nodes = content["related_nodes"]
    critical_nodes = content["critical_nodes"]
    passby_nodes = content["passby_nodes"]

    print_content = [f"Line {source_node['lineno']} - {arg}"]
    src_lines = src_code.splitlines()
    marked_lines =  [ele['lineno'] for ele in marked_nodes]
    related_lines = [ele['lineno'] for ele in related_nodes]
    critical_lines = [ele['lineno'] for ele in critical_nodes]
    passby_lines = [ele['lineno'] for ele in passby_nodes]
    for i in range(len(src_lines)):
        line = ""
        if i + 1 == source_node['lineno']:
            if critical_lines:
                line = "$$$$"
            else:
                line = "$$"
        else:
            if i + 1 in marked_lines:
                line += "++"
            elif i + 1 in related_lines:
                line += " -"
            else:
                line += "  "

            if critical_lines:
                if i + 1 in critical_lines:
                    line += "**"
                elif i + 1 in passby_lines:
                    line += " -"
                else:
                    line += "  "

        line += f"{i + 1:5d} {src_lines[i]}"
        print_content.append(line)
    logger.debug("\n" + "\n".join(print_content))


########################
#  For exports
########################
def export_extracted_code(combined_driver, 
                          extracted_node_ids, 
                          marked_node_ids,
                          rewrite_statements
                          ):
    cfg = combined_driver.results["CFG"].CFG.graph
    ast = combined_driver.results["AST"]

    def _get_all_children(node):
        if node is None:
            return []
        all_children = []
        for child in node.named_children:
            child_id = ast.parser.index[(child.start_point, child.end_point, child.type)]
            all_children.append(child_id)
            all_children += _get_all_children(child)
        all_children.append(ast.parser.index[(node.start_point, node.end_point, node.type)])
        return all_children

    def _node_has_all_extracted(all_children):
        for eid in extracted_node_ids:
            if eid >= 0 and  eid not in all_children:
                return False
        return True

    first_node_id = min([e for e in extracted_node_ids if e >= 0])
    first_node = ast.orig_nodes[first_node_id]
    all_children = _get_all_children(first_node)
    while not _node_has_all_extracted(all_children):
        if not first_node.parent:
            break
        first_node = first_node.parent
        all_children = _get_all_children(first_node)

    def _has_extracted_code(node):
        node_id = ast.parser.index[(node.start_point, node.end_point, node.type)]
        if node_id in extracted_node_ids:
            return True
        for child in node.named_children:
            if _has_extracted_code(child):
                return True
        return False

    def _reconstruct_with_select(node, extracted_node_ids, rewrite_statements):
        string = ""
        node_id = ast.parser.index[(node.start_point, node.end_point, node.type)]
        if node_id in extracted_node_ids:
            if node_id in rewrite_statements:
                # HACK
                if node.type == "if_statement":
                    content = f"if ({rewrite_statements[node_id]})"
                elif node.type == "while_statement":
                    content = f"while ({rewrite_statements[node_id]})"
                # HACK: For is also the same
                else:
                    content = rewrite_statements[node_id]
            else:
                content = cfg.nodes[node_id]["content"]
            if node.type in ["if_statement"]:
                has_child = False
                has_else = False
                for cc in node.named_children:
                    if cc.type == "else_clause":
                        has_else = _has_extracted_code(cc)
                    elif _has_extracted_code(cc):
                        has_child = True
                        break
                if not node_id in extracted_node_ids and not has_child and not has_else:
                    return ""
                if not has_child:
                    content += "{}\n"
                    return content
            elif node.type in ["while_statement", "for_statement"]:
                inner = node.named_children[-1]
                if not _has_extracted_code(inner):
                    content += "{}\n"
                    return content
            string += content + "\n"
        for child in node.named_children:
            if child.type == "else_clause":
                if _has_extracted_code(child):
                    string += " else "
                    string += _reconstruct_with_select(child, extracted_node_ids, rewrite_statements)
            elif child.type == "case_statement":
                has_child = _has_extracted_code(child)
                if not has_child:
                    continue
                label = child.text.decode("UTF-8").rsplit(":", 1)[0].strip()
                string += f" {label}: {{\n"
                string += _reconstruct_with_select(child.named_children[-1], extracted_node_ids, rewrite_statements)
                string += "}\n"
            elif child.type == "compound_statement":
                has_child = _has_extracted_code(child)
                if child.parent.type in ["case_statement", "if_statement"] and not has_child:
                    continue
                string += " {\n"
                if has_child:
                    for cc in child.named_children:
                        string += _reconstruct_with_select(cc, extracted_node_ids, rewrite_statements)
                string += " }\n"
            else:
                string += _reconstruct_with_select(child, extracted_node_ids, rewrite_statements) 
        return string
    code =  _reconstruct_with_select(first_node, extracted_node_ids, rewrite_statements)
    print(code)
    # HACK: remove "return" statement if it only occurs once
    if code.count("return ") > 1:
        msg = "Multiple return statements found in extracted code"
        logger.error(msg)
        raise NotImplementedError(msg)
    elif code.count("return ") == 1:
        code = code.replace("return ", "")
    logger.debug("\nExtracted code:\n" + code)
    return code
