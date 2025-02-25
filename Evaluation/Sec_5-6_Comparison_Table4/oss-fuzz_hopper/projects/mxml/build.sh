#!/bin/bash -eu


export LDFLAGS=$CFLAGS
./configure
make libmxml.a

#$CC $CFLAGS $LIB_FUZZING_ENGINE $SRC/fuzz_loadfile.c -I./ ./libmxml.a \
#    -o $OUT/fuzz_loadfile
#for new_f in $new_fuzzers; do
#  cp $SRC/new_fuzzers/${new_f}.cpp $SRC/${new_f}.c
#  echo "==== Compiling $new_f ===="
#done

###################################

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/mxml/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
        mkdir -p $out_path
        if [ ! "$(ls -A $path)" ]; then
            continue
        fi
        for cfile in "$path"/*.c; do
            out_name=$(basename -s .c $cfile)
            $CC $CFLAGS -I./ \
                $cfile -o $out_path/$out_name \
              ./libmxml.a -pthread &> /home/log
            if [ $? -ne 0 ]; then
                $SRC/hopper_helper.py $cfile /home/log
                $CC $CFLAGS -I./ \
                    $cfile -o $out_path/$out_name \
                  ./libmxml.a -pthread 
            fi
        done
    done
done

true
