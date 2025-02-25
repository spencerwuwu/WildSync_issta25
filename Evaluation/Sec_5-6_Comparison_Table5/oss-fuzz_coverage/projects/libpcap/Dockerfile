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
RUN git clone --no-checkout https://github.com/the-tcpdump-group/libpcap.git libpcap
RUN git -C libpcap checkout acc5cb90d78e467ccaa24953f6a729c623a99f25
# for corpus as wireshark
RUN git clone --no-checkout https://github.com/the-tcpdump-group/tcpdump.git tcpdump
RUN git -C tcpdump checkout 2fef164426273c4c610a3c060b7990db0e99914d
WORKDIR $SRC
COPY build.sh $SRC/

COPY new_fuzzers/ $SRC/new_fuzzers/
