#!/bin/bash -eu

rm -rf build
mkdir build
cd build
#cmake -DBUILD_SHARED_LIBS=OFF -DENABLE_CJSON_TEST=OFF ..
cmake -DENABLE_CJSON_TEST=OFF ..
make -j$(nproc)
