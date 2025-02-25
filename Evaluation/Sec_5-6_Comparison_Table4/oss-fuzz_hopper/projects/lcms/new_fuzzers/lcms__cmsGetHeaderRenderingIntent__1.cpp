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
if (_IN_BUFFER_SIZE < sizeof(cmsUInt32Number) + 1) {
	return 0;
}
cmsUInt32Number __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(cmsUInt32Number));
_BUF_POINTER += sizeof(cmsUInt32Number);

cmsUInt32Number Intent;
Intent = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<4){
return 0;
}
cmsFloat64Number limit = *(( uint32_t *)data) % 401;
cmsHPROFILE limitingDeviceLinkProfile =
      cmsCreateInkLimitingDeviceLink(cmsSigCmykData, limit);
if( limitingDeviceLinkProfile){
cmsCloseProfile(limitingDeviceLinkProfile);
}
/**** Start of extracted external code ****/
// Extracted from: openjpeg2: thirdparty/liblcms2/src/cmsio1.c_0
 return (cmsGetHeaderRenderingIntent(limitingDeviceLinkProfile) == Intent );

/**** End of extracted external code ****/
return 0;
}

