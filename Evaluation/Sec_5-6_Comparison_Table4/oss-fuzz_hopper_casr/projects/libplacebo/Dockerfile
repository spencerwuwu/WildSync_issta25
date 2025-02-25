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

FROM gcr.io/oss-fuzz-base/base-builder
RUN apt-get update && apt-get install -y python3-pip pkg-config && \
    pip3 install meson ninja

RUN git clone --no-checkout https://code.videolan.org/videolan/libplacebo.git libplacebo
RUN git -C libplacebo checkout 1fd3c7bde7b943fe8985c893310b5269a09b46c5

COPY build.sh $SRC/
COPY *.c $SRC/libplacebo/src/tests/fuzz/
COPY meson.build $SRC/libplacebo/src

COPY new_fuzzers $SRC/new_fuzzers

WORKDIR libplacebo
