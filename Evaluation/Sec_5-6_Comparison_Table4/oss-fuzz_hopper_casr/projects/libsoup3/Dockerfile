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
RUN apt-get update && \
    apt-get install -y python3-pip gtk-doc-tools libffi-dev autotools-dev \
            libtool gperf libnghttp2-dev libsqlite3-dev libpsl-dev && \
    pip3 install meson ninja

RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/glib
RUN git -C glib checkout 1e6c6ebaa5330802910fb60256c94fd42b390038

RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/libsoup
RUN git -C libsoup checkout 521a861524042dda0835cd7a5d4fb0bf29f2896b

WORKDIR libsoup
COPY build.sh $SRC/

COPY meson.build $SRC/libsoup/fuzzing
COPY new_fuzzers $SRC/new_fuzzers
COPY new_fuzz.h $SRC/libsoup/fuzzing
