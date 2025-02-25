#!/bin/bash -eu

./autogen.sh
./configure --prefix="$WORK"
make clean
make -j$(nproc)

