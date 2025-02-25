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

export WORK=$(pwd)/build
mkdir -p $WORK

sed -i 's/-Werror//g' ./configure.ac
./bootstrap.sh

# Limit the size of buffer allocations to avoid bogus OOM issues
# https://github.com/libgd/libgd/issues/422
sed -i'' -e 's/INT_MAX/100000/' "$SRC/libgd/src/gd_security.c"

./configure --prefix="$WORK" --disable-shared
make -j$(nproc) install

#for new_f in $new_fuzzers; do
#    $CXX $CXXFLAGS -std=c++11 -I"$WORK/include" -L"$WORK/lib" \
#      $SRC/$new_f.cpp -o $OUT/$new_f \
#      $LIB_FUZZING_ENGINE -lgd -Wl,-Bstatic -lz -Wl,-Bdynamic
#done
#

###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/libgd/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS -std=c11 -I"$WORK/include" -L"$WORK/lib" \
        $cfile -o $out_path/$out_name \
        -lgd -Wl,-Bstatic -lz -Wl,-Bdynamic -lm &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -std=c11 -I"$WORK/include" -L"$WORK/lib" \
            $cfile -o $out_path/$out_name \
            -lgd -Wl,-Bstatic -lz -Wl,-Bdynamic -lm
    fi
done

true
