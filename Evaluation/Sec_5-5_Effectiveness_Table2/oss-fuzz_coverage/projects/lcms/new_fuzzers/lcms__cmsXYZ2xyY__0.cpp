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


/**** End of fuzz variables initialization code ****/
if( size<sizeof( cmsViewingConditions)){
return 0;
}
cmsViewingConditions viewingConditions;
viewingConditions.whitePoint.X = data[0]/ 255.0;
viewingConditions.whitePoint.Y = data[1]/ 255.0;
viewingConditions.whitePoint.Z = data[2]/ 255.0;
viewingConditions.Yb = data[3] / 255.0;
viewingConditions.La = data[4]/ 255.0;
viewingConditions.surround = data[5] % 4 + 1;
viewingConditions.D_value = data[6] / 255.0;
cmsContext context = cmsCreateContext(NULL, NULL);
cmsHANDLE hModel = cmsCIECAM02Init(context, &viewingConditions);
if( hModel){
cmsCIEXYZ inputXYZ;
inputXYZ.X = data[0]/ 255.0;
inputXYZ.Y = data[1] / 255.0;
inputXYZ.Z = data[2] / 255.0;
cmsJCh outputJCh;
cmsCIEXYZ outputXYZ;
cmsCIECAM02Forward(hModel, &inputXYZ, &outputJCh);
/**** Start of extracted external code ****/
// Extracted from: colord: lib/colord/cd-icc.c_0
 cmsCIExyY tmp;
cmsXYZ2xyY( & tmp, &inputXYZ);
{
}

/**** End of extracted external code ****/
cmsCIECAM02Reverse(hModel, &outputJCh, &outputXYZ);
cmsCIECAM02Done(hModel);
}
cmsDeleteContext(context);
return 0;
}

