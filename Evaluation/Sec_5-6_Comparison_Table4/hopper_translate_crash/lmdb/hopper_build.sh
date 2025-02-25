#!/bin/bash -eu

cd libraries/liblmdb
sed -i '26 s/CFLAGS	=/CFLAGS	+=/' ./Makefile
sed -i '21d' ./Makefile

make -j$(nproc)

