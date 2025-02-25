#!/bin/bash -eu

export WORK=/home/work
mkdir -p $WORK
PREFIX=$WORK/prefix

rm -rf build && mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$PREFIX -DBUILD_SHARED_LIBS=ON ..

make -j$(nproc) 
