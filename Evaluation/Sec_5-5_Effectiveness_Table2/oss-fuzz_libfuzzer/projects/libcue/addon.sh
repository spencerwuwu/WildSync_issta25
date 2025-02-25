

new_fuzzers="libcue__cd_get_cdtext__0 libcue__cd_get_ntrack__0 libcue__cd_get_ntrack__1 libcue__cd_get_rem__0 libcue__cd_get_track__0 libcue__cd_get_track__1 libcue__cd_get_track__2"

for new_f in $new_fuzzers; do
  cp ${new_f} $OUT/${new_f}
  cp $OUT/${fuzzer_name}_seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
  cp $SRC/libcue/oss-fuzz/cue.dict $OUT/${new_f}.dict
    echo -en "[libfuzzer]\nclose_fd_mask=2\n" >$OUT/${new_f}.options
done
