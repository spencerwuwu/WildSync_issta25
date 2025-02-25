#!/bin/bash -eu

# Build glib
pushd $SRC/glib/
meson \
    setup \
    --libdir=lib \
    --default-library=static \
    -Db_lundef=false \
    -Dlibmount=disabled \
    _builddir
ninja -C _builddir
ninja -C _builddir install
popd

pushd $SRC/libsoup/
meson setup \
    --default-library=shared \
    -Dvapi=disabled        \
    -Dgssapi=disabled      \
    -Dsysprof=disabled     \
    --wrap-mode=nofallback \
    -Dtls_check=false \
    -Dntlm=disabled \
    -Dintrospection=disabled \
     build
ninja -C build 
popd

