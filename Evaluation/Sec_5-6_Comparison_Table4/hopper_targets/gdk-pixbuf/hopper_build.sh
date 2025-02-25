#!/bin/bash -eu

meson \
    --libdir=lib \
    --default-library=shared \
    -Dintrospection=disabled \
    -Dgtk_doc=false \
    -Dman=false \
    -Ddocs=false \
    -Dbuiltin_loaders='all' \
    build
ninja -C build

touch glibconfig.h
