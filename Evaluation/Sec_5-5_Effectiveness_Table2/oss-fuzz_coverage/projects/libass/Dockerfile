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
RUN apt-get update && apt-get install -y make autoconf automake libtool pkg-config libfontconfig1-dev libfreetype-dev libfribidi-dev python3-pip && \
    pip3 install meson==0.55.0 ninja

RUN git clone --no-checkout https://github.com/libass/libass.git
RUN git -C libass checkout 649a7c2e1fc6f4188ea1a89968560715800b883d

RUN git clone --no-checkout https://github.com/harfbuzz/harfbuzz.git
RUN git -C harfbuzz checkout 3c8d448fa1174b743963fbc5061a585a255e880b

COPY build.sh *.options $SRC/

COPY Makefile_util.am $SRC/libass/
COPY new_fuzzers $SRC/new_fuzzers
COPY seeds.zip $SRC/seeds.zip

#COPY new_seeds $SRC/new_seeds
