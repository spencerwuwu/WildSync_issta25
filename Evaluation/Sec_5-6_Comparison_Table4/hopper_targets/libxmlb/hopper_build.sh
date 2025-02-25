#!/bin/bash -eu

export CFLAGS="$CFLAGS -gdwarf-4     -fPIC"
export CXXFLAGS="$CXXFLAGS -gdwarf-4 -fPIC"

export WORK=/home/work
mkdir -p $WORK

PREFIX=$WORK/prefix

# build libxmlb
meson \
    --prefix=$PREFIX \
    --default-library=shared \
    -Dintrospection=false \
    -Dgtkdoc=false \
    -Dtests=false \
    -Dstemmer=false \
    -Dlzma=disabled \
    build
ninja -C build
