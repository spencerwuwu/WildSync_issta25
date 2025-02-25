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

#FROM gcr.io/oss-fuzz-base/base-builder
FROM oss-fuzz-hopper
RUN apt-get update && apt-get install -y make autoconf automake libtool cmake nasm

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://gitlab.com/libtiff/libtiff
RUN git -C libtiff checkout bb1ff4f80ecb7b7b635fd51fc2ebf04d8f09109a
RUN git clone --no-checkout https://github.com/madler/zlib
RUN git -C zlib checkout 99b229487c2997b4b22eaef90fedfbff4d8826cc
RUN git clone --no-checkout https://github.com/libjpeg-turbo/libjpeg-turbo
RUN git -C libjpeg-turbo checkout 2dfe6c0fe9e18671105e94f7cbf044d4a1d157e6
RUN git clone --no-checkout https://www.cl.cam.ac.uk/~mgk25/git/jbigkit
RUN git -C jbigkit checkout 7d3c1bea895d910907e2501fe9165e353eceabae
ADD https://lcamtuf.coredump.cx/afl/demo/afl_testcases.tgz afl_testcases.tgz
ADD https://raw.githubusercontent.com/google/AFL/debe27037b9444bbf090a0ffbd5d24889bb887ae/dictionaries/tiff.dict tiff.dict
WORKDIR libtiff
COPY build.sh $SRC/

#COPY build-addon.sh $SRC/
#RUN cat $SRC/build-addon.sh >> $SRC/libtiff/contrib/oss-fuzz/build.sh 
#COPY new_fuzzers/* $SRC/libtiff/contrib/oss-fuzz/ 

COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/libtiff/hopper_results/

# casr
RUN bash $SRC/build.sh
ENTRYPOINT ["/bin/bash", "/home/process_casr.sh"]
