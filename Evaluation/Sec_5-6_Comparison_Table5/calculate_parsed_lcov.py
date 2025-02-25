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

filename_sets = {
    "OSS": "orig",
    "new": "new"
}

lcov_dir = "lcov_parsed"

if not len(targets):
    print("Remember to set `inforc`")
    exit(1)

#print("<library> Line_Coverage ")
myTable = PrettyTable(["<library>", "Line_Coverage"])
for fuzzer in ["libfuzzer"]:
    for proj in targets:
        #msg = f"{proj} "
        oss_lcov = 0
        oss_fcov = 0
        new_fcov = 0
        new_lcov = 0
        for round in range(5):
            s_infos = {}
            f_infos = {}
            for key, fname in filename_sets.items():
                f_name = f"{lcov_dir}/{fuzzer}_{proj}_{fname}_{round}.json"
                s_info, f_info = lcov_json_to_sets(f_name)
                s_infos[key] = s_info
                f_infos[key] = f_info
            s_diff = [e for e in s_infos["new"] if e not in s_infos["OSS"]]
            f_diff = [e for e in f_infos["new"] if e not in f_infos["OSS"]]
            oss_lcov += len(s_infos['OSS']) 
            new_lcov += len(s_diff)
            oss_fcov += len(f_infos['OSS']) 
            new_fcov += len(f_diff)
        all_lcov = (oss_lcov + new_lcov)/5
        all_fcov = (oss_fcov + new_fcov)/5
        line_imp = "{:.2f}".format(new_lcov/oss_lcov*100)
        func_imp = "{:.2f}".format(new_fcov/oss_fcov*100)
        myTable.add_row([target, int(all_lcov)])
        #msg += f"{int(all_lcov)}"
        #print(msg)
