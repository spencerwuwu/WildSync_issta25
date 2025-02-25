#!/bin/bash -eu

# build project
mkdir build
cd build
cmake -DWITH_TESTS=OFF  -DWITH_ZLIB=OFF -DBUILD_SHARED_LIBS=ON ../
cmake --build .
