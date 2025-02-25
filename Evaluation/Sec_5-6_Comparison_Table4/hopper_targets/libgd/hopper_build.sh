#!/bin/bash -eu

export WORK=$(pwd)/build
mkdir -p $WORK

sed -i 's/-Werror//g' ./configure.ac
./bootstrap.sh

# Limit the size of buffer allocations to avoid bogus OOM issues
# https://github.com/libgd/libgd/issues/422
sed -i'' -e 's/INT_MAX/100000/' "/home/libgd/src/gd_security.c"

./configure --prefix="$WORK" --enable-shared
make -j$(nproc)

