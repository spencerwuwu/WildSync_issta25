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

#FROM gcr.io/oss-fuzz-base/base-builder
FROM oss-fuzz-hopper
RUN apt-get update && \
    apt-get install -y make autoconf automake libtool zlib1g-dev

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/pnggroup/libpng.git
RUN git -C libpng checkout f1848a3b560ddcad065242268433af475948461e
#RUN cp libpng/contrib/oss-fuzz/build.sh $SRC

WORKDIR libpng

COPY build.sh $SRC/

COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/libpng/hopper_results/

# casr
RUN bash $SRC/build.sh
ENTRYPOINT ["/bin/bash", "/home/process_casr.sh"]
