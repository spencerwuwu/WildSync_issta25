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
                    
 int  LLVMFuzzerTestOneInput(  uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

int  lcms_des_color_space;

/**** End of fuzz variables initialization code ****/
cmsHPROFILE srcProfile = cmsOpenProfileFromMem(data, size);
if(! srcProfile)return 0;
cmsHPROFILE dstProfile = cmsCreate_sRGBProfile();
if(! dstProfile){
cmsCloseProfile(srcProfile);
return 0;
}
cmsColorSpaceSignature srcCS = cmsGetColorSpace(srcProfile);
cmsUInt32Number nSrcComponents = cmsChannelsOf(srcCS);
/**** Start of extracted external code ****/
// Extracted from: ghostscript: base/gsicc_lcms2.c_0
 lcms_des_color_space =_cmsLCMScolorSpace(srcCS);

/**** End of extracted external code ****/
cmsUInt32Number srcFormat;
if( srcCS== cmsSigLabData){
srcFormat =
        COLORSPACE_SH(PT_Lab) | CHANNELS_SH(nSrcComponents) | BYTES_SH(0);
}
else {
srcFormat =
        COLORSPACE_SH(PT_ANY) | CHANNELS_SH(nSrcComponents) | BYTES_SH(1);
}
cmsUInt32Number intent = 0;
cmsUInt32Number flags = 0;
cmsHTRANSFORM hTransform = cmsCreateTransform(
      srcProfile, srcFormat, dstProfile, TYPE_BGR_8, intent, flags);
cmsCloseProfile(srcProfile);
cmsCloseProfile(dstProfile);
if(! hTransform)return 0;
uint8_t output[4];
if( T_BYTES( srcFormat)==0){
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
cmsDoTransform(hTransform, input, output, 1);
}
cmsDeleteTransform(hTransform);
return 0;
}

