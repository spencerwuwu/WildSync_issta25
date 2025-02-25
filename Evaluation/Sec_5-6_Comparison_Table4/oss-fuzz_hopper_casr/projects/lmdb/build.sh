#!/bin/bash -eu

cd libraries/liblmdb
sed -i '26 s/CFLAGS	=/CFLAGS	+=/' ./Makefile
sed -i '21d' ./Makefile

make -j$(nproc)
#$CC $CXXFLAGS -I. -c $SRC/lmdb_fuzzer.c -o lmdb_fuzzer.o
#$CC $CXXFLAGS $LIB_FUZZING_ENGINE lmdb_fuzzer.o \
#    $(find $SRC -name liblmdb.a) -o $OUT/lmdb_fuzzer
#
#for new_f in $new_fuzzers; do
#  cp $SRC/new_fuzzers/${new_f}.cpp $SRC/${new_f}.c
#  $CC $CXXFLAGS -I. -c $SRC/${new_f}.c -o ${new_f}.o
#  $CC $CXXFLAGS $LIB_FUZZING_ENGINE ${new_f}.o \
#      $(find $SRC -name liblmdb.a) -o $OUT/${new_f}
#done

###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/lmdb/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS -I. \
        $cfile -o $out_path/$out_name \
        $(find $SRC -name liblmdb.a) -lpthread &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS -I. \
            $cfile -o $out_path/$out_name \
            $(find $SRC -name liblmdb.a) -lpthread
    fi
done

true
