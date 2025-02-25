
OUT=$OUT/hopper

rm -rf $OUT
mkdir -p $OUT

set +e
for round_path in "$SRC/cjson/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
        mkdir -p $out_path
        if [ ! "$(ls -A $path)" ]; then
            continue
        fi
        for cfile in "$path"/*.c; do
            out_name=$(basename -s .c $cfile)
            $CC $CFLAGS $cfile -I. \
                -o $out_path/$out_name \
                $SRC/cjson/build/libcjson.a &> /home/log
            if [ $? -ne 0 ]; then
                $SRC/hopper_helper.py $cfile /home/log
                $CC $CFLAGS $cfile -I. \
                    -o $out_path/$out_name \
                    $SRC/cjson/build/libcjson.a 
            fi
        done
    done
done
true
