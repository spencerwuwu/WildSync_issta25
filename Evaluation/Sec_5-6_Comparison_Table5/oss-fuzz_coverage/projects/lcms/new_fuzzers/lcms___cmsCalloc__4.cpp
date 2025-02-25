#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

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
if (_IN_BUFFER_SIZE < sizeof(cmsUInt32Number) + 1) {
	return 0;
}
cmsUInt32Number __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(cmsUInt32Number));
_BUF_POINTER += sizeof(cmsUInt32Number);

cmsUInt32Number Count;
Count = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

cmsUInt32Number * ElementSizes;

/**** End of fuzz variables initialization code ****/
if( size==0)return 0;
cmsHANDLE handle = cmsIT8LoadFromMem(0, (void *)data, size);
/**** Start of extracted external code ****/
// Extracted from: ghostscript: lcms2mt/src/cmstypes.c_1
 ElementSizes = ( cmsUInt32Number * )_cmsCalloc(0,  Count,  sizeof ( cmsUInt32Number ));

/**** End of extracted external code ****/
if( handle){
char filename[256];
sprintf(filename, "/tmp/fuzzer-it.%d.it8", getpid());
cmsIT8SaveToFile(handle, filename);
cmsIT8Free(handle);
}
return 0;
}

