#!/bin/bash -eu

export CFLAGS="$CFLAGS -gdwarf-4     -fPIC"
export CXXFLAGS="$CXXFLAGS -gdwarf-4 -fPIC"

export WORK=/home/work
mkdir -p $WORK

PREFIX=$WORK/prefix
mkdir -p $PREFIX

export PKG_CONFIG="`which pkg-config` --static"
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
export PATH=$PREFIX/bin:$PATH

## build zlib
pushd "$SRC/zlib"
./configure --static --prefix=$PREFIX
make -j$(nproc) CFLAGS="$CFLAGS -fPIC"
make install
popd


# Build png
pushd "$SRC/libpng"
cmake . -DCMAKE_INSTALL_PREFIX=$PREFIX -DPNG_SHARED=off
make -j$(nproc)
make install
popd


# make libharu
rm -rf build && mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$PREFIX -DBUILD_SHARED_LIBS=OFF ..

make -j$(nproc) install


DEPS="libpng"

PREDEPS_LDFLAGS="-Wl,-Bdynamic -ldl -lm -lc -pthread -lrt -lpthread"

BUILD_CFLAGS="$CFLAGS `pkg-config --static --cflags $DEPS`"
BUILD_LDFLAGS="-Wl,-static `pkg-config --static --libs $DEPS`"

###################################

OUT=/home/crash_build
mkdir -p $OUT

OUTSRC=$OUT/hopper-srcs/
OUT=$OUT/hopper-builds/

#rm -rf $OUT
mkdir $OUTSRC

set +e
path="$SRC/libharu/hopper_results"
out_path="$OUT"
mkdir -p $out_path
if [ ! "$(ls -A $path)" ]; then
    continue
fi
for cfile in "$path"/*.c; do
    out_name=$(basename -s .c $cfile)
    $CC $BUILD_CFLAGS \
        -c $cfile -o \
        /tmp/$out_name &> /home/log 

    if [ $? -ne 0 ]; then
       $SRC/hopper_helper.py $cfile /home/log
       $CC $BUILD_CFLAGS \
           -c $cfile -o \
           /tmp/$out_name
    fi

    if [ ! -f /tmp/$out_name ]; then
        continue
    fi

    $CXX $CXXFLAGS \
        /tmp/$out_name -o $out_path/$out_name \
        $PREDEPS_LDFLAGS \
        -L $PREFIX/lib  -lhpdf \
        $BUILD_LDFLAGS \
        -Wl,-Bdynamic
done

true
