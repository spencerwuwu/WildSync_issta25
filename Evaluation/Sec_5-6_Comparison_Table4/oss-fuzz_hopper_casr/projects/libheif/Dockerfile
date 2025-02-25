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

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN apt-get update && apt-get install -y \
    autoconf \
    automake \
    build-essential \
    cmake \
    libtool \
    make \
    mercurial \
    pkg-config \
    yasm

RUN git clone \
    --no-checkout \
    https://github.com/strukturag/libde265.git \
    libde265
RUN git -C libde265 checkout c7c724bfc873a7bebe1855b7a34c1e83337c1ff3

#RUN hg clone \
#    http://hg.videolan.org/x265 \
#    x265
RUN git clone \
    --no-checkout \
    https://github.com/videolan/x265.git \
    x265
RUN git -C x265 checkout 419182243fb2e2dfbe91dfc45a51778cf704f849

RUN git clone \
    --no-checkout \
    https://aomedia.googlesource.com/aom \
    aom
RUN git -C aom checkout 402e264b94fd74bdf66837da216b6251805b4ae4

RUN git clone \
    --no-checkout \
    https://github.com/strukturag/libheif.git \
    libheif
RUN git -C libheif checkout c98a3fae1ac63cc6052e40b26733396a8598542d

WORKDIR libheif

COPY build.sh $SRC/
# This is to fix Fuzz Introspector build by using LLVM old pass manager
# re https://github.com/ossf/fuzz-introspector/issues/305
ENV OLD_LLVMPASS 1

COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/libgd/hopper_results/

# casr
RUN bash $SRC/build.sh
ENTRYPOINT ["/bin/bash", "/home/process_casr.sh"]
