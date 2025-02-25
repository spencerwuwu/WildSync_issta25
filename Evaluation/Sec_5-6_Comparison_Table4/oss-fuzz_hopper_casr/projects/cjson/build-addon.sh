
OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC


set +e
path="$SRC/cjson/hopper_results"
out_path="$OUT"
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
true
