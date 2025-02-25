#!/bin/bash -eu
# Copyright 2021 Google LLC
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


# build project
mkdir build
cd build
cmake -DWITH_TESTS=OFF -DWITH_ZLIB=OFF -DBUILD_SHARED_LIBS=OFF ..
cmake --build .


###################################

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

INCLUDES="-DLIBVNCSERVER_HAVE_LIBZ -DLIBVNCSERVER_WITH_WEBSOCKETS -I/home/libvncserver/include -I/home/libvncserver/build/include -I/home/libvncserver/src/libvncserver -I/home/libvncserver/home/libvncclient -I/home/libvncserver/src/common"

set +e
for round_path in "$SRC/libvncserver/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
        mkdir -p $out_path
        if [ ! "$(ls -A $path)" ]; then
            continue
        fi
        for cfile in "$path"/*.c; do
            out_name=$(basename -s .c $cfile)
            $CC $CFLAGS -std=c11 \
                $INCLUDES \
                $cfile -o $out_path/$out_name \
                /home/libvncserver/build/libvncserver.a \
                /home/libvncserver/build/libvncclient.a \
                -lz -lcrypt -lssl -lcrypto -ldl -pthread &> /home/log
            if [ $? -ne 0 ]; then
                $SRC/hopper_helper.py $cfile /home/log
                $CC $CFLAGS -std=c11 \
                $INCLUDES \
                    $cfile -o $out_path/$out_name \
                    /home/libvncserver/build/libvncserver.a \
                    /home/libvncserver/build/libvncclient.a \
                    -lz -lcrypt -lssl -lcrypto -ldl -pthread
            fi
        done
    done
done

true
