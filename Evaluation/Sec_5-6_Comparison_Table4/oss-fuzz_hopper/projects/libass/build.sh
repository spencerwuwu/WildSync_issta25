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

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/libass/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
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
    done
done

true


#cp fuzz/fuzz_ossfuzz $OUT/libass_fuzzer
#cp fuzz/ass.dict $OUT/ass.dict
#
## New fuzzers
#new_fuzzers="libass__ass_add_font__0 libass__ass_flush_events__0 libass__ass_flush_events__1 libass__ass_flush_events__2 libass__ass_flush_events__3 libass__ass_process_chunk__0 libass__ass_process_codec_private__0"
#new_fuzzers+=" libass__ass_add_font__0__tweak0 libass__ass_process_chunk__0__tweak0 libass__ass_process_codec_private__0__tweak0"
#
#for new_f in $new_fuzzers; do
#    cp "${SRC}/new_fuzzers/${new_f}.cpp" fuzz/${new_f}.c
#    make -j "$(nproc)" fuzz/$new_f
#    cp fuzz/$new_f "${OUT}/${new_f}"
#    cp $SRC/seeds.zip "${OUT}/${new_f}_seed_corpus.zip"
#done
#
#
#cp $SRC/*.options $OUT/
#
#cp $SRC/seeds.zip $OUT/libass_fuzzer_seed_corpus.zip
