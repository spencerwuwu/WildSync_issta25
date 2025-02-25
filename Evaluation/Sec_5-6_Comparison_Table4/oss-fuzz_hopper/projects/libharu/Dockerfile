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
#RUN apt-get update && \
#    apt-get install -y python3-pip gtk-doc-tools libffi-dev autotools-dev \
#            libtool gperf libnghttp2-dev libsqlite3-dev libpsl-dev 
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        make libtool pkg-config gtk-doc-tools autotools-dev automake 

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home
        
RUN git clone --no-checkout https://github.com/madler/zlib.git
RUN git -C zlib checkout 0f51fb4933fc9ce18199cb2554dacea8033e7fd3
RUN git clone --no-checkout https://github.com/pnggroup/libpng.git
RUN git -C libpng checkout f1848a3b560ddcad065242268433af475948461e

RUN git clone --no-checkout https://github.com/libharu/libharu.git libharu
RUN git -C libharu checkout 0c598becaadaef8e3d12b883f9fc2864a118c12d

WORKDIR $SRC/libharu

COPY build.sh $SRC/
COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/libharu/hopper_results/

#COPY build.sh $SRC/
#COPY fuzzers/ $SRC/fuzzers/
#
#COPY addon.sh $SRC/
#COPY new_fuzzers/ $SRC/new_fuzzers/
#RUN cat $SRC/addon.sh >> $SRC/build.sh
