#!/bin/bash -eu

CFLAGS="-DDEBUG_SVG_RENDER $CFLAGS" meson \
    --libdir=lib \
    --default-library=shared \
    build

ninja -C build
touch src/cairo-features.h

