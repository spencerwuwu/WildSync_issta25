#!/bin/bash -eu

rm -rf build && mkdir build
cd build

cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release ../

make 
