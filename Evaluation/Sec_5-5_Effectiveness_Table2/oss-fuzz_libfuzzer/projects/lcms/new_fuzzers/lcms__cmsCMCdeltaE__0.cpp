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
if( size<16){
return 0;
}
cmsHANDLE hGDB = cmsGBDAlloc(NULL);
if(! hGDB){
return 0;
}
cmsCIELab Lab;
Lab.L = *(( uint32_t *)data);
Lab.a = *(( uint32_t *)data+1);
Lab.b = *(( uint32_t *)data+2);
cmsGDBAddPoint(hGDB, &Lab);
/**** Start of extracted external code ****/
// Extracted from: enblend-enfuse: src/fixmath.h_0
cmsCMCdeltaE(&Lab, &Lab,  2.0,  1.0);

/**** End of extracted external code ****/
cmsGDBCheckPoint(hGDB, &Lab);
cmsGDBCompute(hGDB, *(( uint32_t *)data+3));
cmsGBDFree(hGDB);
return 0;
}

