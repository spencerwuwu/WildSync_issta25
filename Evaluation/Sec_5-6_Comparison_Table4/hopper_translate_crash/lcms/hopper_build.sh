#!/bin/bash -eu

# build the target.
./configure --enable-shared=yes
make -j$(nproc) all

