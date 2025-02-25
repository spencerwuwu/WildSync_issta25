new_fuzzers="c-ares__ares_getsock__0 c-ares__ares_process_fd__0 c-ares__ares_process_fd__1 c-ares__ares_process_fd__4 c-ares__ares_process_fd__5 c-ares__ares_process_fd__6"

for new_f in $new_fuzzers; do
    f="$SRC/c-ares/test/${new_f}.c"
    cp "$SRC/new_fuzzers/${new_f}.cpp" $f

    $CC $CFLAGS -Iinclude -Isrc/lib -c $f -o $WORK/${new_f}.o
    if [ $? -ne 0 ]; then
        echo "Failed to compile $new_f"
        continue
    fi
    $CXX $CXXFLAGS -std=c++11 $WORK/${new_f}.o \
        -o $OUT/${new_f} \
        $LIB_FUZZING_ENGINE $SRC/c-ares/src/lib/.libs/libcares.a
    if [ $? -ne 0 ]; then
        echo "Failed to compile $new_f"
        continue
    fi
done
