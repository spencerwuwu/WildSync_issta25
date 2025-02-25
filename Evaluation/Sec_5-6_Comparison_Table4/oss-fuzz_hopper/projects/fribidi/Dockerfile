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

FROM gcr.io/oss-fuzz-base/base-builder
RUN apt-get update && apt-get install -y python3-pip pkg-config && \
    pip3 install meson==0.54.0 ninja

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/fribidi/fribidi.git
RUN git -C fribidi checkout bfb0385ef862cd40af7d5d33648fbb4bfe8eee65

WORKDIR fribidi
COPY build.sh $SRC/

COPY new_fuzz.patch $SRC/fribidi/
COPY new_fuzzers $SRC/fribidi/new_fuzzers
COPY meson.build $SRC/fribidi/new_fuzzers/
RUN  cd $SRC/fribidi/ && git apply new_fuzz.patch

COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/fribidi/hopper_results/
