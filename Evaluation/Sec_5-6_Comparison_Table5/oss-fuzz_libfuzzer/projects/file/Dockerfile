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
RUN apt-get update && apt install -y make autoconf automake libtool shtool
RUN apt-get update && apt install -y libbz2-dev      liblzma-dev      zlib1g-dev      libzstd-dev      liblz4-dev
RUN apt-get update && apt install -y libbz2-dev:i386 liblzma-dev:i386 zlib1g-dev:i386 libzstd-dev:i386 liblz4-dev:i386 
RUN git clone --no-checkout https://github.com/file/file.git file
RUN git -C file checkout 0fa2c8c3e64c372d038d46969bafaaa09a13a87b

# Supplement file's existing test files
RUN git clone --depth 1 https://github.com/DavidKorczynski/binary-samples.git
RUN git clone --depth 1 https://github.com/corkami/pocs

WORKDIR file
COPY build.sh fuzzer_temp_file.h magic_fuzzer.cc magic_fuzzer_loaddb.cc magic_fuzzer_fd.cc $SRC/

#COPY new_fuzzers $SRC/new_fuzzers
