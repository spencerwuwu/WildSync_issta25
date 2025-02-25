#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "leptfuzz.h"
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int*) + sizeof(int*) + sizeof(int*) + sizeof(int*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 5;

int* __fuzz_0 = (int*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

int* x_start;
x_start = __fuzz_0;
int* __fuzz_1 = (int*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

int* y_start;
y_start = __fuzz_1;
int* __fuzz_2 = (int*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_2_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_2_LEN;

int* x_end;
x_end = __fuzz_2;
int* __fuzz_3 = (int*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_3_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_3_LEN;

int* y_end;
y_end = __fuzz_3;
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
// Extracted from: tesseract: src/textord/imagefind.cpp_0
 Box * input_box =boxCreate ( * x_start, * y_start, * x_end - * x_start, * y_end - * y_start );
 Box * output_box = nullptr;
pixClipBoxToForeground(NULL,  input_box,  nullptr,  & output_box);

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
