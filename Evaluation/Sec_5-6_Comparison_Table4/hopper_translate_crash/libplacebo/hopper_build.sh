#!/bin/bash -eu

git submodule update --init

rm -rf build

meson setup -Ddefault_library=shared build

ninja -C build 

