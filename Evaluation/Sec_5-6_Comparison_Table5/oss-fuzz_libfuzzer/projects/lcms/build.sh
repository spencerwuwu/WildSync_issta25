#!/bin/bash -eu
# Copyright 2016 Google Inc.
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

# build the target.
./configure --enable-shared=no
make -j$(nproc) all

# build your fuzzer(s)
FUZZERS="cmsIT8_load_fuzzer            \
        cms_transform_fuzzer           \
        cms_overwrite_transform_fuzzer \
        cms_transform_all_fuzzer       \
        cms_profile_fuzzer             \
        cms_universal_transform_fuzzer \
        cms_md5_fuzzer                 \
        cms_dict_fuzzer                \
        cms_postscript_fuzzer          \
        cms_cie_cam02_fuzzer           \
        cms_gdb_fuzzer                 \
        cms_cgats_fuzzer               \
        cms_virtual_profile_fuzzer     \
        cms_devicelink_fuzzer" 

new_fuzzers="lcms___cmsCalloc__1 lcms___cmsCalloc__2 lcms___cmsCalloc__3 lcms___cmsCalloc__4 lcms___cmsCalloc__5 lcms___cmsCalloc__6 lcms___cmsCalloc__7 lcms___cmsDupMem__0 lcms___cmsDupMem__10 lcms___cmsDupMem__2 lcms___cmsDupMem__3 lcms___cmsDupMem__4 lcms___cmsDupMem__5 lcms___cmsDupMem__6 lcms___cmsDupMem__7 lcms___cmsDupMem__8 lcms___cmsDupMem__9 lcms___cmsLCMScolorSpace__0 lcms___cmsLCMScolorSpace__1 lcms___cmsLCMScolorSpace__2 lcms___cmsLCMScolorSpace__3 lcms___cmsLCMScolorSpace__4 lcms___cmsLCMScolorSpace__5 lcms___cmsLCMScolorSpace__6 lcms___cmsLockMutex__0 lcms__cmsBuildGamma__1 lcms__cmsBuildGamma__10 lcms__cmsBuildGamma__2 lcms__cmsBuildGamma__3 lcms__cmsBuildGamma__4 lcms__cmsBuildGamma__5 lcms__cmsBuildGamma__6 lcms__cmsBuildGamma__7 lcms__cmsBuildGamma__8 lcms__cmsBuildGamma__9 lcms__cmsBuildParametricToneCurve__3 lcms__cmsBuildTabulatedToneCurve16__6 lcms__cmsBuildTabulatedToneCurveFloat__0 lcms__cmsCMCdeltaE__0 lcms__cmsChangeBuffersFormat__0 lcms__cmsDetectBlackPoint__0 lcms__cmsDupContext__0 lcms__cmsGetContextUserData__0 lcms__cmsGetContextUserData__2 lcms__cmsGetContextUserData__3 lcms__cmsGetDeviceClass__0 lcms__cmsGetDeviceClass__1 lcms__cmsGetDeviceClass__2 lcms__cmsGetHeaderRenderingIntent__0 lcms__cmsGetHeaderRenderingIntent__1 lcms__cmsGetHeaderRenderingIntent__2 lcms__cmsGetPCS__0 lcms__cmsGetPCS__1 lcms__cmsGetProfileContextID__0 lcms__cmsIT8EnumProperties__0 lcms__cmsIT8GetProperty__0 lcms__cmsIT8GetProperty__1 lcms__cmsIT8GetProperty__10 lcms__cmsIT8GetProperty__2 lcms__cmsIT8GetProperty__3 lcms__cmsIT8GetProperty__4 lcms__cmsIT8GetProperty__5 lcms__cmsIT8GetProperty__6 lcms__cmsIT8GetProperty__8 lcms__cmsIT8GetSheetType__0 lcms__cmsIT8SaveToMem__0 lcms__cmsJoinToneCurve__0 lcms__cmsJoinToneCurve__1 lcms__cmsJoinToneCurve__2 lcms__cmsJoinToneCurve__3 lcms__cmsLab2LCh__0 lcms__cmsMD5alloc__0 lcms__cmsPluginTHR__0 lcms__cmsPluginTHR__1 lcms__cmsSmoothToneCurve__0 lcms__cmsSmoothToneCurve__1 lcms__cmsXYZ2Lab__0 lcms__cmsXYZ2Lab__1 lcms__cmsXYZ2xyY__0 lcms__cmsXYZ2xyY__3 lcms__cmsXYZ2xyY__4 lcms__cmsXYZ2xyY__5"

for new_f in $new_fuzzers; do
    cp $SRC/new_fuzzers/${new_f}.cpp $SRC/new_fuzzers/${new_f}.c 
    $CC $CFLAGS -c -Iinclude \
        $SRC/new_fuzzers/${new_f}.c -o $SRC/${new_f}.o 
    $CXX $CXXFLAGS \
        $SRC/${new_f}.o -o $OUT/${new_f} \
        $LIB_FUZZING_ENGINE src/.libs/liblcms2.a 
    cp $SRC/icc.dict $OUT/${new_f}.dict
    cp $SRC/seed_corpus.zip $OUT/${new_f}_seed_corpus.zip
done



for F in $FUZZERS; do
    $CC $CFLAGS -c -Iinclude \
        $SRC/$F.c -o $SRC/$F.o
    $CXX $CXXFLAGS \
        $SRC/$F.o -o $OUT/$F \
        $LIB_FUZZING_ENGINE src/.libs/liblcms2.a
done

cp $SRC/*.dict $SRC/*.options $OUT/
cp $SRC/icc.dict $OUT/cms_transform_all_fuzzer.dict
cp $SRC/icc.dict $OUT/cms_universal_transform_fuzzer.dict
cp $SRC/icc.dict $OUT/cms_profile_fuzzer.dict
cp $SRC/icc.dict $OUT/cms_postscript_fuzzer.dict
cp $SRC/icc.dict $OUT/cms_virtual_profile_fuzzer.dict
cp $SRC/icc.dict $OUT/cms_md5_fuzzer.dict
cp $SRC/seed_corpus.zip $OUT/cms_postscript_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cms_profile_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cms_universal_transform_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cms_transform_all_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cms_transform_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cms_virtual_profile_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cmsIT8_load_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cms_md5_fuzzer_seed_corpus.zip
cp $SRC/seed_corpus.zip $OUT/cms_overwrite_transform_fuzzer_seed_corpus.zip
