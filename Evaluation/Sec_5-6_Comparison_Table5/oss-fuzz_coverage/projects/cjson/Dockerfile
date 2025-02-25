#!/bin/bash -eu
# Copyright 2019 Google Inc.
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


RUN apt-get update && apt-get install -y cmake

RUN git clone --no-checkout https://github.com/DaveGamble/cJSON.git cjson
RUN git -C cjson checkout 424ce4ce9668f288fb4ab665775546d3ed709e96

WORKDIR cjson
COPY build.sh $SRC/

COPY build-addon.sh $SRC/
COPY new_fuzzers/* $SRC/cjson/fuzzing/
RUN cat $SRC/build-addon.sh >> $SRC/cjson/fuzzing/ossfuzz.sh
