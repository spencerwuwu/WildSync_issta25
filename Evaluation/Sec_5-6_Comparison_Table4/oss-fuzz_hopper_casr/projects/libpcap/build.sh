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

# build project
mkdir build
cd build
cmake ..
make


# build fuzz targets
#for target in pcap filter both
#do
#    $CC $CFLAGS -I.. -c ../testprogs/fuzz/fuzz_$target.c -o fuzz_$target.o
#    $CXX $CXXFLAGS fuzz_$target.o -o $OUT/fuzz_$target libpcap.a $LIB_FUZZING_ENGINE
#done

###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

more_flags="$(pkg-config --static --libs libibverbs)"

#set +e
path="$SRC/libpcap/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS -I.. $cfile -o $out_path/$out_name libpcap.a -lm -lnl-genl-3 -lnl-3 $more_flags &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -I.. $cfile -o $out_path/$out_name libpcap.a -lm -lnl-genl-3 -lnl-3 $more_flags
    fi
done

#true
