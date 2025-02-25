new_fuzzers="{{fnames}}"
{% if is_try %}
log_f="$SRC/new_fuzzer_log.txt"
suc_new_dir="$SRC/succ_new_fuzzers"
mkdir -p $suc_new_dir
set +ex
for new_f in $new_fuzzers; do
  for decoder in "${fuzzer_decoders[@]}"; do
    cp $SRC/new_fuzzers/${new_f}.cpp $SRC/libvpx/examples/
    fuzzer_name=${new_f}"_"${decoder}

    $CXX $CXXFLAGS -std=c++11 \
        -DDECODER=${decoder} \
        -I$SRC/libvpx \
        -I${build_dir} \
        -Wl,--start-group \
        $LIB_FUZZING_ENGINE \
        $SRC/libvpx/examples/${new_f}.cpp -o $suc_new_dir/${fuzzer_name} \
        ${build_dir}/libvpx.a \
        -Wl,--end-group \
        >> $log_f 2>&1
    if [ $? -ne 0 ]; then
        echo "Failed to compile $fuzzer_name"
        continue
    fi
  done
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
{% endif %}


