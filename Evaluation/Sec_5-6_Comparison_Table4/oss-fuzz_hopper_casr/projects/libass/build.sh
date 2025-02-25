#!/bin/bash -eux
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

export WORK=/home/work
mkdir -p $WORK

cd $SRC/harfbuzz

# setup
build=$WORK/build

# cleanup
rm -rf $build
mkdir -p $build

# disable sanitize=vptr for harfbuzz since it compiles without rtti
CFLAGS="$CFLAGS -fno-sanitize=vptr" \
CXXFLAGS="$CXXFLAGS -fno-sanitize=vptr" \
meson --default-library=static --wrap-mode=nodownload \
      -Dfuzzer_ldflags="$(echo $LIB_FUZZING_ENGINE)" \
      -Dtests=disabled \
      --prefix=/work/ --libdir=lib $build \
  || (cat build/meson-logs/meson-log.txt && false)
meson install -C $build

cd $SRC/libass

export PKG_CONFIG_PATH=/work/lib/pkgconfig
./autogen.sh
./configure FUZZ_CPPFLAGS="-DASS_FUZZMODE=2" --disable-asm --disable-shared #--enable-fuzz
#make -j "$(nproc)" fuzz/fuzz_ossfuzz
make -j "$(nproc)" 


###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/libass/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS  \
      $cfile -c -o /tmp/$out_name &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS \
          $cfile -c -o /tmp/$out_name 
    fi
    if [ ! -f /tmp/$out_name ]; then
        continue
    fi
    $CXX $CXXFLAGS -std=c++11 \
        /tmp/$out_name \
        -o $out_path/$out_name \
        libass/.libs/libass.a \
        -Wl,-Bstatic \
        $(pkg-config --static --libs ./libass.pc | sed -e 's/-lm //g' -e 's/-lass //g') \
        -Wl,-Bdynamic
done

true
