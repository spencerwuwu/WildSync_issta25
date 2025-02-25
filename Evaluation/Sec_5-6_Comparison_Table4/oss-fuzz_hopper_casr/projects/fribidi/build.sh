#!/bin/bash -eu
# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################

export WORK=$(pwd)

# setup
build=$WORK/build

# cleanup
rm -rf $build
mkdir -p $build


# Build the library.
meson --default-library=static --wrap-mode=nodownload -Ddocs=false \
      $build \
  || (cat build/meson-logs/meson-log.txt && false)

# Build the fuzzers.
ninja -v -j$(nproc) -C $build 

# Build the new fuzzers

#for new_f in $new_fuzzers; do
#  ninja -v -j$(nproc) -C $build new_fuzzers/$new_f
#  mv $build/new_fuzzers/$new_f $OUT/
#done


###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

export BASE=/home/fribidi/
export LIB_PATH="-I${BASE}/lib -I${BASE}/build -I${BASE}/build/lib -I${BASE}/build/gen.tab/ -I${BASE}/gen.tab/"


set +e
path="$SRC/fribidi/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    cp $cfile $OUTSRC
    out_name=$(basename -s .c $cfile)

    $CC $CFLAGS -std=c11 \
         $LIB_PATH  \
         $cfile -o $out_path/$out_name \
         $build/lib/libfribidi.a &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -std=c11 \
             $LIB_PATH  \
             $cfile -o $out_path/$out_name \
             $build/lib/libfribidi.a 
    fi
done

true
