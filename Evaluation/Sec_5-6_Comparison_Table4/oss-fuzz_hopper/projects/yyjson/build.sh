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

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/yyjson/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
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
    done
done

true
