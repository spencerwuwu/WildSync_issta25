#!/bin/bash -eu

#cmake . -DCMAKE_INSTALL_PREFIX=$WORK -DBUILD_SHARED_LIBS=off
cmake . -DBUILD_SHARED_LIBS=on
make -j$(nproc)
#make install

