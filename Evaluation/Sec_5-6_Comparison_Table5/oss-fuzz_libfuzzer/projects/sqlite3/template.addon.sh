new_fuzzers="{{fnames}}"
{% if is_try %}
log_f="$SRC/new_fuzzer_log.txt"
suc_new_dir="$SRC/succ_new_fuzzers"
mkdir -p $suc_new_dir
set +ex
for new_f in $new_fuzzers; do
    f="$SRC/sqlite3/test/${new_f}.c"
    cp "$SRC/new_fuzzers/${new_f}.cpp" $f

    $CC $CFLAGS -I. -c \
        $f -o $SRC/sqlite3/test/${new_f}.o \
        >> $log_f 2>&1
    if [ $? -ne 0 ]; then
        echo "Failed to compile $new_f"
        continue
    fi

    $CXX $CXXFLAGS \
        $SRC/sqlite3/test/${new_f}.o -o ${suc_new_dir}/${new_f}\
        $LIB_FUZZING_ENGINE ./sqlite3.o
    if [ $? -ne 0 ]; then
        echo "Failed to link $new_f"
        continue
    fi
done
echo ""
echo "==== Make log ===="
cat $log_f
echo ""
echo "=========="
echo "WORKING_HANRESSES=$(ls $suc_new_dir/| tr '\n' ' ')"
echo "=========="
echo ""
{% else %}
for new_f in $new_fuzzers; do
    f="$SRC/sqlite3/test/${new_f}.c"
    cp "$SRC/new_fuzzers/${new_f}.cpp" $f

    $CC $CFLAGS -I. -c \
        $f -o $SRC/sqlite3/test/${new_f}.o
    if [ $? -ne 0 ]; then
        echo "Failed to compile $new_f"
        continue
    fi

    $CXX $CXXFLAGS \
        $SRC/sqlite3/test/${new_f}.o -o $OUT/${new_f}\
        $LIB_FUZZING_ENGINE ./sqlite3.o
    if [ $? -ne 0 ]; then
        echo "Failed to link $new_f"
        continue
    fi
done
{% endif %}

