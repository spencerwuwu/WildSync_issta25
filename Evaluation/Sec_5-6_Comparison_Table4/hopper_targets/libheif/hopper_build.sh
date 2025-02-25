#!/bin/bash -eu

mkdir build
cd build
cmake .. \
      -DWITH_DEFLATE_HEADER_COMPRESSION=OFF

make -j$(nproc)
