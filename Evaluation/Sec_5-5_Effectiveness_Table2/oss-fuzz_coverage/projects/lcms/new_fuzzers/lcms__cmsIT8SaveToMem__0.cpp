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
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

guint  i;
gboolean  ret;

/**** End of fuzz variables initialization code ****/
if( size==0)return 0;
cmsHANDLE handle = cmsIT8LoadFromMem(0, (void *)data, size);
if( handle){
char filename[256];
sprintf(filename, "/tmp/fuzzer-it.%d.it8", getpid());
cmsIT8SaveToFile(handle, filename);
/**** Start of extracted external code ****/
// Extracted from: colord: lib/colord/cd-it8.c_0
// TODO: very hacky. Who knows whether this will work
int __priv__title = 5;
int __priv__originator = 5;
int __priv__reference = 5;
int __priv__enable_created = 5;
int __priv__kind = 5;
int __priv__options__len = 5;
 cmsUInt32Number size_tmp = 0;
 if ( __priv__title != NULL ){
}
 if ( __priv__originator != NULL ){
}
 if ( __priv__reference != NULL ){
}
 if ( __priv__enable_created ){
}
 switch ( __priv__kind ){
}
 for ( i = 0;
 i < __priv__options__len;
 i ++ ){
}
 ret =cmsIT8SaveToMem(handle,  NULL,  & size_tmp);

/**** End of extracted external code ****/
cmsIT8Free(handle);
}
return 0;
}

