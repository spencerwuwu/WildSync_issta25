#!/bin/bash -eu


mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON -DENABLE_TOOLS=OFF ..
make

