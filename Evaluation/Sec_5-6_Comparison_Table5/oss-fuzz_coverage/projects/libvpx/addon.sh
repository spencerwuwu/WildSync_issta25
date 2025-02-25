new_fuzzers=""

for new_f in $new_fuzzers; do
    cp $SRC/new_fuzzers/${new_f}.cpp $SRC/libvpx/examples/
    fuzzer_name=${new_f}"_"${decoder}

    $CXX $CXXFLAGS -std=c++11 \
        -DDECODER=${decoder} \
        -I$SRC/libvpx \
        -I${build_dir} \
        -Wl,--start-group \
        $LIB_FUZZING_ENGINE \
        $SRC/libvpx/examples/${new_f}.cpp -o $OUT/${fuzzer_name} \
        ${build_dir}/libvpx.a \
        -Wl,--end-group 
  cp $SRC/vpx_fuzzer_seed_corpus.zip $OUT/${fuzzer_name}_seed_corpus.zip
  cp $SRC/vpx_dec_fuzzer.dict $OUT/${fuzzer_name}.dict
done


