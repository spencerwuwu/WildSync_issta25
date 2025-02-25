#!/bin/bash -eu

# Copyright 2017-2018 Glenn Randers-Pehrson
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
# Revisions by Glenn Randers-Pehrson, 2017:
# 1. Build only the library, not the tools (changed "make -j$(nproc) all" to
#     "make -j$(nproc) libpng16.la").
# 2. Disabled WARNING and WRITE options in pnglibconf.dfa.
# 3. Build zlib alongside libpng
################################################################################

# Disable logging via library build configuration control.
cat scripts/pnglibconf.dfa | \
  sed -e "s/option STDIO/option STDIO disabled/" \
      -e "s/option WARNING /option WARNING disabled/" \
      -e "s/option WRITE enables WRITE_INT_FUNCTIONS/option WRITE disabled/" \
> scripts/pnglibconf.dfa.temp
mv scripts/pnglibconf.dfa.temp scripts/pnglibconf.dfa

# build the libpng library.
autoreconf -f -i
./configure --with-libpng-prefix=OSS_FUZZ_
make -j$(nproc) clean
make -j$(nproc) libpng16.la

# build libpng_read_fuzzer.
$CXX $CXXFLAGS -std=c++11 -I. \
     $SRC/libpng/contrib/oss-fuzz/libpng_read_fuzzer.cc \
     -o $OUT/libpng_read_fuzzer \
     -lFuzzingEngine .libs/libpng16.a -lz

new_fuzzers="libpng__png_get_PLTE__0 libpng__png_get_PLTE__1 libpng__png_get_PLTE__2 libpng__png_get_PLTE__3 libpng__png_get_PLTE__4 libpng__png_get_PLTE__5 libpng__png_get_bKGD__0 libpng__png_get_bKGD__1 libpng__png_get_bKGD__2 libpng__png_get_bKGD__3 libpng__png_get_bKGD__4 libpng__png_get_bit_depth__0 libpng__png_get_bit_depth__1 libpng__png_get_bit_depth__2 libpng__png_get_bit_depth__3 libpng__png_get_bit_depth__5 libpng__png_get_bit_depth__6 libpng__png_get_bit_depth__7 libpng__png_get_cHRM__0 libpng__png_get_cHRM_fixed__0 libpng__png_get_channels__0 libpng__png_get_channels__1 libpng__png_get_channels__2 libpng__png_get_channels__3 libpng__png_get_channels__4 libpng__png_get_channels__5 libpng__png_get_channels__6 libpng__png_get_channels__7 libpng__png_get_channels__8 libpng__png_get_channels__9 libpng__png_get_color_type__0 libpng__png_get_color_type__1 libpng__png_get_color_type__10 libpng__png_get_color_type__2 libpng__png_get_color_type__3 libpng__png_get_color_type__4 libpng__png_get_color_type__6 libpng__png_get_color_type__8 libpng__png_get_color_type__9 libpng__png_get_eXIf_1__0 libpng__png_get_eXIf_1__1 libpng__png_get_eXIf_1__3 libpng__png_get_filter_type__0 libpng__png_get_gAMA__0 libpng__png_get_gAMA__1 libpng__png_get_gAMA__2 libpng__png_get_gAMA__3 libpng__png_get_gAMA__5 libpng__png_get_iCCP__0 libpng__png_get_image_height__1 libpng__png_get_image_height__10 libpng__png_get_image_height__2 libpng__png_get_image_height__3 libpng__png_get_image_height__4 libpng__png_get_image_height__5 libpng__png_get_image_height__6 libpng__png_get_image_height__7 libpng__png_get_image_height__8 libpng__png_get_image_height__9 libpng__png_get_image_width__1 libpng__png_get_image_width__10 libpng__png_get_image_width__2 libpng__png_get_image_width__3 libpng__png_get_image_width__4 libpng__png_get_image_width__5 libpng__png_get_image_width__6 libpng__png_get_image_width__7 libpng__png_get_image_width__8 libpng__png_get_image_width__9 libpng__png_get_interlace_type__0 libpng__png_get_interlace_type__1 libpng__png_get_interlace_type__3 libpng__png_get_interlace_type__4 libpng__png_get_interlace_type__7 libpng__png_get_interlace_type__8 libpng__png_get_interlace_type__9 libpng__png_get_oFFs__0 libpng__png_get_oFFs__1 libpng__png_get_pHYs__0 libpng__png_get_pHYs__1 libpng__png_get_pixel_aspect_ratio__0 libpng__png_get_pixel_aspect_ratio__1 libpng__png_get_pixel_aspect_ratio__2 libpng__png_get_rows__0 libpng__png_get_rows__1 libpng__png_get_rows__2 libpng__png_get_rows__3 libpng__png_get_sBIT__0 libpng__png_get_sBIT__1 libpng__png_get_sBIT__2 libpng__png_get_sRGB__0 libpng__png_get_sRGB__1 libpng__png_get_sRGB__3 libpng__png_get_sRGB__4 libpng__png_get_tIME__0 libpng__png_get_tIME__1 libpng__png_get_tIME__2 libpng__png_get_tIME__3 libpng__png_get_tRNS__0 libpng__png_get_tRNS__1 libpng__png_get_tRNS__2 libpng__png_get_tRNS__3 libpng__png_get_tRNS__4 libpng__png_get_tRNS__5 libpng__png_get_tRNS__6 libpng__png_get_tRNS__7 libpng__png_get_text__0 libpng__png_get_text__1 libpng__png_get_text__3 libpng__png_get_text__4 libpng__png_get_text__5 libpng__png_get_text__6 libpng__png_get_unknown_chunks__0 libpng__png_get_user_chunk_ptr__1 libpng__png_get_valid__0 libpng__png_get_valid__5 libpng__png_get_x_pixels_per_meter__3 libpng__png_get_x_pixels_per_meter__4 libpng__png_get_x_pixels_per_meter__5 libpng__png_get_y_pixels_per_meter__3 libpng__png_get_y_pixels_per_meter__4 libpng__png_get_y_pixels_per_meter__5 libpng__png_warning__0"


for new_f in $new_fuzzers; do
    $CXX $CXXFLAGS -std=c++11 -I. \
         "$SRC/libpng/contrib/oss-fuzz/${new_f}.cpp" \
         -o $OUT/$new_f \
         -lFuzzingEngine .libs/libpng16.a -lz 
done



# add seed corpus.
find $SRC/libpng -name "*.png" | grep -v crashers | \
     xargs zip $OUT/libpng_read_fuzzer_seed_corpus.zip

cp $SRC/libpng/contrib/oss-fuzz/*.dict \
     $SRC/libpng/contrib/oss-fuzz/*.options $OUT/

for new_f in $new_fuzzers; do
  cp $OUT/libpng_read_fuzzer_seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
  cp $OUT/libpng_read_fuzzer.options $OUT/${new_f}.options
done
