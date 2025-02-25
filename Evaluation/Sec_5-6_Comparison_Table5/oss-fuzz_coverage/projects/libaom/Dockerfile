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
RUN apt-get update && apt-get install -y cmake yasm wget
RUN git clone --no-checkout https://aomedia.googlesource.com/aom aom
# Legacy
RUN git -C aom checkout 9a83c6a5a55c176adbce740e47d3512edfc9ae71
ADD https://storage.googleapis.com/aom-test-data/fuzzer/dec_fuzzer_seed_corpus.zip $SRC/
COPY build.sh $SRC/
WORKDIR aom

COPY new_fuzzers/ $SRC/new_fuzzers/
COPY addon.sh $SRC/
RUN cat $SRC/addon.sh >> $SRC/build.sh
