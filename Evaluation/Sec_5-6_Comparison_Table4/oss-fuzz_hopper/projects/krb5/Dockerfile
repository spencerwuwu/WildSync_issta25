# Copyright 2022 Google LLC
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
RUN apt-get update && apt-get install -y autoconf bison pkg-config

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/krb5/krb5.git
RUN git -C krb5 checkout 2063e72ca296a55768c071fb70727ff4688c3496
#RUN cp $SRC/krb5/src/tests/fuzzing/oss-fuzz.sh $SRC/build.sh
WORKDIR $SRC/krb5/
COPY build.sh $SRC

COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/krb5/hopper_results/

RUN bash $SRC/build.sh

#COPY add.configure.ac $SRC

#RUN mkdir -p $SRC/krb5/src/tests/new_fuzzing/
#COPY Makefile.in $SRC/krb5/src/tests/new_fuzzing/
#COPY new.configure.ac $SRC/krb5/src/configure.ac
#COPY new_fuzzers $SRC/new_fuzzers
#COPY deps $SRC/krb5/src/tests/new_fuzzing/
