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

sed -i 's/-Werror//g' ./configure.ac
./bootstrap.sh

# Limit the size of buffer allocations to avoid bogus OOM issues
# https://github.com/libgd/libgd/issues/422
sed -i'' -e 's/INT_MAX/100000/' "$SRC/libgd/src/gd_security.c"

./configure --prefix="$WORK" --disable-shared
make -j$(nproc) install

# New fuzzers
new_fuzzers="libgd__gdImageColorClosest__0 libgd__gdImageColorClosest__1 libgd__gdImageColorDeallocate__0 libgd__gdImageColorExact__0 libgd__gdImageColorResolve__0 libgd__gdImageCopyRotated__0 libgd__gdImagePaletteCopy__0 libgd__gdImagePaletteCopy__1"


for target in Bmp Gd Gd2 Gif Jpeg Png Tga Tiff WBMP Webp; do
    lowercase=$(echo $target | tr "[:upper:]" "[:lower:]")
    $CXX $CXXFLAGS -std=c++11 -I"$WORK/include" -L"$WORK/lib" \
      -DFUZZ_GD_FORMAT=$target \
      $SRC/parser_target.cc -o $OUT/${lowercase}_target \
      $LIB_FUZZING_ENGINE -lgd -Wl,-Bstatic -lz -Wl,-Bdynamic
done

for fuzzers in $(find $SRC -name '*_fuzzer.cc'); do
      fuzz_basename=$(basename -s .cc $fuzzers)
      $CXX $CXXFLAGS -std=c++11 -I"$WORK/include" -L"$WORK/lib" \
      $fuzzers -o $OUT/$fuzz_basename \
      $LIB_FUZZING_ENGINE -lgd -Wl,-Bstatic -lz -Wl,-Bdynamic
done

mkdir afl_testcases
(cd afl_testcases; tar xvf "$SRC/afl_testcases.tgz")
for format in bmp gif png webp; do
    mkdir $format
    find afl_testcases -type f -name '*.'$format -exec mv -n {} $format/ \;
    zip -rj $format.zip $format/
    cp $format.zip "$OUT/${format}_target_seed_corpus.zip"
done

for new_f in $new_fuzzers; do
    $CXX $CXXFLAGS -std=c++11 -I"$WORK/include" -L"$WORK/lib" \
      $SRC/$new_f.cpp -o $OUT/$new_f \
      $LIB_FUZZING_ENGINE -lgd -Wl,-Bstatic -lz -Wl,-Bdynamic
done

