#!/bin/bash -eu

mkdir build
cd build

cmake -DBUILD_SHARED_LIBS=ON ..
make -j$(nproc)

