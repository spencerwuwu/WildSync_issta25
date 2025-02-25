#!/bin/bash -eu

mkdir build && cd build
cmake ..
make -j$(nproc)

#$CC $CFLAGS -I/src/yyjson/src \
#	-c $SRC/fuzzer.c \
#	-o fuzzer.o
#$CC $CFLAGS $LIB_FUZZING_ENGINE \
#	fuzzer.o -o $OUT/fuzzer \
#    ./CMakeFiles/yyjson.dir/src/yyjson.c.o
#
###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/yyjson/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS -I/src/yyjson/src \
        $cfile -o $out_path/$out_name \
        ./CMakeFiles/yyjson.dir/src/yyjson.c.o &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -I/src/yyjson/src \
            $cfile -o $out_path/$out_name \
            ./CMakeFiles/yyjson.dir/src/yyjson.c.o
    fi
done

true
