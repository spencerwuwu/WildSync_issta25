#!/bin/bash -eu
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
mkdir json-c-build
cd json-c-build
cmake -DBUILD_SHARED_LIBS=OFF ..
make -j$(nproc)
cd ..

cp $SRC/*.dict $OUT/

for f in $SRC/*_fuzzer.cc; do
    fuzzer=$(basename "$f" _fuzzer.cc)
    $CXX $CXXFLAGS -std=c++11 -I$SRC/json-c -I$SRC/json-c/json-c-build\
         $SRC/${fuzzer}_fuzzer.cc -o $OUT/${fuzzer}_fuzzer \
         $LIB_FUZZING_ENGINE $SRC/json-c/json-c-build/libjson-c.a
done

new_fuzzers="json-c__json_object_array_del_idx__0 json-c__json_object_array_get_idx__0 json-c__json_object_array_get_idx__1 json-c__json_object_array_get_idx__10 json-c__json_object_array_get_idx__2 json-c__json_object_array_get_idx__3 json-c__json_object_array_get_idx__4 json-c__json_object_array_get_idx__5 json-c__json_object_array_get_idx__6 json-c__json_object_array_get_idx__7 json-c__json_object_array_get_idx__8 json-c__json_object_array_get_idx__9 json-c__json_object_array_length__0 json-c__json_object_array_length__1 json-c__json_object_array_length__10 json-c__json_object_array_length__2 json-c__json_object_array_length__3 json-c__json_object_array_length__4 json-c__json_object_array_length__5 json-c__json_object_array_length__6 json-c__json_object_array_length__7 json-c__json_object_array_length__8 json-c__json_object_array_length__9 json-c__json_object_get_array__0 json-c__json_object_get_array__1 json-c__json_object_get_array__10 json-c__json_object_get_array__2 json-c__json_object_get_array__3 json-c__json_object_get_array__4 json-c__json_object_get_array__5 json-c__json_object_get_array__6 json-c__json_object_get_array__7 json-c__json_object_get_array__8 json-c__json_object_get_array__9 json-c__json_object_get_boolean__0 json-c__json_object_get_boolean__4 json-c__json_object_get_boolean__6 json-c__json_object_get_boolean__7 json-c__json_object_get_double__0 json-c__json_object_get_double__1 json-c__json_object_get_double__2 json-c__json_object_get_double__3 json-c__json_object_get_double__5 json-c__json_object_get_int64__1 json-c__json_object_get_int64__2 json-c__json_object_get_int64__3 json-c__json_object_get_int64__4 json-c__json_object_get_int64__5 json-c__json_object_get_int64__6 json-c__json_object_get_int64__7 json-c__json_object_get_int__0 json-c__json_object_get_int__10 json-c__json_object_get_int__3 json-c__json_object_get_int__4 json-c__json_object_get_int__5 json-c__json_object_get_int__6 json-c__json_object_get_int__7 json-c__json_object_get_int__8 json-c__json_object_get_int__9 json-c__json_object_get_string__8 json-c__json_object_object_del__0 json-c__json_object_object_get_ex__0 json-c__json_object_object_get_ex__10 json-c__json_object_object_get_ex__4 json-c__json_object_object_get_ex__8 json-c__json_object_to_json_string_ext__7"
new_fuzzers+=" json-c__json_object_array_del_idx__0__manual0 json-c__json_object_array_get_idx__9__manual0 json-c__json_object_array_length__0__manual0 json-c__json_object_object_del__0__manual0"


for new_f in $new_fuzzers; do
    $CXX $CXXFLAGS -std=c++11 -I$SRC/json-c -I$SRC/json-c/json-c-build\
         $SRC/${new_f}.cpp -o $OUT/${new_f} \
         $LIB_FUZZING_ENGINE $SRC/json-c/json-c-build/libjson-c.a
done
