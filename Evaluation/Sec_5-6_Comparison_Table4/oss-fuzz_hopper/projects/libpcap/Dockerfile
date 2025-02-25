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
RUN apt-get update && apt-get install -y make cmake flex bison

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/the-tcpdump-group/libpcap.git libpcap
RUN git -C libpcap checkout 15021c447e82edb2442c46eb72027d905a534c81
# for corpus as wireshark
#RUN git clone --no-checkout https://github.com/the-tcpdump-group/tcpdump.git tcpdump
#RUN git -C tcpdump checkout 07a730a6f5d7d92d34cd7b137fa7fc21505f8441

WORKDIR libpcap
COPY build.sh $SRC/

COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/libpcap/hopper_results/
