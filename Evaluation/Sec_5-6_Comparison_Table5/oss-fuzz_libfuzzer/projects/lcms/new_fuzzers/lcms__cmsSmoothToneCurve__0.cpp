#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
                    
#include <stdint.h>
 int  LLVMFuzzerTestOneInput(  uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<16){
return 0;
}
cmsHPROFILE hInProfile = cmsOpenProfileFromMem(data, size);
if(! hInProfile){
return 0;
}
cmsHPROFILE hOutProfile = cmsCreate_sRGBProfile();
if(! hOutProfile){
cmsCloseProfile(hInProfile);
return 0;
}
cmsColorSpaceSignature srcCS = cmsGetColorSpace(hInProfile);
cmsUInt32Number nSrcComponents = cmsChannelsOf(srcCS);
cmsUInt32Number srcFormat;
if( srcCS== cmsSigLabData){
srcFormat =
        COLORSPACE_SH(PT_Lab) | CHANNELS_SH(nSrcComponents) | BYTES_SH(0);
}
else {
srcFormat =
        COLORSPACE_SH(PT_ANY) | CHANNELS_SH(nSrcComponents) | BYTES_SH(1);
}
cmsHTRANSFORM hTransform = cmsCreateTransform(
      hInProfile, srcFormat, hOutProfile, TYPE_BGR_8,
      *(( uint32_t *)data + 3) % 16, *(( uint32_t *)data + 2));
cmsCloseProfile(hInProfile);
cmsCloseProfile(hOutProfile);
if(! hTransform){
return 0;
}
cmsFloat64Number version;
if(*((uint32_t*) data+3)%2==0){
version = 3.4;
}
else {
version = 4.4;
}
cmsHPROFILE devicelinkProfile = cmsTransform2DeviceLink(
      hTransform, version, *(( uint32_t *)data + 2));
cmsDeleteTransform(hTransform);
if( devicelinkProfile){
cmsCloseProfile(devicelinkProfile);
}
cmsToneCurve *tone = cmsBuildGamma(NULL, *(( uint32_t *)data + 3));
if(! tone){
return 0;
}
cmsToneCurve *rgb_curves[15] = {tone, tone, tone, tone, tone,
                                  tone, tone, tone, tone, tone,
                                  tone, tone, tone, tone, tone};
cmsHPROFILE linearizationDeviceLinkProfile =
      cmsCreateLinearizationDeviceLink(srcCS, rgb_curves);
/**** Start of extracted external code ****/
// Extracted from: colord: client/cd-create-profile.c_0
cmsSmoothToneCurve(tone,  5);

/**** End of extracted external code ****/
cmsFreeToneCurve(tone);
if( linearizationDeviceLinkProfile){
cmsCloseProfile(linearizationDeviceLinkProfile);
}
return 0;
}

