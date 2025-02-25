#!/bin/bash -eu
# Copyright 2018 Google Inc.
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
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=OFF ..
make -j$(nproc)
cd ..

#for f in $SRC/*_fuzzer.cc; do
#    fuzzer=$(basename "$f" _fuzzer.cc)
#    $CXX $CXXFLAGS -std=c++11 -I$SRC/json-c -I$SRC/json-c/json-c-build\
#         $SRC/${fuzzer}_fuzzer.cc -o $OUT/${fuzzer}_fuzzer \
#         $LIB_FUZZING_ENGINE $SRC/json-c/json-c-build/libjson-c.a
#done


###################################

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/json-c/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
        mkdir -p $out_path
        if [ ! "$(ls -A $path)" ]; then
            continue
        fi
        for cfile in "$path"/*.c; do
            out_name=$(basename -s .c $cfile)

            $CC $CFLAGS -std=c11 -I/home/json-c -I/home/json-c/build \
                 $cfile -o $out_path/$out_name \
                 /home/json-c/build/libjson-c.a &> /home/log
            if [ $? -ne 0 ]; then
                $SRC/hopper_helper.py $cfile /home/log
                $CC $CFLAGS -std=c11 -I/home/json-c -I/home/json-c/build \
                     $cfile -o $out_path/$out_name \
                     /home/json-c/build/libjson-c.a
            fi
        done
    done
done

true
