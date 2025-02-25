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
RUN apt-get update && apt-get install -y make autoconf automake libtool curl tcl zlib1g-dev tcl-dev

# We won't be able to poll fossil for changes, so this will build
# only once a day.
#RUN mkdir $SRC/sqlite3 && \
#    cd $SRC/sqlite3 && \
#    curl 'https://www.sqlite.org/src/tarball?uuid=trunk' -o sqlite3.tar.gz && \
#    tar xzf sqlite3.tar.gz
RUN git clone --no-checkout https://github.com/sqlite/sqlite.git sqlite3
# LEGACY
RUN git -C sqlite3 checkout dcd7408db8d1c68995bdc4825d281e7cc8dfa54e

RUN find $SRC/sqlite3 -name "*.test" | xargs zip $SRC/ossfuzz_seed_corpus.zip

WORKDIR sqlite3
COPY build.sh *.dict *.options $SRC/

COPY new_fuzzers/ $SRC/new_fuzzers/
COPY addon.sh $SRC/
RUN cat $SRC/addon.sh >> $SRC/build.sh
