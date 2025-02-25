#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "lcms2.h"
#include <stdbool.h>
#define gchar char
#define gboolean bool
#define guchar unsigned char
#define guint unsigned int
typedef struct {
	guint			 idx;
	cmsFloat32Number	*data;
} CdIccUtilsGamutCheckHelper;
                    
void
run_test( uint8_t *data,
         size_t size,
         uint32_t intent,
         uint32_t flags, int dstVal) {
  if (size < 2) {
    return;
  }

  cmsHPROFILE srcProfile = cmsOpenProfileFromMem(data, size);
  if (!srcProfile) return;

   
  cmsHPROFILE dstProfile;
  uint32_t dstFormat;
  if (dstVal == 1) {
    dstProfile = cmsCreateLab4Profile(NULL);
    dstFormat = TYPE_Lab_8;
  }
  else if (dstVal == 2) {
    dstProfile = cmsCreateLab2Profile(NULL);
    dstFormat = TYPE_LabV2_8;
  }
  else if (dstVal == 3) {
    cmsToneCurve* gamma18;
    gamma18 = cmsBuildGamma(0, 1.8);
    dstProfile = cmsCreateGrayProfile(NULL, gamma18);
    cmsFreeToneCurve(gamma18);
    dstFormat = TYPE_GRAY_FLT | EXTRA_SH(1);
  }
  else if (dstVal == 4) {
    dstProfile = cmsCreateXYZProfile();
    dstFormat = TYPE_XYZ_16;
  }
  else if (dstVal == 5) {
    dstProfile = cmsCreateXYZProfile();
    dstFormat = TYPE_XYZ_DBL;
  }
  else if (dstVal == 6) {
    dstProfile = cmsCreateLab4Profile(NULL);
    dstFormat = TYPE_Lab_DBL;
  }
  else if (dstVal == 7) {
    dstProfile = cmsCreateLab4Profile(NULL);
    dstFormat = TYPE_Lab_DBL;
  }
  else {
    dstProfile = cmsCreate_sRGBProfile();
    dstFormat = TYPE_RGB_8;
  }

  if (!dstProfile) {
    cmsCloseProfile(srcProfile);
    return;
  }

   
  cmsColorSpaceSignature srcCS = cmsGetColorSpace(srcProfile);
  cmsUInt32Number nSrcComponents = cmsChannelsOf(srcCS);
  cmsUInt32Number srcFormat;
  if (srcCS == cmsSigLabData) {
    if (dstVal != 7) {
        srcFormat =
            COLORSPACE_SH(PT_Lab) | CHANNELS_SH(nSrcComponents) | BYTES_SH(0);
    }
    else {
        srcFormat =
            COLORSPACE_SH(PT_Lab) | CHANNELS_SH(nSrcComponents) | BYTES_SH(0) | FLOAT_SH(1);
    }
  } else {
    srcFormat =
        COLORSPACE_SH(PT_ANY) | CHANNELS_SH(nSrcComponents) | BYTES_SH(1);
  }

   
  cmsContext ctx = cmsCreateContext(NULL, NULL);
  cmsHTRANSFORM hTransform = cmsCreateTransformTHR(
    ctx,
    srcProfile,
    srcFormat,
    dstProfile,
    dstFormat,
    intent,
    flags);

  cmsCloseProfile(srcProfile);
  cmsCloseProfile(dstProfile);
  if (!hTransform) return;


   
   
   
  if (T_BYTES(srcFormat) == 0) {   
     
    long long output[nSrcComponents*4];
    double input[nSrcComponents];
    for (uint32_t i = 0; i < nSrcComponents; i++) input[i] = 0.5f;
    cmsDoTransform(hTransform, input, output, 1);
  } 
  else {
    uint8_t input[nSrcComponents];
    for (uint32_t i = 0; i < nSrcComponents; i++) input[i] = 128;

    if (dstFormat == TYPE_XYZ_16) {
      cmsCIEXYZ output_XYZ = { 0, 0, 0 };
      cmsDoTransform(hTransform, input, &output_XYZ, 1);
    }
    else if (dstFormat == TYPE_XYZ_DBL) {
      cmsCIEXYZTRIPLE out[4];
      cmsDoTransform(hTransform, input, out, 1);
    }
    else if (dstFormat == TYPE_Lab_DBL) {
      cmsCIELab Lab1;
      cmsDoTransform(hTransform, input, &Lab1, 1);
    }
    else {
      uint8_t output[4];
      cmsDoTransform(hTransform, input, output, 1);
    }
  }
  cmsDeleteTransform(hTransform);
}
 int  LLVMFuzzerTestOneInput(  uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(cmsContext) + 1) {
	return 0;
}
cmsContext __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(cmsContext));
_BUF_POINTER += sizeof(cmsContext);

