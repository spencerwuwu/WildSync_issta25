#!/bin/bash -eu
# Copyright 2019 Google Inc.
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

# Build dependencies.
export DEPS_PATH=$SRC/deps
mkdir -p $DEPS_PATH

cd $SRC/x265/build/linux
cmake -G "Unix Makefiles" \
    -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX \
    -DCMAKE_C_FLAGS="$CFLAGS" -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCMAKE_INSTALL_PREFIX="$DEPS_PATH" \
    -DENABLE_SHARED:bool=off \
    ../../source
make clean
make -j$(nproc) x265-static
make install

cd $SRC/libde265
./autogen.sh
./configure \
    --prefix="$DEPS_PATH" \
    --disable-shared \
    --enable-static \
    --disable-dec265 \
    --disable-sherlock265 \
    --disable-hdrcopy \
    --disable-enc265 \
    --disable-acceleration_speed
make clean
make -j$(nproc)
make install

mkdir -p $SRC/aom/build/linux
cd $SRC/aom/build/linux
cmake -G "Unix Makefiles" \
  -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX \
  -DCMAKE_C_FLAGS="$CFLAGS" -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
  -DCMAKE_INSTALL_PREFIX="$DEPS_PATH" \
  -DENABLE_SHARED:bool=off -DCONFIG_PIC=1 \
  -DENABLE_EXAMPLES=0 -DENABLE_DOCS=0 -DENABLE_TESTS=0 \
  -DCONFIG_SIZE_LIMIT=1 \
  -DDECODE_HEIGHT_LIMIT=12288 -DDECODE_WIDTH_LIMIT=12288 \
  -DDO_RANGE_CHECK_CLAMP=1 \
  -DAOM_MAX_ALLOCABLE_MEMORY=536870912 \
  -DAOM_TARGET_CPU=generic \
  ../../
make clean
make -j$(nproc)
make install

# Remove shared libraries to avoid accidental linking against them.
rm -f $DEPS_PATH/lib/*.so
rm -f $DEPS_PATH/lib/*.so.*

cd $SRC/libheif
mkdir build
cd build
cmake .. --preset=fuzzing \
      -DFUZZING_COMPILE_OPTIONS="" \
      -DFUZZING_LINKER_OPTIONS="$LIB_FUZZING_ENGINE" \
      -DFUZZING_C_COMPILER=$CC -DFUZZING_CXX_COMPILER=$CXX \
      -DWITH_DEFLATE_HEADER_COMPRESSION=OFF


new_fuzzers="libheif__heif_image_handle_get_color_profile_type__0 libheif__heif_image_handle_get_color_profile_type__1 libheif__heif_image_handle_get_color_profile_type__2 libheif__heif_image_handle_get_color_profile_type__4 libheif__heif_image_handle_get_luma_bits_per_pixel__1 libheif__heif_image_handle_get_luma_bits_per_pixel__2 libheif__heif_image_handle_get_luma_bits_per_pixel__3 libheif__heif_image_handle_get_luma_bits_per_pixel__4 libheif__heif_image_handle_get_luma_bits_per_pixel__7 libheif__heif_image_handle_get_raw_color_profile_size__0 libheif__heif_image_handle_get_raw_color_profile_size__1 libheif__heif_image_handle_get_raw_color_profile_size__3 libheif__heif_image_handle_get_raw_color_profile_size__4"


make -j$(nproc)
cp new_fuzzing/libheif* $OUT/
for f in $new_fuzzers; do
    cp ../fuzzing/data/dictionary.txt $OUT/${f}.dict
done


#./autogen.sh
#PKG_CONFIG="pkg-config --static" PKG_CONFIG_PATH="$DEPS_PATH/lib/pkgconfig" ./configure \
#    --disable-shared \
#    --enable-static \
#    --disable-examples \
#    --disable-go \
#    --enable-libfuzzer="$LIB_FUZZING_ENGINE" \
#    CPPFLAGS="-I$DEPS_PATH/include"
#make clean
#make -j$(nproc)

cp fuzzing/*_fuzzer $OUT
cp ../fuzzing/data/dictionary.txt $OUT/box-fuzzer.dict
cp ../fuzzing/data/dictionary.txt $OUT/file-fuzzer.dict

zip -r $OUT/file-fuzzer_seed_corpus.zip ../fuzzing/data/corpus/*.heic

for f in $new_fuzzers; do
    cp ../fuzzing/data/dictionary.txt $OUT/${f}.dict
    cp $OUT/file-fuzzer_seed_corpus.zip $OUT/${f}_seed_corpus.zip
done