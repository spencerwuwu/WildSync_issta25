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

#FROM gcr.io/oss-fuzz-base/base-builder
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/cjson_hopper
ENV LIB_PATH="/home/cjson/"

ENV DEPS_LDD=${LIB_PATH}/build/
ENV HEADER=${LIB_PATH}/cJSON.h
ENV SHARED_LIB=${LIB_PATH}/build/libcjson.so

####################

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/DaveGamble/cJSON.git cjson
RUN git -C cjson checkout 424ce4ce9668f288fb4ab665775546d3ed709e96

WORKDIR cjson
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY --chown=hopper:hopper hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

