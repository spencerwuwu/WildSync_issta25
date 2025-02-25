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

# For fuzz-introspector. This is to exclude all libxml2 code from the
# fuzz-introspector reports.
export FUZZ_INTROSPECTOR_CONFIG=$SRC/fuzz_introspector_exclusion.config
cat > $FUZZ_INTROSPECTOR_CONFIG <<EOF
FILES_TO_AVOID
libxml2
EOF

DEPS=/deps

cd $SRC/libarchive

sed -i 's/-Wall//g' ./CMakeLists.txt
sed -i 's/-Werror//g' ./CMakeLists.txt

mkdir build2
cd build2
cmake -DCHECK_CRC_ON_SOLID_SKIP=1 -DDONT_FAIL_ON_CRC_ERROR=1 ../
make -j$(nproc)


###################################

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/libarchive/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
        mkdir -p $out_path
        if [ ! "$(ls -A $path)" ]; then
            continue
        fi
        for cfile in "$path"/*.c; do
            out_name=$(basename -s .c $cfile)
            $CC $CFLAGS -I../libarchive \
                $cfile -o $out_path/$out_name \
                ./libarchive/libarchive.a \
                -Wl,-Bstatic -llzo2 -Wl,-Bdynamic -lcrypto -lacl -llzma -llz4 -lbz2 -lz ${DEPS}/libxml2.a &> /home/log
            if [ $? -ne 0 ]; then
                $SRC/hopper_helper.py $cfile /home/log
                $CC $CFLAGS -I../libarchive \
                    $cfile -o $out_path/$out_name \
                    ./libarchive/libarchive.a \
                    -Wl,-Bstatic -llzo2 -Wl,-Bdynamic -lcrypto -lacl -llzma -llz4 -lbz2 -lz ${DEPS}/libxml2.a
            fi
        done
    done
done

true


# build fuzzer(s)
#$CXX $CXXFLAGS -I../libarchive \
#    $SRC/libarchive_fuzzer.cc -o $OUT/libarchive_fuzzer \
#    $LIB_FUZZING_ENGINE ./libarchive/libarchive.a \
#    -Wl,-Bstatic -llzo2 -Wl,-Bdynamic -lcrypto -lacl -llzma -llz4 -lbz2 -lz ${DEPS}/libxml2.a

