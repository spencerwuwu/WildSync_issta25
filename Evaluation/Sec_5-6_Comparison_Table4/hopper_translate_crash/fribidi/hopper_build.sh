#!/bin/bash -eu

export WORK=$(pwd)

# setup
build=$WORK/build

# cleanup
rm -rf $build
mkdir -p $build

# Build the library.
meson --default-library=shared --wrap-mode=nodownload -Ddocs=false \
      $build \
  || (cat build/meson-logs/meson-log.txt && false)

# Build the fuzzers.
ninja -v -j$(nproc) -C $build 
