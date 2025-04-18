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
PREFIX=$WORK/prefix
mkdir -p $PREFIX

export PKG_CONFIG="`which pkg-config` --static"
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
export PATH=$PREFIX/bin:$PATH

BUILD=$WORK/build

rm -rf $WORK/*
rm -rf $BUILD
mkdir -p $BUILD

# Build glib
pushd $SRC/glib/
meson \
    --prefix=$PREFIX \
    --libdir=lib \
    --default-library=static \
    -Db_lundef=false \
    -Doss_fuzz=enabled \
    -Dlibmount=disabled \
    _builddir
ninja -C _builddir
ninja -C _builddir install
popd

pushd $SRC/freetype2
./autogen.sh
./configure --prefix="$PREFIX" --disable-shared PKG_CONFIG_PATH="$PKG_CONFIG_PATH"
make -j$(nproc)
make install

# Build cairo
pushd $SRC/cairo
CFLAGS="-DDEBUG_SVG_RENDER $CFLAGS" meson \
    --prefix=$PREFIX \
    --libdir=lib \
    --default-library=static \
    _builddir
ninja -C _builddir
ninja -C _builddir install
popd

#mv $SRC/{*.zip,*.dict} $OUT

#if [ ! -f "${OUT}/cairo_seed_corpus.zip" ]; then
#  echo "missing seed corpus"
#  exit 1
#fi
#
#if [ ! -f "${OUT}/cairo.dict" ]; then
#  echo "missing dictionary"
#  exit 1
#fi

PREDEPS_LDFLAGS="-Wl,-Bdynamic -ldl -lm -lc -pthread -lrt -lpthread"
DEPS="gmodule-2.0 glib-2.0 gio-2.0 gobject-2.0 freetype2 cairo cairo-gobject" 
BUILD_CFLAGS="$CFLAGS `pkg-config --static --cflags $DEPS`"
BUILD_LDFLAGS="-Wl,-static `pkg-config --static --libs $DEPS`"

fuzzers=$(find $SRC/fuzz/ -name "*_fuzzer.c")
for f in $fuzzers $SRC/cairo/test/svg/fuzzer/svg-render-fuzzer.c; do
  fuzzer_name=$(basename $f .c)
  $CC $CFLAGS $BUILD_CFLAGS \
    -c $f -o $WORK/${fuzzer_name}.o
  $CXX $CXXFLAGS \
    $WORK/${fuzzer_name}.o -o $OUT/${fuzzer_name} \
    $PREDEPS_LDFLAGS \
    $BUILD_LDFLAGS \
    $LIB_FUZZING_ENGINE \
    -Wl,-Bdynamic
  cd $OUT; ln -sf cairo_seed_corpus.zip ${fuzzer_name}_seed_corpus.zip
  cd $OUT; ln -sf cairo.dict ${fuzzer_name}.dict
done

###################################

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/cairo/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
        mkdir -p $out_path
        if [ ! "$(ls -A $path)" ]; then
            continue
        fi
        for cfile in "$path"/*.c; do
            out_name=$(basename -s .c $cfile)
            $CC $CFLAGS $BUILD_CFLAGS \
              $cfile -o $out_path/$out_name \
              $PREDEPS_LDFLAGS \
              $BUILD_LDFLAGS \
              -Wl,-Bdynamic  &> /home/log
            if [ $? -ne 0 ]; then
                $SRC/hopper_helper.py $cfile /home/log
                $CC $CFLAGS $BUILD_CFLAGS \
                  $cfile -o $out_path/$out_name \
                  $PREDEPS_LDFLAGS \
                  $BUILD_LDFLAGS \
                  -Wl,-Bdynamic 
            fi
        done
    done
done

true
