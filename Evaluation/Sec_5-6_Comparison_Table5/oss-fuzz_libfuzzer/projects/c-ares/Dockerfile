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
RUN apt-get update && apt-get install -y make autoconf automake libtool
RUN git clone --no-checkout https://github.com/c-ares/c-ares.git c-ares
# Legacy
RUN git -C c-ares checkout 4b7301a0d28aae87004a428918d2b38c6b92aef3
WORKDIR c-ares
COPY build.sh $SRC/

COPY new_fuzzers/ $SRC/new_fuzzers/
COPY addon.sh $SRC/
RUN cat $SRC/addon.sh >> $SRC/build.sh
