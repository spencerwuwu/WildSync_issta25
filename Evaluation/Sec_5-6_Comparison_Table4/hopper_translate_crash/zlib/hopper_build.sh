#!/bin/bash -eu

./configure
make -j$(nproc) clean
make -j$(nproc) all

