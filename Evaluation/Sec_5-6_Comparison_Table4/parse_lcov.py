#!/usr/bin/env python3
import os
import sys

from subprocess import Popen, PIPE

import json
from subprocess import Popen, PIPE


out_dir = "lcov_parsed"

proj_src = {
    "cjson":      "/home/cjson/",
    "libcue":     "/home/libcue/",
    "libpcap":    "/home/libpcap",
    "libxmlb":    "/home/libxmlb/src",
    "gmime3":     "/home/gmime/gmime/",
    "libharu":    "/home/libharu/src/",
    "yyjson":     "/home/yyjson/src/",
    "mxml":       "/home/mxml/",
    "libyang":    "/home/libyang/src/",
    "krb5":       "/home/krb5/src/",
    "lmdb":       "/home/lmdb/libraries/liblmdb/",
    "yyjson":     "/home/yyjson/src/",
    "zlib":       "/home/zlib/",
    "libsoup3":   "/home/libsoup/libsoup/",
    "libplacebo": "/home/libplacebo/src",
    "lcms":       "/home/lcms/src",
    "cairo":      "/home/cairo/src",
    "libtiff":    "/home/libtiff/libtiff/",
    "json-c":     "/home/json-c/",
    "gdk-pixbuf": "/home/gdk-pixbuf/",
    "leptonica" : "/home/leptonica/src/",
    "libheif":    "/home/libheif/libheif/",
    "libgd":      "/home/libgd/", 
    "vorbis":     "/home/vorbis/lib/",
    "mpg123":     "/home/mpg123/src",
    "libass":     "/home/libass/libass/",
    "libwebp":    "/home/libwebp/src/",
    "fribidi":    "/home/fribidi/lib/",
    "speex":      "/home/speex/libspeex/",
    "libvnc":     "/home/libvncserver/src",
    "lcms":       "/home/lcms/src/",
    "libpng":     "/home/libpng/",
    "libarchive": "/home/libarchive/libarchive/",
    "libevent":   "/home/libevent"
}

#targets = [
#    #"lcms",
#    #"libplacebo",
#    #"zlib",
#    #"cjson",
#    "libsoup3",
#    #"cairo",        
#    #"fribidi",      
#    #"gdk-pixbuf",   
#    #"json-c",       
#    #"leptonica",
#    #"libass",   
#    #"libgd",    
#    #"libheif",  
#    #"libtiff",
#    #"libxml2",
#    #"vorbis",
#    #"libpng",
#    #"libvnc",
#    #"libarchive",
#]


def run_cmd(cmd, to_p=False):
    p = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
    out, err = p.communicate()
    if p.returncode != 0:
        if to_p:
            print(err.decode())
        return None
    return out.decode("utf-8")


def simplify_filename(filename):
    items = filename.split("/")
    dirs = []
    for item in items:
        if item == ".":
            continue
        elif item == "..":
            dirs.pop()
        else:
            dirs.append(item)
    return "/".join(dirs)


def process_lcov(lcov_file, target_dir="",
                 source_coverages={},
                 function_coverages={}):
    with open(lcov_file, "r") as fd:
        lines = fd.readlines()

    data_chunks = []
    # Split data into chunks first
    chunk = []
    for line in lines:
        if line.startswith("end_of_record"):
            data_chunks.append(chunk)
            chunk = []
        else:
            chunk.append(line)

    if len(chunk) > 0:
        print("ERROR: file not ended with 'end_of_record'")
        exit(1)

    for chunk in data_chunks:
        if not chunk[0].startswith("SF:"):
            print("Chunk not started with SF", chunk[0])
            exit(1)
        filename = simplify_filename(chunk[0].replace("SF:", "").strip())
        if target_dir and target_dir not in filename:
            continue

        if "hopper_results" in filename:
            continue

        if "Magick++/fuzz/" in filename:
            continue

        if "libpng" in lcov_file and "contrib" in filename:
            continue

        if "libcue" in lcov_file and "oss-fuzz" in filename:
            continue

        if "cjson" in lcov_file and "fuzzing" in filename:
            continue

        if filename not in source_coverages:
            source_coverages[filename] = []
        if filename not in function_coverages:
            function_coverages[filename] = []
        for line in chunk:
            if line.startswith("DA:"):
                lineno = int(line.replace("DA:", "").split(",")[0])
                hit_count = int(line.replace("DA:", "").split(",")[-1].strip())
                if hit_count == 0:
                    continue
                if lineno not in source_coverages[filename]:
                    source_coverages[filename].append(lineno)
            elif line.startswith("FNDA:"):
                hit_count = int(line.replace("FNDA:", "").split(",")[0])
                if hit_count == 0:
                    continue
                funcname = line.replace("FN:", "").split(",")[-1].strip()
                if funcname not in function_coverages[filename]:
                    function_coverages[filename].append(funcname)

    return source_coverages, function_coverages



def main(proj_src):
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    #for workdir in os.listdir("uncompressed"):
    workdir = sys.argv[1]
    path = f"{workdir}/coverage_ar"
    for s in os.listdir(path):
        storage = f"{path}/{s}"
        print(f"Processing {storage}")
        out_file = f"{out_dir}/{s}.json"
        proj = s.split("_")[1]
        lcov_file = f"{storage}/report/linux/summary.lcov"
        
        source_coverages = {} 
        function_coverages = {}
        source_coverages, function_coverages = process_lcov(lcov_file, 
                                                            target_dir=proj_src[proj], 
                                                            source_coverages=source_coverages, 
                                                            function_coverages=function_coverages)
        #lcov_file = f"{storage}/build/out/{proj}/report/linux/summary.lcov"
        #source_coverage, function_coverages = process_lcov(lcov_file, target_dir=target_dir)
        data = {"source_coverage": source_coverages, "function_coverage": function_coverages}

        with open(out_file, "w") as fd:
            json.dump(data, fd, indent=2)


"""
for tar in os.listdir("tarballs"):
    cmd = f"tar -xzvf tarballs/{tar}  --wildcards '*/coverage_ar/'  > /dev/null"
    print(cmd)
    run_cmd(cmd)
    run_cmd("mv *_workdir uncompressed/")
"""


main(proj_src)

