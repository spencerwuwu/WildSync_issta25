
new_fuzzers="vorbis__ov_comment__0 vorbis__ov_comment__2 vorbis__ov_comment__3 vorbis__ov_pcm_seek__0 vorbis__ov_pcm_seek__1 vorbis__ov_pcm_tell__0 vorbis__ov_pcm_total__0 vorbis__ov_pcm_total__1 vorbis__ov_pcm_total__10 vorbis__ov_pcm_total__2 vorbis__ov_pcm_total__3 vorbis__ov_pcm_total__4 vorbis__ov_pcm_total__5 vorbis__ov_pcm_total__7 vorbis__ov_pcm_total__8 vorbis__ov_pcm_total__9 vorbis__ov_raw_seek__0 vorbis__ov_raw_seek__1 vorbis__ov_seekable__0"

for new_f in $new_fuzzers; do
    $CXX $CXXFLAGS "$SRC/vorbis/contrib/oss-fuzz/${new_f}.cpp" -o $OUT/$new_f -L"$WORK/lib" -I"$WORK/include" $LIB_FUZZING_ENGINE -lvorbisfile -lvorbis -logg
    cp "$OUT/decode_fuzzer_seed_corpus.zip" "$OUT/${new_f}_seed_corpus.zip"
done

