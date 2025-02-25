new_fuzzers="libaom__aom_codec_encode__0 libaom__aom_codec_encode__1 libaom__aom_img_add_metadata__0 libaom__aom_img_plane_height__0 libaom__aom_img_plane_width__0 libaom__aom_img_remove_metadata__0 libaom__aom_img_remove_metadata__1 libaom__aom_img_wrap__1"

for new_f in $new_fuzzers; do
  cp $SRC/new_fuzzers/${new_f}.cpp $SRC/aom/examples/

  $CXX $CXXFLAGS -std=c++11 \
    -I$SRC/aom \
    -I${build_dir} \
    -Wl,--start-group \
    $LIB_FUZZING_ENGINE \
    $SRC/aom/examples/${new_f}.cpp -o $OUT/${new_f} \
    ${build_dir}/libaom.a -Wl,--end-group

    if [ $? -ne 0 ]; then
        echo "Failed to compile $new_f"
        continue
    fi
  cp $SRC/dec_fuzzer_seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
  cp $SRC/aom/examples/av1_dec_fuzzer.dict $OUT/${new_f}.dict
done

