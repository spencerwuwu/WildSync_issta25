
new_fuzzers="leptonica__l_generateCIDataForPdf__0 leptonica__pixAddGaussianNoise__0 leptonica__pixClearInRect__1 leptonica__pixClearInRect__2 leptonica__pixClipBoxToForeground__1 leptonica__pixForegroundFraction__0 leptonica__pixRemoveAlpha__0 leptonica__pixRenderBoxArb__0 leptonica__pixRenderBoxArb__1 leptonica__pixRenderBoxArb__2 leptonica__pixRenderPolyline__0 leptonica__pixRenderPolyline__1 leptonica__pixaClear__0"
new_fuzzers+=" leptonica__l_generateCIDataForPdf__0__tweak0"

for new_f in $new_fuzzers; do
    cp $SRC/new_fuzzers/${new_f}.cpp $SRC/leptonica/prog/fuzzing/
    $CXX $CXXFLAGS -std=c++11 -I"$WORK/include" \
      $SRC/leptonica/prog/fuzzing/${new_f}.cpp -o $OUT/${new_f} \
      -Isrc/ \
      "$WORK/lib/libleptonica.a" \
      "$WORK/lib/libtiff.a" \
      "$WORK/lib/libwebp.a" \
      "$WORK/lib/libpng.a" \
      "$WORK/lib/libjpeg.a" \
      "$WORK/lib/libjbig.a" \
      "$WORK/lib/libzstd.a" \
      "$WORK/lib/libz.a" \
      "$WORK/lib/libsharpyuv.a" \
      $LIB_FUZZING_ENGINE 
    cp $SRC/leptonica/prog/fuzzing/general_corpus.zip "$OUT/${new_f}_seed_corpus.zip"
done

