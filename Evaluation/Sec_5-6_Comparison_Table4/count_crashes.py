#!/usr/bin/env python3

import os
import sys
from prettytable import PrettyTable

workdir = sys.argv[1]

rc = "./inforc"
if not os.path.exists(rc):
    print("inforc file not found")
    exit(1)
with open(rc, "r") as f:
    exec(f.read())

myTable = PrettyTable(["<library>", "total_crashes", "sanitized_crashes", "unique_crashes"])

if not len(targets):
    print("Remember to set `inforc`")
    exit(1)

for target in targets:
    proj = f"hopper_{target}"
    if not os.path.exists(f"{workdir}/trans_ar/{proj}/"):
        continue
    v_c = len(os.listdir(f"{workdir}/trans_ar/{proj}/violated_crashes"))
    n_c = len(os.listdir(f"{workdir}/trans_ar/{proj}/crashes"))
    set_f = f"{workdir}/casr_ar/{target}/set.txt"
    if os.path.exists(set_f):
        with open(set_f, "r") as fd:
            uc = len(list(fd.readlines()))
    else:
        uc = 0
    myTable.add_row([target, v_c + n_c, n_c, uc])

print(myTable)

