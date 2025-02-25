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
BOXA *boxa1, *boxa2;
boxa1 = boxaReadMem(data, size);
if( boxa1==NULL)return 0;
/**** Start of extracted external code ****/
// Extracted from: tesseract: src/ccstruct/debugpixa.h_0
pixaClear(NULL);

/**** End of extracted external code ****/
boxa2 = boxaReconcileAllByMedian(boxa1, L_ADJUST_LEFT_AND_RIGHT,
                                     L_ADJUST_TOP_AND_BOT, 50, 0, NULL);
if( boxa2!=NULL)boxaDestroy(&boxa2);
boxa2 = boxaReconcileAllByMedian(boxa1, L_ADJUST_SKIP,
                                     L_ADJUST_TOP_AND_BOT, 50, 0, NULL);
if( boxa2!=NULL)boxaDestroy(&boxa2);
boxaDestroy(&boxa1);
return 0;
}
