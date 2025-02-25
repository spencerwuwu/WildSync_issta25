#!/bin/bash -eu


export LDFLAGS=$CFLAGS
./configure
make libmxml.a
$CC $CFLAGS $LIB_FUZZING_ENGINE $SRC/fuzz_loadfile.c -I./ ./libmxml.a \
    -o $OUT/fuzz_loadfile


new_fuzzers="mxml__mxmlAdd__0 mxml__mxmlAdd__1 mxml__mxmlAdd__2 mxml__mxmlAdd__3 mxml__mxmlAdd__4 mxml__mxmlAdd__5 mxml__mxmlAdd__6 mxml__mxmlElementGetAttrByIndex__0 mxml__mxmlElementGetAttrCount__0 mxml__mxmlFindElement__0 mxml__mxmlFindElement__1 mxml__mxmlFindElement__2 mxml__mxmlFindElement__3 mxml__mxmlFindElement__4 mxml__mxmlFindPath__1 mxml__mxmlFindPath__2 mxml__mxmlFindPath__3 mxml__mxmlFindPath__4 mxml__mxmlFindPath__5 mxml__mxmlGetFirstChild__1 mxml__mxmlGetFirstChild__10 mxml__mxmlGetFirstChild__2 mxml__mxmlGetFirstChild__3 mxml__mxmlGetFirstChild__4 mxml__mxmlGetFirstChild__5 mxml__mxmlGetFirstChild__6 mxml__mxmlGetFirstChild__7 mxml__mxmlGetFirstChild__8 mxml__mxmlGetFirstChild__9 mxml__mxmlGetLastChild__0 mxml__mxmlGetLastChild__1 mxml__mxmlGetLastChild__2 mxml__mxmlGetNextSibling__10 mxml__mxmlGetNextSibling__3 mxml__mxmlGetNextSibling__4 mxml__mxmlGetNextSibling__5 mxml__mxmlGetNextSibling__6 mxml__mxmlGetNextSibling__7 mxml__mxmlGetNextSibling__8 mxml__mxmlGetNextSibling__9 mxml__mxmlGetParent__0 mxml__mxmlGetParent__1 mxml__mxmlGetParent__2 mxml__mxmlGetParent__3 mxml__mxmlGetParent__4 mxml__mxmlGetParent__5 mxml__mxmlGetParent__7 mxml__mxmlGetParent__9 mxml__mxmlGetText__1 mxml__mxmlRemove__0 mxml__mxmlRemove__1 mxml__mxmlRemove__2 mxml__mxmlRemove__3 mxml__mxmlRemove__4 mxml__mxmlRemove__5 mxml__mxmlRemove__6 mxml__mxmlRemove__7 mxml__mxmlRemove__8 mxml__mxmlRemove__9 mxml__mxmlSaveAllocString__0 mxml__mxmlSaveAllocString__1 mxml__mxmlSaveAllocString__2 mxml__mxmlSaveAllocString__3 mxml__mxmlSaveAllocString__4 mxml__mxmlSaveAllocString__5"


for new_f in $new_fuzzers; do
  cp $SRC/new_fuzzers/${new_f}.cpp $SRC/${new_f}.c
  echo "==== Compiling $new_f ===="
  $CC $CFLAGS $LIB_FUZZING_ENGINE $SRC/${new_f}.c -I./ ./libmxml.a \
    -o $OUT/${new_f}
done
