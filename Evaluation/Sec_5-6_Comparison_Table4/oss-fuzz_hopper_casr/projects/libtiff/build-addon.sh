
new_fuzzers="libtiff__TIFFCheckpointDirectory__0 libtiff__TIFFCheckpointDirectory__1 libtiff__TIFFCheckpointDirectory__2 libtiff__TIFFCheckpointDirectory__3 libtiff__TIFFClientdata__0 libtiff__TIFFClientdata__1 libtiff__TIFFClientdata__2 libtiff__TIFFClientdata__5 libtiff__TIFFClientdata__6 libtiff__TIFFClientdata__7 libtiff__TIFFCurrentDirectory__0 libtiff__TIFFCurrentDirectory__1 libtiff__TIFFCurrentDirectory__2 libtiff__TIFFCurrentDirectory__3 libtiff__TIFFCurrentDirectory__4 libtiff__TIFFCurrentDirectory__5 libtiff__TIFFCurrentDirectory__6 libtiff__TIFFCurrentDirectory__7 libtiff__TIFFDefaultStripSize__0 libtiff__TIFFDefaultStripSize__1 libtiff__TIFFDefaultStripSize__2 libtiff__TIFFDefaultStripSize__5 libtiff__TIFFDefaultStripSize__6 libtiff__TIFFDefaultTileSize__3 libtiff__TIFFDeferStrileArrayWriting__0 libtiff__TIFFFlushData__0 libtiff__TIFFFlush__0 libtiff__TIFFForceStrileArrayWriting__0 libtiff__TIFFGetSizeProc__0 libtiff__TIFFGetSizeProc__1 libtiff__TIFFMergeFieldInfo__1 libtiff__TIFFNumberOfDirectories__0 libtiff__TIFFNumberOfDirectories__1 libtiff__TIFFNumberOfDirectories__2 libtiff__TIFFRasterScanlineSize__0 libtiff__TIFFRewriteDirectory__0 libtiff__TIFFVStripSize__2 libtiff__TIFFVStripSize__4"


for new_f in $new_fuzzers; do
    $CXX $CXXFLAGS -std=c++11 -I$WORK/include \
        $SRC/libtiff/contrib/oss-fuzz/${new_f}.cpp -o $OUT/${new_f} \
        $LIB_FUZZING_ENGINE $WORK/lib/libtiffxx.a $WORK/lib/libtiff.a $WORK/lib/libz.a $WORK/lib/libjpeg.a \
        $WORK/lib/libjbig.a $WORK/lib/libjbig85.a -Wl,-Bstatic -llzma -Wl,-Bdynamic
    cp tif.zip "$OUT/${new_f}_seed_corpus.zip"
    cp "$SRC/tiff.dict" "$OUT/${new_f}.dict"
done
