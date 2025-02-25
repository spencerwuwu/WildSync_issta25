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
RUN apt-get update && apt-get install -y make autoconf automake libtool \
  pkg-config cmake nasm

RUN git clone --no-checkout https://github.com/DanBloomberg/leptonica.git leptonica
RUN git -C leptonica checkout 32af1df77f4f6311e581f2e263fc51506a5b6340

RUN git clone --no-checkout https://github.com/madler/zlib.git zlib
RUN git -C zlib checkout 99b229487c2997b4b22eaef90fedfbff4d8826cc
RUN git clone --no-checkout https://gitlab.com/libtiff/libtiff libtiff
RUN git -C libtiff checkout 1d28fe638020f55644319599ece84b3fb0b39bb5
RUN git clone --no-checkout https://github.com/glennrp/libpng.git libpng
RUN git -C libpng checkout f1848a3b560ddcad065242268433af475948461e
RUN git clone --no-checkout https://chromium.googlesource.com/webm/libwebp libwebp
RUN git -C libwebp checkout a16d30cb578cedd3b3a2d75bf041f99310fdef63
RUN git clone --no-checkout https://www.cl.cam.ac.uk/~mgk25/git/jbigkit jbigkit
RUN git -C jbigkit checkout 7d3c1bea895d910907e2501fe9165e353eceabae
RUN git clone --no-checkout https://github.com/libjpeg-turbo/libjpeg-turbo libjpeg-turbo
RUN git -C libjpeg-turbo checkout dfde1f857d5bdc0ff214c5f920f04ea8287005ed
RUN git clone --no-checkout https://github.com/facebook/zstd zstd
RUN git -C zstd checkout 216099a73f6ec19c246019df12a2877dada45cca

WORKDIR leptonica
COPY build.sh $SRC/
COPY build-addon.sh $SRC/
RUN cat $SRC/build-addon.sh >> $SRC/leptonica/prog/fuzzing/oss-fuzz-build.sh

COPY new_fuzzers $SRC/new_fuzzers
