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

new_fuzzers="libvnc__rfbClientSendString__0 libvnc__rfbDoCopyRegion__0 libvnc__rfbHttpCheckFds__0 libvnc__rfbHttpCheckFds__1 libvnc__rfbHttpCheckFds__2 libvnc__rfbHttpCheckFds__3 libvnc__rfbNewFramebuffer__0 libvnc__rfbNewFramebuffer__1 libvnc__rfbProcessNewConnection__0 libvnc__rfbRunEventLoop__0 libvnc__rfbRunEventLoop__1 libvnc__rfbScreenCleanup__0 libvnc__rfbSendBell__0 libvnc__rfbSendBell__1 libvnc__rfbSendServerCutText__1 libvnc__rfbSendServerCutText__3 libvnc__rfbShutdownSockets__0 libvnc__rfbStartOnHoldClient__0 libvnc__rfbStartOnHoldClient__1 libvnc__rfbStatGetSentBytesIfRaw__0 libvnc__rfbStatGetSentBytesIfRaw__1 libvnc__rfbStatGetSentBytesIfRaw__2 libvnc__rfbStatGetSentBytesIfRaw__3 libvnc__rfbStatGetSentBytes__0"
for new_f in $new_fuzzers; do
    cp "${SRC}/new_fuzzers/${new_f}.cpp" test/${new_f}.c
done


cp $SRC/CMakeLists.txt $SRC/libvncserver/CMakeLists.txt

# build project
mkdir build
cd build
cmake -DWITH_TESTS=ON -DBUILD_SHARED_LIBS=OFF ..
cmake --build .
cp fuzz* $OUT/

for new_f in $new_fuzzers; do
    cp $new_f $OUT/
done



# Adds simple corpus and dictionary
echo "RFB 003.008" > rfb_corpus
zip -r $OUT/fuzz_server_seed_corpus.zip rfb_corpus
echo "\"RFB \"" > $OUT/fuzz_server.dict

for new_f in $new_fuzzers; do
    cp $OUT/fuzz_server_seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
    cp $OUT/fuzz_server.dict $OUT/${new_f}.dict
done