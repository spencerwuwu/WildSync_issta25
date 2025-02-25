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
    apt-get install -y python3-pip gtk-doc-tools libffi-dev autotools-dev libtool gperf
RUN pip3 install -U meson==1.2.0 ninja packaging

RUN git clone --no-checkout git://git.sv.nongnu.org/freetype/freetype2.git
RUN git -C freetype2 checkout f42ce25563b73fed0123d18a2556b9ba01d2c76b
RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/glib
RUN git -C glib checkout 308b06d3523106daa3b4828928fa7772b24defe7
RUN git clone --no-checkout https://gitlab.freedesktop.org/cairo/cairo.git
RUN git -C cairo checkout bbbc68e6195ba75bcb3ef71a4be8082aa6619862
RUN zip -q $SRC/cairo_seed_corpus.zip $SRC/cairo/test/reference/*

ADD https://raw.githubusercontent.com/google/fuzzing/master/dictionaries/png.dict $SRC/cairo.dict

WORKDIR $SRC/cairo
COPY targets $SRC/fuzz
COPY build.sh $SRC/

COPY build-addon.sh $SRC/
RUN cat $SRC/build-addon.sh >> $SRC/build.sh
COPY new_fuzzers/* $SRC/fuzz/ 
