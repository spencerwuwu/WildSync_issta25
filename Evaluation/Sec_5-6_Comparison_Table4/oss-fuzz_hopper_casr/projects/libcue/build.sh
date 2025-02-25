#!/bin/bash -eu
# Copyright 2023 Google LLC
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

rm -rf build && mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release ../

make 

###################################
cd ../

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/libcue/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS -I. -I./build\
        $cfile -o $out_path/$out_name \
        ./build/libcue.a &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -I. -I./build\
            $cfile -o $out_path/$out_name \
            ./build/libcue.a
    fi
done

true