cmsContext ContextID;
ContextID = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

cmsToneCurve * Result;

/**** End of fuzz variables initialization code ****/
if( size<12){
return 0;
}
uint32_t flags         = *(( uint32_t *)data+0);
uint32_t intent        = *(( uint32_t *)data+1) % 16;
int decider = *((int*)data+2) % 10;
data += 12;
size -= 12;
int dstVal = decider;
if( size<2){
return 0;
}
cmsHPROFILE srcProfile = cmsOpenProfileFromMem(data, size);
if(! srcProfile)return 0;
cmsHPROFILE dstProfile;
uint32_t dstFormat;
if( dstVal==1){
dstProfile = cmsCreateLab4Profile(NULL);
dstFormat = TYPE_Lab_8;
}
else if( dstVal==2){
dstProfile = cmsCreateLab2Profile(NULL);
dstFormat = TYPE_LabV2_8;
}
else if( dstVal==3){
cmsToneCurve* gamma18;
gamma18 = cmsBuildGamma(0, 1.8);
dstProfile = cmsCreateGrayProfile(NULL, gamma18);
cmsFreeToneCurve(gamma18);
/**** Start of extracted external code ****/
// Extracted from: ghostscript: lcms2mt/testbed/testcms2.c_2
// TODO: very hacky. Who knows whether this will work
int __cmsContext = 5;
 Result =cmsJoinToneCurve( ContextID, gamma18, gamma18,  4096);

/**** End of extracted external code ****/
dstFormat = TYPE_GRAY_FLT | EXTRA_SH(1);
}
else if( dstVal==4){
dstProfile = cmsCreateXYZProfile();
dstFormat = TYPE_XYZ_16;
}
else if( dstVal==5){
dstProfile = cmsCreateXYZProfile();
dstFormat = TYPE_XYZ_DBL;
}
else if( dstVal==6){
dstProfile = cmsCreateLab4Profile(NULL);
dstFormat = TYPE_Lab_DBL;
}
else if( dstVal==7){
dstProfile = cmsCreateLab4Profile(NULL);
dstFormat = TYPE_Lab_DBL;
}
else {
dstProfile = cmsCreate_sRGBProfile();
dstFormat = TYPE_RGB_8;
}
if(! dstProfile){
cmsCloseProfile(srcProfile);
return 0;
}
cmsColorSpaceSignature srcCS = cmsGetColorSpace(srcProfile);
cmsUInt32Number nSrcComponents = cmsChannelsOf(srcCS);
cmsUInt32Number srcFormat;
if( srcCS== cmsSigLabData){
if( dstVal!=7){
srcFormat =
            COLORSPACE_SH(PT_Lab) | CHANNELS_SH(nSrcComponents) | BYTES_SH(0);
}
else {
srcFormat =
            COLORSPACE_SH(PT_Lab) | CHANNELS_SH(nSrcComponents) | BYTES_SH(0) | FLOAT_SH(1);
}
}
else {
srcFormat =
        COLORSPACE_SH(PT_ANY) | CHANNELS_SH(nSrcComponents) | BYTES_SH(1);
}
cmsContext ctx = cmsCreateContext(NULL, NULL);
cmsHTRANSFORM hTransform = cmsCreateTransformTHR(
    ctx,
    srcProfile,
    srcFormat,
    dstProfile,
    dstFormat,
    intent,
    flags);
cmsCloseProfile(srcProfile);
cmsCloseProfile(dstProfile);
if(! hTransform)return 0;
if( T_BYTES( srcFormat)==0){
long long output[nSrcComponents*4];
double input[nSrcComponents];
for(uint32_t i=0;
 i< nSrcComponents;
 i++)input[i] = 0.5f;
cmsDoTransform(hTransform, input, output, 1);
}
else {
uint8_t input[nSrcComponents];
for(uint32_t i=0;
 i< nSrcComponents;
 i++)input[i] = 128;
if( dstFormat== TYPE_XYZ_16){
cmsCIEXYZ output_XYZ = { 0, 0, 0 };
cmsDoTransform(hTransform, input, &output_XYZ, 1);
}
else if( dstFormat== TYPE_XYZ_DBL){
cmsCIEXYZTRIPLE out[4];
cmsDoTransform(hTransform, input, out, 1);
}
else if( dstFormat== TYPE_Lab_DBL){
cmsCIELab Lab1;
cmsDoTransform(hTransform, input, &Lab1, 1);
}
else {
uint8_t output[4];
cmsDoTransform(hTransform, input, output, 1);
}
}
cmsDeleteTransform(hTransform);
return 0;
}

