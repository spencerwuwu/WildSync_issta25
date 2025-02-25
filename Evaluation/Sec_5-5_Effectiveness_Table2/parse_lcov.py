#!/usr/bin/env python3
import sys
import os
import json
from subprocess import Popen, PIPE

out_dir = "lcov_parsed"

proj_src = {
    "cjson": "/src/cjson/",
    "libcue": "/src/libcue/",
    "libpcap": "/src/libpcap",
    "libxmlb": "/src/libxmlb/src",
    "gmime3": "/src/gmime/gmime/",
    "libharu": "/src/libharu/src/",
    "yyjson": "/src/yyjson/src/",
    "mxml": "/src/mxml/",
    "libyang": "/src/libyang/src/",
    "krb5": "/src/krb5/src/",
    "lmdb": "/src/lmdb/libraries/liblmdb/",
    "libspectre": "/src/libspectre/libspectre",
    "yyjson": "/src/yyjson/src/",
    "zlib": "/src/zlib/",
    "libsoup3": "/src/libsoup/libsoup/",
    "libplacebo": "/src/libplacebo/src",
    "lcms": "/src/lcms/src",
    "cairo": "src/cairo/src",
    "libtiff": "/src/libtiff/libtiff/",
    "json-c": "/src/json-c/",
    "gdk-pixbuf": "/src/gdk-pixbuf/",
    "leptonica" : "/src/leptonica/src/",
    "libheif": "/src/libheif/libheif/",
    "libgd": "/src/libgd/", 
    "vorbis": "/src/vorbis/lib/",
    "mpg123": "/src/mpg123/src",
    "libass": "/src/libass/libass/",
    "libwebp": "/src/libwebp/src/",
    "fribidi": "/src/fribidi/lib/",
    "speex": "/src/speex/libspeex/",
    "libvnc": "src/libvncserver/src",
    "lcms": "/src/lcms/src/",
    "libpng": "/src/libpng/",
    "libarchive": "src/libarchive/libarchive/",
    "libevent": "/src/libevent"
}


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

        if "Magick++/fuzz/" in filename:
            continue

        if "libpng" in lcov_file and "contrib" in filename:
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



def main(workdir, proj_src):
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    groups = ["orig", "new"]
    path = f"{workdir}/coverage_ar"
    for s in os.listdir(path):
        storage = f"{path}/{s}"
        print(f"Processing {storage}")
        out_file = f"{out_dir}/{s}.json"
        proj = s.split("_")[1]
        ret = run_cmd(f"find {storage} -name '*.lcov'", to_p=True)
        if ret is None:
            continue
        lcov_files = []
        for line in ret.split("\n"):
            if line.strip() == "":
                continue
            if "report/linux/" in line:
                continue
            lcov_files.append(line.strip())
        
        source_coverages = {} 
        function_coverages = {}
        for lcov_file in lcov_files:
            source_coverages, function_coverages = process_lcov(lcov_file, 
                                                                target_dir=proj_src[proj], 
                                                                source_coverages=source_coverages, 
                                                                function_coverages=function_coverages)
        data = {"source_coverage": source_coverages, "function_coverage": function_coverages}

        with open(out_file, "w") as fd:
            json.dump(data, fd, indent=2)



if __name__ == "__main__":
    WORKDIR = sys.argv[1]
    if not os.path.exists(WORKDIR):
        print("Path not found", WORKDIR)
        exit(1)
    main(WORKDIR, proj_src)

