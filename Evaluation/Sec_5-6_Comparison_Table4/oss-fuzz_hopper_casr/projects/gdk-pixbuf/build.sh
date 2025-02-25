#!/bin/bash -eu
# Copyright 2018 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################
PREFIX=$WORK/prefix
mkdir -p $PREFIX

export PKG_CONFIG="`which pkg-config` --static"
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
export PATH=$PREFIX/bin:$PATH

BUILD=$WORK/build

rm -rf $WORK/*
rm -rf $BUILD
mkdir -p $BUILD

# Build glib
R_CFLAGS=$CFLAGS
R_CXXFLAGS=$CXXFLAGS
export CFLAGS=${CFLAGS//-flto/}
export CFLAGS=${CFLAGS//-fuse-ld=gold/}
export CXXFLAGS=${CXXFLAGS//-flto/}
export CXXFLAGS=${CXXFLAGS//-fuse-ld=gold/}
export FUZZ_INTROSPECTOR=0

pushd $SRC/glib-2.64.2
meson \
    --prefix=$PREFIX \
    --libdir=lib \
    --default-library=static \
    -Db_lundef=false \
    -Doss_fuzz=enabled \
    -Dlibmount=disabled \
    -Dinternal_pcre=true \
    _builddir
ninja -C _builddir
ninja -C _builddir install
popd

# Build gdk-pixbuf
pushd $SRC/gdk-pixbuf
meson \
    --prefix=$PREFIX \
    --libdir=lib \
    --default-library=static \
    -Dintrospection=disabled \
    -Dgtk_doc=false \
    -Dman=false \
    -Ddocs=false \
    -Dbuiltin_loaders='all' \
    _builddir
ninja -C _builddir
ninja -C _builddir install
popd

mv $SRC/{*.zip,*.dict} $OUT

if [ ! -f "${OUT}/gdk-pixbuf_seed_corpus.zip" ]; then
  echo "missing seed corpus"
  exit 1
fi

if [ ! -f "${OUT}/gdk-pixbuf.dict" ]; then
  echo "missing dictionary"
  exit 1
fi

export CFLAGS=$R_CFLAGS
export CXXFLAGS=$R_CXXFLAGS
export FUZZ_INTROSPECTOR=1

PREDEPS_LDFLAGS="-Wl,-Bdynamic -ldl -lm -pthread -lrt -lpthread -lc"
DEPS="gmodule-2.0 glib-2.0 gio-2.0 gobject-2.0 gdk-pixbuf-2.0"
BUILD_CFLAGS="$CFLAGS `pkg-config --static --cflags $DEPS`"
BUILD_LDFLAGS="-Wl,-static `pkg-config --static --libs $DEPS`"



new_fuzzers="gdk-pixbuf__gdk_pixbuf_animation_get_iter__0 gdk-pixbuf__gdk_pixbuf_animation_get_iter__1 gdk-pixbuf__gdk_pixbuf_animation_get_iter__2 gdk-pixbuf__gdk_pixbuf_animation_get_iter__3 gdk-pixbuf__gdk_pixbuf_animation_get_iter__4 gdk-pixbuf__gdk_pixbuf_animation_get_iter__5 gdk-pixbuf__gdk_pixbuf_animation_new_from_stream__0 gdk-pixbuf__gdk_pixbuf_animation_new_from_stream__1 gdk-pixbuf__gdk_pixbuf_apply_embedded_orientation__0 gdk-pixbuf__gdk_pixbuf_apply_embedded_orientation__1 gdk-pixbuf__gdk_pixbuf_apply_embedded_orientation__2 gdk-pixbuf__gdk_pixbuf_apply_embedded_orientation__3 gdk-pixbuf__gdk_pixbuf_apply_embedded_orientation__4 gdk-pixbuf__gdk_pixbuf_apply_embedded_orientation__5 gdk-pixbuf__gdk_pixbuf_apply_embedded_orientation__6 gdk-pixbuf__gdk_pixbuf_copy_area__6 gdk-pixbuf__gdk_pixbuf_copy_area__7 gdk-pixbuf__gdk_pixbuf_copy_area__8 gdk-pixbuf__gdk_pixbuf_fill__0 gdk-pixbuf__gdk_pixbuf_fill__1 gdk-pixbuf__gdk_pixbuf_fill__2 gdk-pixbuf__gdk_pixbuf_fill__3 gdk-pixbuf__gdk_pixbuf_fill__4 gdk-pixbuf__gdk_pixbuf_fill__5 gdk-pixbuf__gdk_pixbuf_fill__6 gdk-pixbuf__gdk_pixbuf_get_colorspace__0 gdk-pixbuf__gdk_pixbuf_get_colorspace__1 gdk-pixbuf__gdk_pixbuf_get_colorspace__2 gdk-pixbuf__gdk_pixbuf_get_colorspace__3 gdk-pixbuf__gdk_pixbuf_get_colorspace__4 gdk-pixbuf__gdk_pixbuf_get_colorspace__5 gdk-pixbuf__gdk_pixbuf_get_colorspace__6 gdk-pixbuf__gdk_pixbuf_get_n_channels__1 gdk-pixbuf__gdk_pixbuf_get_n_channels__10 gdk-pixbuf__gdk_pixbuf_get_n_channels__2 gdk-pixbuf__gdk_pixbuf_get_n_channels__3 gdk-pixbuf__gdk_pixbuf_get_n_channels__4 gdk-pixbuf__gdk_pixbuf_get_n_channels__5 gdk-pixbuf__gdk_pixbuf_get_n_channels__6 gdk-pixbuf__gdk_pixbuf_get_n_channels__7 gdk-pixbuf__gdk_pixbuf_get_n_channels__8 gdk-pixbuf__gdk_pixbuf_get_n_channels__9 gdk-pixbuf__gdk_pixbuf_loader_new_with_mime_type__1 gdk-pixbuf__gdk_pixbuf_loader_new_with_type__0 gdk-pixbuf__gdk_pixbuf_new_from_resource__0 gdk-pixbuf__gdk_pixbuf_new_from_resource__1 gdk-pixbuf__gdk_pixbuf_new_from_resource__2 gdk-pixbuf__gdk_pixbuf_new_from_resource__3 gdk-pixbuf__gdk_pixbuf_new_from_resource__4 gdk-pixbuf__gdk_pixbuf_new_from_resource__5 gdk-pixbuf__gdk_pixbuf_new_from_resource__6 gdk-pixbuf__gdk_pixbuf_new_from_resource__7 gdk-pixbuf__gdk_pixbuf_new_from_stream_finish__0 gdk-pixbuf__gdk_pixbuf_new_subpixbuf__0 gdk-pixbuf__gdk_pixbuf_new_subpixbuf__1 gdk-pixbuf__gdk_pixbuf_new_subpixbuf__2 gdk-pixbuf__gdk_pixbuf_savev__0 gdk-pixbuf__gdk_pixbuf_scale_simple__0 gdk-pixbuf__gdk_pixbuf_scale_simple__1 gdk-pixbuf__gdk_pixbuf_scale_simple__4"
new_fuzzers+=" gdk-pixbuf__gdk_pixbuf_savev__0__tweak0"


for new_f in $new_fuzzers; do
    mv $SRC/fuzz/${new_f}.cpp  $SRC/fuzz/${new_f}.c
    $CC $CFLAGS $BUILD_CFLAGS -c $SRC/fuzz/${new_f}.c -o $WORK/${new_f}.o 
    $CXX $CXXFLAGS \
        $WORK/${new_f}.o -o $OUT/${new_f} \
        $PREDEPS_LDFLAGS \
        $BUILD_LDFLAGS \
        $LIB_FUZZING_ENGINE \
        -Wl,-Bdynamic
    ln -sf $OUT/gdk-pixbuf_seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
    ln -sf $OUT/gdk-pixbuf.dict $OUT/${new_f}.dict
done



fuzzers=$(find $SRC/fuzz/ -name "*_fuzzer.c")
for f in $fuzzers; do
  fuzzer_name=$(basename $f .c)
  $CC $CFLAGS $BUILD_CFLAGS -c $f -o $WORK/${fuzzer_name}.o
  $CXX $CXXFLAGS \
    $WORK/${fuzzer_name}.o -o $OUT/${fuzzer_name} \
    $PREDEPS_LDFLAGS \
    $BUILD_LDFLAGS \
    $LIB_FUZZING_ENGINE \
    -Wl,-Bdynamic
  ln -sf $OUT/gdk-pixbuf_seed_corpus.zip $OUT/${fuzzer_name}_seed_corpus.zip
  ln -sf $OUT/gdk-pixbuf.dict $OUT/${fuzzer_name}.dict
done
