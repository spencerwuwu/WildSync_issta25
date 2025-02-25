#!/bin/bash -eu

sed -i 's/-Wall//g' ./CMakeLists.txt
sed -i 's/-Werror//g' ./CMakeLists.txt

mkdir build2
cd build2
cmake -DCHECK_CRC_ON_SOLID_SKIP=1 -DDONT_FAIL_ON_CRC_ERROR=1 ../
make -j$(nproc)
