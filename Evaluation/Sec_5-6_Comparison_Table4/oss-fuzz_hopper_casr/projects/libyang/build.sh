#!/bin/bash -eu
# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################


mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=OFF -DENABLE_TOOLS=OFF ..
make

static_pcre=($(find /home/pcre2 -name "libpcre2-8.a"))

#for fuzzer in lyd_parse_mem_json lyd_parse_mem_xml lys_parse_mem; do
#  $CC $CFLAGS -c ../tests/fuzz/${fuzzer}.c -I./libyang -I./compat
#  $CXX $CXXFLAGS $LIB_FUZZING_ENGINE ${fuzzer}.o -o $OUT/${fuzzer} \
#    ./libyang.a ${static_pcre}
#done
#
#
#for new_f in $new_fuzzers; do
#  cp $SRC/new_fuzzers/${new_f}.cpp ../tests/fuzz/${new_f}.c
#  $CC $CFLAGS -c ../tests/fuzz/${new_f}.c -I./libyang -I./compat
#  $CXX $CXXFLAGS $LIB_FUZZING_ENGINE ${new_f}.o -o $OUT/${new_f} \
#    ./libyang.a ${static_pcre}
#done

###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/libyang/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS -I./libyang -I./compat \
        $cfile -o $out_path/$out_name \
        ./libyang.a ${static_pcre} &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -I./libyang -I./compat \
            $cfile -o $out_path/$out_name \
            ./libyang.a ${static_pcre}
    fi
done

true
