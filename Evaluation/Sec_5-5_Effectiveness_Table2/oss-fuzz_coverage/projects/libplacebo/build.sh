#!/bin/bash -eu
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

git submodule update --init

rm -rf build

echo "option('fuzzer_ldflags', type : 'string', description: 'Extra LDFLAGS used during linking of fuzzing binaries')" >> meson_options.txt

new_fuzzers="libplacebo__pl_find_named_fmt__0 libplacebo__pl_find_named_fmt__1 libplacebo__pl_find_named_fmt__2 libplacebo__pl_gpu_finish__0 libplacebo__pl_gpu_finish__1 libplacebo__pl_opengl_get__0 libplacebo__pl_plane_find_fmt__0"

for new_f in $new_fuzzers; do
  cp $SRC/new_fuzzers/${new_f}.cpp src/tests/fuzz/${new_f}.c
done

meson setup -Dfuzzer_ldflags="$(echo $LIB_FUZZING_ENGINE_DEPRECATED)" -Ddefault_library=static -Dfuzz=true build

fuzzers="lut options shaders user_shaders"
for f in $fuzzers; do
    ninja -v -C build "src/fuzz.${f}.c"
    cp "build/src/fuzz.${f}.c" $OUT/${f}
done




for new_f in $new_fuzzers; do
  ninja -v -C build "src/fuzz.${new_f}"
  mv "build/src/fuzz.${new_f}" $OUT/${new_f}
done

