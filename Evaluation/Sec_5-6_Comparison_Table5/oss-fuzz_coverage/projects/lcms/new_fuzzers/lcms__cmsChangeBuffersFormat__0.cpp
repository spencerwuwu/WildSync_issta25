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
         uint32_t intent_id,
         uint32_t input_format,
         uint32_t output_format,
         uint32_t flags) {
 if (size < 2) {
   return;
 }

 size_t mid = size / 2;

 cmsHPROFILE hInProfile, hOutProfile;
 cmsHTRANSFORM hTransform;

 hInProfile = cmsOpenProfileFromMem(data, mid);
 hOutProfile = cmsOpenProfileFromMem(data + mid, size - mid);
 hTransform = cmsCreateTransform(hInProfile, input_format, hOutProfile,
                                 output_format, intent_id, flags);
 cmsCloseProfile(hInProfile);
 cmsCloseProfile(hOutProfile);

 if (hTransform) {
   cmsDeleteTransform(hTransform);
 }
}
 int  LLVMFuzzerTestOneInput(  uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(cmsUInt32Number) + sizeof(cmsUInt32Number) + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) + 1) {
	return 0;
}
cmsUInt32Number __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(cmsUInt32Number));
_BUF_POINTER += sizeof(cmsUInt32Number);

cmsUInt32Number __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(cmsUInt32Number));
_BUF_POINTER += sizeof(cmsUInt32Number);

int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int planar;
planar = __fuzz_2;
int __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int numbytes;
numbytes = __fuzz_3;
int __fuzz_4;
memcpy(&__fuzz_4, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int swap_endian;
swap_endian = __fuzz_4;
int __fuzz_5;
memcpy(&__fuzz_5, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int hasalpha;
hasalpha = __fuzz_5;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<16){
return 0;
}
uint32_t input_format  = *(( uint32_t *)data);
uint32_t output_format = *(( uint32_t *)data+1);
uint32_t flags         = *(( uint32_t *)data+2);
uint32_t intent        = *(( uint32_t *)data+3) % 16;
data += 16;
size -= 16;
if( size<2){
return 0;
}
size_t mid = size / 2;
cmsHPROFILE hInProfile, hOutProfile;
cmsHTRANSFORM hTransform;
hInProfile = cmsOpenProfileFromMem(data, mid);
hOutProfile = cmsOpenProfileFromMem(data + mid, size - mid);
hTransform = cmsCreateTransform(hInProfile, input_format, hOutProfile,
                                 output_format, intent, flags);
cmsCloseProfile(hInProfile);
cmsCloseProfile(hOutProfile);
if( hTransform){
/**** Start of extracted external code ****/
// Extracted from: ghostscript: base/gsicc_lcms2.c_0
// TODO: very hacky. Who knows whether this will work
int __cmsUInt32Number = 5;
int __input_buff_desc__num_chan = 5;
int __output_buff_desc__num_chan = 5;
 cmsUInt32Number dwInputFormat, dwOutputFormat, num_src_lcms, num_des_lcms;
 dwInputFormat = __fuzz_0;
 dwOutputFormat = __fuzz_1;
 dwInputFormat = dwInputFormat |PLANAR_SH ( planar );
 dwOutputFormat = dwOutputFormat |PLANAR_SH ( planar );
 if ( numbytes > 2 ){
}
 dwInputFormat = dwInputFormat |BYTES_SH ( numbytes );
 if ( numbytes > 2 ){
}
 dwOutputFormat = dwOutputFormat |BYTES_SH ( numbytes );
 dwInputFormat = dwInputFormat |ENDIAN16_SH ( swap_endian );
 dwOutputFormat = dwOutputFormat |ENDIAN16_SH ( swap_endian );
 if ( num_src_lcms != __input_buff_desc__num_chan || num_des_lcms != __output_buff_desc__num_chan ){
}
 dwInputFormat = dwInputFormat |CHANNELS_SH ( num_src_lcms );
 dwOutputFormat = dwOutputFormat |CHANNELS_SH ( num_des_lcms );
 dwInputFormat = dwInputFormat |EXTRA_SH ( hasalpha );
 dwOutputFormat = dwOutputFormat |EXTRA_SH ( hasalpha );
cmsChangeBuffersFormat(hTransform,  dwInputFormat,  dwOutputFormat);

/**** End of extracted external code ****/
cmsDeleteTransform(hTransform);
}
return 0;
}

