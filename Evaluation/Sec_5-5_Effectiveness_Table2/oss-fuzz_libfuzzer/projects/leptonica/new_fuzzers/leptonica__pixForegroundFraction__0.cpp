#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "leptfuzz.h"
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<3)return 0;
leptSetStdNullHandler();
PIX *pixs_payload = pixReadMemSpix(data, size);
if( pixs_payload==NULL)return 0;
/**** Start of extracted external code ****/
// Extracted from: tesseract: src/ccmain/equationdetect.cpp_0
 l_float32 fract;
pixForegroundFraction(pixs_payload,  & fract);

/**** End of extracted external code ****/
pixSetChromaSampling(pixs_payload, 0);
pixDestroy(&pixs_payload);
return 0;
}
