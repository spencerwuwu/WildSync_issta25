#!/usr/bin/env python3
import re
import os
from subprocess import Popen, PIPE
import sys
import json

log_dir = f"{sys.argv[1]}/log"

all_targets = [
        "cairo",
        "cjson",
        "fribidi",
        "gdk-pixbuf",
        "json-c",
        "krb5",
        "lcms",
        "leptonica",
        "libarchive",
        "libass",
        "libgd",
        "libheif",
        "libpcap",
        "libplacebo",
        "libpng",
        "libsoup3",
        "libtiff",
        "libvnc",
        "libyang",
        "lmdb",
        "mxml",
        "vorbis",
        "yyjson",
        "zlib",
        "libcue",
        "libharu",
        "libxmlb",
]

def run_cmd(cmd, to_p=False):
    p = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
    out, err = p.communicate()
    if p.returncode != 0:
        if to_p:
            print(err.decode())
        return None
    return out.decode("utf-8")

targets = run_cmd(f"ls -hl {log_dir}", to_p=True)
if not targets:
    exit(1)

all_results = {}

targets = targets.split("\n")
for log in targets:
    if ".log" not in log:
        continue
    if "_build.log" in log:
        continue
    log_file = log.split()[-1]
    log_file = f"{log_dir}/{log_file}"
    fname = log_file.split("/libfuzzer_")[-1].rsplit("_",2)[0] + ".cpp"
    libname, fname = fname.split("_", 1)
    try:
        lines = run_cmd(f"cat {log_file} | tail -n 400")
        if not lines:
            continue
        match = re.search(r"Done (\d+) runs in (\d+) second", lines)
        if not match:
            if libname not in all_results:
                all_results[libname] = {
                        "timeout": {},
                        "oom": {},
                        "crash": {}
                        }
            if "SUMMARY: libFuzzer: out-of-memory" in lines:
                #print("OOM " + log_file)
                all_results[libname]["oom"][fname] = log_file
            elif "SUMMARY: libFuzzer: timeout" in lines:
                #print("timeout " + log_file)
                all_results[libname]["timeout"][fname] = log_file
            else:
                #print(log_file)
                all_results[libname]["crash"][fname] = log_file
            continue
    except:
        lines = run_cmd(f"cat {log_file} | tail -n 400 | grep timeout")
        if libname not in all_results:
            all_results[libname] = {
                    "timeout": {},
                    "oom": {},
                    "crash": {}
                    }

        if lines:
            #print("timeout " + log_file)
            all_results[libname]["timeout"][fname] = log_file
        else:
            lines = run_cmd(f"cat {log_file} | tail -n 400 | grep 'out-of-memory'")
            if lines:
                #print("OOM " + log_file)
                all_results[libname]["oom"][fname] = log_file
            else:
                #print(log_file)
                all_results[libname]["crash"][fname] = log_file

for target in all_targets:
    if target in all_results:
        print(target)
        print(f"-- Crashes (potential): {len(all_results[target]['crash'])}")
        print(f"-- OOM/Timeout: {len(all_results[target]['timeout']) + len(all_results[target]['oom'])}")
        print("-"*10)
print("="*20)
f_out = "crash-list.json"
print("Storing crash indexes to " + f_out)

with open(f_out, "w") as fd:
    json.dump(all_results, fd)


