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

cd libpcap
# build project
mkdir build
cd build
cmake ..
make


# build fuzz targets
for target in pcap filter both
do
    $CC $CFLAGS -I.. -c ../testprogs/fuzz/fuzz_$target.c -o fuzz_$target.o
    $CXX $CXXFLAGS fuzz_$target.o -o $OUT/fuzz_$target libpcap.a $LIB_FUZZING_ENGINE
done

new_fuzzers="libpcap__pcap_activate__0 libpcap__pcap_activate__1 libpcap__pcap_activate__2 libpcap__pcap_activate__3 libpcap__pcap_activate__4 libpcap__pcap_breakloop__0 libpcap__pcap_breakloop__1 libpcap__pcap_breakloop__2 libpcap__pcap_breakloop__3 libpcap__pcap_breakloop__4 libpcap__pcap_breakloop__5 libpcap__pcap_breakloop__6 libpcap__pcap_compile_nopcap__0 libpcap__pcap_datalink__0 libpcap__pcap_datalink__1 libpcap__pcap_datalink__2 libpcap__pcap_datalink__3 libpcap__pcap_datalink__4 libpcap__pcap_datalink__5 libpcap__pcap_datalink__6 libpcap__pcap_datalink__7 libpcap__pcap_dispatch__7 libpcap__pcap_fileno__1 libpcap__pcap_fileno__2 libpcap__pcap_fileno__6 libpcap__pcap_fileno__7 libpcap__pcap_get_selectable_fd__0 libpcap__pcap_get_selectable_fd__1 libpcap__pcap_get_selectable_fd__2 libpcap__pcap_get_selectable_fd__3 libpcap__pcap_get_selectable_fd__4 libpcap__pcap_get_selectable_fd__5 libpcap__pcap_get_selectable_fd__6 libpcap__pcap_get_selectable_fd__7 libpcap__pcap_get_selectable_fd__8 libpcap__pcap_inject__1 libpcap__pcap_list_datalinks__0 libpcap__pcap_list_tstamp_types__0 libpcap__pcap_next__0 libpcap__pcap_next__1 libpcap__pcap_next__2 libpcap__pcap_next__3 libpcap__pcap_sendpacket__0 libpcap__pcap_snapshot__0 libpcap__pcap_snapshot__1 libpcap__pcap_snapshot__2 libpcap__pcap_snapshot__3 libpcap__pcap_snapshot__4 libpcap__pcap_snapshot__5 libpcap__pcap_snapshot__7 libpcap__pcap_snapshot__9"


for new_f in $new_fuzzers; do
    cp "$SRC/new_fuzzers/${new_f}.cpp" "$SRC/new_fuzzers/${new_f}.c"
    $CC $CFLAGS -I.. -c "$SRC/new_fuzzers/${new_f}.c" -o ${new_f}.o
    if [ $? -ne 0 ]; then
        echo "Failed to compile $new_f"
        continue
    fi
    $CXX $CXXFLAGS ${new_f}.o \
        -o $OUT/${new_f} \
        libpcap.a $LIB_FUZZING_ENGINE 
done


# export other associated stuff
cd ..
cp testprogs/fuzz/fuzz_*.options $OUT/
# builds corpus
cd $SRC/tcpdump/
zip -r fuzz_pcap_seed_corpus.zip tests/
cp fuzz_pcap_seed_corpus.zip $OUT/
cd $SRC/libpcap/testprogs/BPF
mkdir corpus
ls *.txt | while read i; do tail -1 $i > corpus/$i; done
zip -r fuzz_filter_seed_corpus.zip corpus/
cp fuzz_filter_seed_corpus.zip $OUT/

for new_f in $new_fuzzers; do
    cp fuzz_filter_seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
done
