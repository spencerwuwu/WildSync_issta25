#!/bin/bash -eu

# Compile krb5 for oss-fuzz.
pushd src/
autoreconf
./configure CFLAGS="-fcommon $CFLAGS" CXXFLAGS="-fcommon $CXXFLAGS" \
    --enable-static --disable-shared #--enable-ossfuzz
make
make install
popd

###################################

export BUILD_CFLAGS="$(pkg-config --static --cflags krb5 kadm-server kadm-client kdb krb5-gssapi mit-krb5 mit-krb5-gssapi gssrpc)"
export BUILD_LDFLAGS="-Wl,-static $(pkg-config --static --libs  krb5 kadm-server kadm-client kdb krb5-gssapi mit-krb5 mit-krb5-gssapi gssrpc)"

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/krb5/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $CFLAGS \
      $BUILD_CFLAGS \
      $cfile -o $out_path/$out_name \
      $BUILD_LDFLAGS -Wl,-Bdynamic &> /home/log
    if [ $? -ne 0 ]; then
        $SRC/hopper_helper.py $cfile /home/log
        $CC $CFLAGS \
          $BUILD_CFLAGS \
          $cfile -o $out_path/$out_name \
          $BUILD_LDFLAGS -Wl,-Bdynamic
    fi
done

true

