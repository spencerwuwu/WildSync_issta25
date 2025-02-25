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
RUN apt-get update && apt-get install -y autoconf automake libtool

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/CESNET/libyang libyang
RUN git -C libyang checkout fba28260f382d81cf8f4b91b24cd717b52324fc2

RUN git clone --no-checkout https://github.com/PCRE2Project/pcre2 pcre2 && \
    git -C pcre2 checkout bfca266959547a5133aaf6faeba56d335e059fd0 && \
    cd pcre2 && \
    ./autogen.sh && \
    ./configure && \
    make && \
    make install

WORKDIR libyang
COPY build.sh $SRC/
COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/libyang/hopper_results/
