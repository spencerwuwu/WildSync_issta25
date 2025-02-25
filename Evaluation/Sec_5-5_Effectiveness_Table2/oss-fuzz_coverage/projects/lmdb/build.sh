#!/bin/bash -eu

cd libraries/liblmdb
sed -i '26 s/CFLAGS	=/CFLAGS	+=/' ./Makefile
sed -i '21d' ./Makefile

make -j$(nproc)
$CC $CXXFLAGS -I. -c $SRC/lmdb_fuzzer.c -o lmdb_fuzzer.o
$CC $CXXFLAGS $LIB_FUZZING_ENGINE lmdb_fuzzer.o \
    $(find $SRC -name liblmdb.a) -o $OUT/lmdb_fuzzer

new_fuzzers="lmdb__mdb_cursor_count__1 lmdb__mdb_cursor_dbi__0 lmdb__mdb_cursor_dbi__4 lmdb__mdb_cursor_dbi__5 lmdb__mdb_cursor_dbi__7 lmdb__mdb_cursor_dbi__9 lmdb__mdb_cursor_del__0 lmdb__mdb_cursor_txn__0 lmdb__mdb_cursor_txn__2 lmdb__mdb_cursor_txn__3 lmdb__mdb_dbi_flags__0 lmdb__mdb_drop__1 lmdb__mdb_drop__2 lmdb__mdb_env_copy__0 lmdb__mdb_env_get_fd__0 lmdb__mdb_env_get_maxkeysize__1 lmdb__mdb_env_get_path__0 lmdb__mdb_env_info__0 lmdb__mdb_env_info__1 lmdb__mdb_env_info__2 lmdb__mdb_env_sync__1 lmdb__mdb_env_sync__2 lmdb__mdb_txn_renew__1 lmdb__mdb_txn_renew__2"


for new_f in $new_fuzzers; do
  cp $SRC/new_fuzzers/${new_f}.cpp $SRC/${new_f}.c
  $CC $CXXFLAGS -I. -c $SRC/${new_f}.c -o ${new_f}.o
  $CC $CXXFLAGS $LIB_FUZZING_ENGINE ${new_f}.o \
      $(find $SRC -name liblmdb.a) -o $OUT/${new_f}
done
