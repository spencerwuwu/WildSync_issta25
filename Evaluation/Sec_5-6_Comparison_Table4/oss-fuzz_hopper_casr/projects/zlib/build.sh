#!/bin/bash -eu
# Copyright 2016 Google Inc.
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

./configure
make -j$(nproc) clean
make -j$(nproc) all
make -j$(nproc) check

# original code
#for f in $(find $SRC -name '*_fuzzer.cc'); do
#    b=$(basename -s .cc $f)
#    $CXX $CXXFLAGS -std=c++11 -I. $f -o $OUT/$b $LIB_FUZZING_ENGINE ./libz.a
#done

###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/zlib/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS -std=c11 -I. \
        $cfile -o $out_path/$out_name ./libz.a &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -std=c11 -I. \
            $cfile -o $out_path/$out_name ./libz.a
    fi
done

true
