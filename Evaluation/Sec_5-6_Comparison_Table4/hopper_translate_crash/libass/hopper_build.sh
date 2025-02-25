#!/bin/bash -eu

./autogen.sh
./configure --disable-asm
make -j "$(nproc)" 

