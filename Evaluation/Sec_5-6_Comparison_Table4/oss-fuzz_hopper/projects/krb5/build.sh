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

OUT=$OUT/hopper
mkdir -p $OUT

rm -rf $OUT

set +e
for round_path in "$SRC/krb5/hopper_results"/*; do
    for queue in queue crashes; do
        path="$round_path/$queue/"
        out_path="$OUT/$(basename $round_path)/$queue"
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
    done
done

true

