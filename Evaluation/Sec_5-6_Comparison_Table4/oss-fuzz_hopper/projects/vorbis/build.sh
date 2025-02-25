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

export WORK=/home/work
mkdir -p $WORK

#. contrib/oss-fuzz/build.sh
pushd $SRC/ogg
./autogen.sh
./configure --prefix="$WORK" --enable-static --disable-shared --disable-crc
make clean
make -j$(nproc)
make install
popd


./autogen.sh
./configure --prefix="$WORK" --enable-static --disable-shared
make clean
make -j$(nproc)
make install

###################################

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/vorbis/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
        mkdir -p $out_path
        if [ ! "$(ls -A $path)" ]; then
            continue
        fi
        for cfile in "$path"/*.c; do
            out_name=$(basename -s .c $cfile)
            $CC $CFLAGS -I"$WORK/include" \
              $cfile -c -o /tmp/$out_name &> /home/log
            if [ $? -ne 0 ]; then
                $SRC/hopper_helper.py $cfile /home/log
                $CC $CFLAGS -I"$WORK/include" \
                  $cfile -c -o /tmp/$out_name 
            fi
            if [ ! -f /tmp/$out_name ]; then
                continue
            fi
            $CXX $CXXFLAGS -I"$WORK/include" \
                /tmp/$out_name \
                -o $out_path/$out_name \
                -L"$WORK/lib" -lvorbisfile -lvorbis -logg
        done
    done
done

true
