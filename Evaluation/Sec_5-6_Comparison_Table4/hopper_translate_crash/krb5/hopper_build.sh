#!/bin/bash -eu

pushd src/
autoreconf
./configure CFLAGS="-fcommon $CFLAGS" CXXFLAGS="-fcommon $CXXFLAGS" \
    --enable-shared 
make
popd
