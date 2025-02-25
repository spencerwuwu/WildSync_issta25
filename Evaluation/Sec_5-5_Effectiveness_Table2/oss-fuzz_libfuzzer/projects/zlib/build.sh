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

new_fuzzers="zlib__deflateCopy__0 zlib__deflatePending__0 zlib__gzclose_r__1 zlib__gzclose_r__3 zlib__gzclose_r__5 zlib__gzclose_r__7 zlib__gzdirect__0 zlib__gzeof__0 zlib__gzeof__1 zlib__gzeof__3 zlib__gzeof__6 zlib__gzeof__7 zlib__gzflush__0 zlib__gzgetc__0 zlib__gzgetc__1 zlib__gzgetc__2 zlib__gzgetc__3 zlib__gzgetc__4 zlib__gzgetc__5 zlib__gzgetc__6 zlib__gzgetc__7 zlib__gzgetc__8 zlib__gzputc__0 zlib__gzputc__1 zlib__gzputc__2 zlib__gzputs__0 zlib__gzputs__1 zlib__gzputs__2 zlib__gzrewind__0 zlib__gzrewind__1 zlib__gzrewind__2 zlib__gzrewind__3 zlib__gzrewind__4 zlib__gzseek64__0 zlib__gzseek64__1 zlib__gzseek__2 zlib__gzseek__4 zlib__gzseek__6 zlib__gztell64__0 zlib__gztell64__1 zlib__gztell__0 zlib__gztell__1 zlib__gzungetc__0 zlib__inflateBackEnd__0 zlib__inflateBackEnd__1 zlib__inflateBackEnd__10 zlib__inflateBackEnd__2 zlib__inflateBackEnd__3 zlib__inflateBackEnd__4 zlib__inflateBackEnd__5 zlib__inflateBackEnd__6 zlib__inflateBackEnd__7 zlib__inflateBackEnd__8 zlib__inflateBackEnd__9 zlib__inflateBack__0 zlib__inflateBack__4 zlib__inflateBack__8 zlib__inflateCopy__0 zlib__inflateCopy__1 zlib__inflateCopy__2 zlib__inflateCopy__3 zlib__inflateCopy__4 zlib__inflateCopy__5 zlib__inflateCopy__6 zlib__inflateCopy__7 zlib__inflateCopy__8 zlib__inflateCopy__9 zlib__inflateGetHeader__0 zlib__inflateGetHeader__1 zlib__inflateGetHeader__3 zlib__inflateGetHeader__4 zlib__inflateMark__0 zlib__inflateMark__1 zlib__inflateMark__2 zlib__inflatePrime__1 zlib__inflatePrime__2 zlib__inflatePrime__3 zlib__inflatePrime__4 zlib__inflatePrime__5 zlib__inflatePrime__6 zlib__inflatePrime__7 zlib__inflatePrime__8 zlib__inflatePrime__9 zlib__inflateSyncPoint__0 zlib__inflateSyncPoint__1 zlib__inflateUndermine__0 zlib__inflateUndermine__1 zlib__inflateValidate__0 zlib__inflateValidate__1 zlib__inflateValidate__2 zlib__inflateValidate__3 zlib__inflateValidate__4 zlib__inflateValidate__5"

# original code
for f in $(find $SRC -name '*_fuzzer.cc'); do
    b=$(basename -s .cc $f)
    $CXX $CXXFLAGS -std=c++11 -I. $f -o $OUT/$b $LIB_FUZZING_ENGINE ./libz.a
done

zip $OUT/seed_corpus.zip *.*

for f in $(find $SRC -name '*_fuzzer.c'); do
    b=$(basename -s .c $f)
    $CC $CFLAGS -I. $f -c -o /tmp/$b.o
    $CXX $CXXFLAGS -o $OUT/$b /tmp/$b.o -stdlib=libc++ $LIB_FUZZING_ENGINE ./libz.a
    rm -f /tmp/$b.o
    ln -sf $OUT/seed_corpus.zip $OUT/${b}_seed_corpus.zip
done

for new_f in $new_fuzzers; do
    echo "==== Compiling $new_f ===="
    cp $SRC/${new_f}.cpp $SRC/${new_f}.c
    $CC $CFLAGS -I. $SRC/${new_f}.c -c -o /tmp/${new_f}.o
    if [ -f /tmp/${new_f}.o ]; then
        $CXX $CXXFLAGS -o $OUT/${new_f} /tmp/${new_f}.o -stdlib=libc++ $LIB_FUZZING_ENGINE ./libz.a
    fi
    rm -f /tmp/${new_f}.o
    ln -sf $OUT/seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
done


