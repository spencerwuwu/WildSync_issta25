#!/usr/bin/env python3 

import os, sys
import subprocess
from elftools.elf.elffile import ELFFile
from elftools.elf.segments import Segment
from multiprocessing import cpu_count, Pool
import signal
import lief
import shutil
import re
import random, string
from functools import partial
import pathlib

bounds_dict = dict()

def random_string(size=6, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))

def handle_sigint(sig, frame):
    print('\n[*] Ctrl+C pressed! Stopping processing.')
    sys.exit(0)
    return

#--------------------------------------------------------------------
# Read the CASR-generated reports into a set of bugs + source lines. 
# Merge both crash sets based on uniqueness of crashing source line.
#--------------------------------------------------------------------
def get_crash_set(results_dir):
    with open("%s/casr/report.txt" % results_dir, "r") as crash_data:
        matches_gdb = set([(x, os.path.basename(y)) for (x,y) \
            in re.findall(r'^\s*\w*.gdb.casrep\W*\w*:\s([\w\-\(\)]*):\s([\/\w\-\.]*[.c.h.cpp.cc.hpp]\:[\w]*)', \
            crash_data.read(), re.MULTILINE)])
    with open("%s/casr/report.txt" % results_dir, "r") as crash_data:
        matches_san = set([(x, os.path.basename(y)) for (x,y) \
            in re.findall(r'^\s*\w*\s*casrep\W*\w*:\s([\w\-\(\)]*):\s([\/\w\-\.]*[.c.h.cpp.cc.hpp]\:[\w]*)', \
            crash_data.read(), re.MULTILINE)])
    return matches_san.union([(x,y) for (x,y) \
        in matches_gdb if y not in [b for (a,b) in matches_san]])

#--------------------------------------------------------------------
# Helper function to create CASR directories for ASAN / GDB reports.
#--------------------------------------------------------------------
def setup_casr_dirs(results_dir):
    casr_dir       = "%s/casr/" % results_dir
    casr_raw_dir   = "%s/casr/raw" % results_dir
    casr_post_dir  = "%s/casr/post" % results_dir

    try: shutil.rmtree(casr_dir)
    except: pass

    os.makedirs(casr_dir, exist_ok=True)
    os.makedirs(casr_raw_dir, exist_ok=True)
    os.makedirs(casr_post_dir, exist_ok=True)
    return

#--------------------------------------------------------------------
# Helper function to run CASR's deduplication on ASAN / GDB reports.
#--------------------------------------------------------------------
def run_casr_dedup(results_dir):
    casr_dir       = "%s/casr/" % results_dir
    casr_raw_dir   = "%s/casr/raw" % results_dir
    casr_post_dir  = "%s/casr/post" % results_dir

    casr_dedup_cmd = "casr-cluster -d %s %s" \
        % (casr_raw_dir, casr_post_dir) 

    if (os.getenv("DEBUG")):
        subprocess.call(casr_dedup_cmd, shell=True)  
    else:
        subprocess.call(casr_dedup_cmd, shell=True, \
            stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL)

    casr_report_cmd = "casr-cli %s" % (casr_raw_dir) 

    with open("%s/report.txt" % casr_dir, "w") as f:
        subprocess.call(casr_report_cmd, shell=True, \
            stderr=subprocess.DEVNULL, stdout=f)
    return

#--------------------------------------------------------------------
# Helper function to run CASR's ASAN / GDB crash report generation.
#--------------------------------------------------------------------
def run_casr(results_dir, cmd):
    casr_raw_dir = "%s/casr/raw" % results_dir

    tid = re.search(r'^([0-9]_)(?:id_)([0-9]{6})|^([0-9]_)(?:id:)([0-9]{6})', \
        os.path.basename(cmd)).group()
    
    casr_gdb_cmd = "casr-gdb -o %s/%s.gdb.casrep -- %s" \
        % (casr_raw_dir, tid, cmd)

    if (os.getenv("DEBUG")):
        subprocess.call(casr_gdb_cmd, shell=True)  
    else:
        subprocess.call(casr_gdb_cmd, shell=True, \
            stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL)

    casr_san_cmd = "casr-san -o %s/%s.san.casrep -- %s" \
        % (casr_raw_dir, tid, cmd)

    if (os.getenv("DEBUG")):
        subprocess.call(casr_san_cmd, shell=True)  
    else:
        subprocess.call(casr_san_cmd, shell=True, \
            stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL)
    return


#--------------------------------------------------------------------
# Retrieve Hopper crashes by translating and re-running all inputs.
#--------------------------------------------------------------------
def process_hopper(proc_dir, result_dir):
    setup_casr_dirs(result_dir)
    bugs_set = set()
    if cpu_count() > 20:
        ncpu = 20
    else:
        ncpu = cpu_count()
    #cpu_pool = Pool(cpu_count())
    cpu_pool = Pool(ncpu)

    crash_pocs  = ["%s/%s" % (proc_dir,x) \
                        for x in os.listdir(proc_dir)]
    try: 
        assert(len(crash_pocs) > 0)
    except: 
        print("[*] ERROR: all Hopper crashes fail")
        return

    cpu_pool.map(partial(run_casr, result_dir), crash_pocs)
    run_casr_dedup(result_dir)
    bugs_set = bugs_set.union(get_crash_set(result_dir)) 
    return bugs_set


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 hopper_help.py <proc_dir> <result_dir>")
        sys.exit(1)

proc_dir = sys.argv[1]
result_dir = sys.argv[2]
ret = process_hopper(proc_dir, result_dir)

with open(f"{result_dir}/set.txt", "w") as fd:
    for data in ret:
        fd.write(f"{data}\n")
