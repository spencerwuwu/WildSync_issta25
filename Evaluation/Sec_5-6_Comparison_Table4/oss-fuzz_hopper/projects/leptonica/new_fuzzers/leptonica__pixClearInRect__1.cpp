#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "leptfuzz.h"
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(int) + sizeof(int) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int width;
width = __fuzz_0;
int __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int shirorekha_top;
shirorekha_top = __fuzz_1;
int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int stroke_width;
stroke_width = __fuzz_2;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<3)return 0;
leptSetStdNullHandler();
PIX *pixs_payload = pixReadMemSpix(data, size);
if( pixs_payload==NULL)return 0;
BOX *box1;
PIX *pix_pointer_payload;
box1 = boxCreate(278, 35, 122, 50);
/**** Start of extracted external code ****/
// Extracted from: tesseract: src/textord/devanagari_processing.cpp_1
 Box * box_to_clear =boxCreate ( 0, shirorekha_top - stroke_width / 3, width, 5 * stroke_width / 3 );
pixClearInRect(NULL,  box_to_clear);
{
}
{
}

/**** End of extracted external code ****/
pix_pointer_payload = pixCopy(NULL, pixs_payload);
pixSetSelectCmap(pix_pointer_payload, box1, 2, 255, 255, 100);
boxDestroy(&box1);
pixDestroy(&pix_pointer_payload);
pix_pointer_payload = pixCopy(NULL, pixs_payload);
pixSetSelectMaskedCmap(pix_pointer_payload, NULL, 1,  50,  0,  250, 249, 248);
pixDestroy(&pix_pointer_payload);
pixDestroy(&pixs_payload);
return 0;
}
