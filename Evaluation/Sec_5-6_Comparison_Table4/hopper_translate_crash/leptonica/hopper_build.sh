#!/bin/bash -eu
./autogen.sh
./configure \
  --enable-shared \
  --with-libpng \
  --with-zlib \
  --with-jpeg \
  --with-libwebp \
  --with-libtiff 
make -j$(nproc)

