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

FROM gcr.io/oss-fuzz-base/base-builder
RUN apt-get update && apt-get install -y yasm wget gcc
RUN git clone --no-checkout https://chromium.googlesource.com/webm/libvpx libvpx
# LEGACY
RUN git -C libvpx checkout 87315c0124e1c388a035bf5719be63d96e079dc2
ADD https://storage.googleapis.com/downloads.webmproject.org/test_data/fuzzer/vpx_fuzzer_seed_corpus.zip $SRC/
COPY build.sh vpx_dec_fuzzer.dict $SRC/

#COPY new_fuzzers/ $SRC/new_fuzzers/
COPY addon.sh $SRC/
RUN cat $SRC/addon.sh >> $SRC/build.sh

WORKDIR libvpx
