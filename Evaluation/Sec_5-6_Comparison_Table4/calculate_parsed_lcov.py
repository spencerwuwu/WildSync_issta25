#!/usr/bin/env python3
import numpy as np
import json
import os
from prettytable import PrettyTable

rc = "./inforc"
if not os.path.exists(rc):
    print("inforc file not found")
    exit(1)
with open(rc, "r") as f:
    exec(f.read())

def lcov_json_to_sets(json_file):
    def collapse_dict(data):
        ret = set()
        for key, values in data.items():
            for value in values:
                ret.add(f"{key}::{value}")
        return ret
    with open(json_file, 'r') as fd:
        data = json.load(fd)
    s_info = data["source_coverage"]
    f_info = data["function_coverage"]
    return collapse_dict(s_info), collapse_dict(f_info)


lcov_dir = "lcov_parsed"

if not len(targets):
    print("Remember to set `inforc`")
    exit(1)

myTable = PrettyTable(["<library>", "line coverage", "function coverage"])
for fuzzer in ["hopper"]:
    print(fuzzer)
    for proj in targets:
        lcov = 0
        fcov = 0
        for round in range(5):
            msg = f"{proj} {round} "
            f_name = f"{lcov_dir}/{fuzzer}_{proj}_{round}.json"
            s_info, f_info = lcov_json_to_sets(f_name)
            lcov += len(s_info)
            fcov += len(f_info)
        lcov = lcov / 5
        fcov = fcov / 5
        myTable.add_row([proj, lcov, fcov])
        
print(myTable)
