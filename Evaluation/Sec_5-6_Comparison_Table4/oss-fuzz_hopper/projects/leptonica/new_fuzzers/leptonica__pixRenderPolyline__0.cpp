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
PIX *pix1, *return_pix1;
PTA *ptas, *ptad;
ptas = ptaCreate(0);
ptad = ptaCreate(0);
/**** Start of extracted external code ****/
// Extracted from: tesseract: src/ccstruct/quspline.cpp_0
 int kLineWidth = 5;
 switch (pixGetDepth ( pixs_payload ) ){
pixRenderPolyline(pixs_payload, ptad,  kLineWidth,  L_SET_PIXELS,  1);
}

/**** End of extracted external code ****/
return_pix1 = pixAffinePta(pixs_payload, ptad, ptas, L_BRING_IN_WHITE);
ptaDestroy(&ptas);
ptaDestroy(&ptad);
pixDestroy(&return_pix1);
pix1 = pixRead("../test8.jpg");
ptas = ptaCreate(0);
ptad = ptaCreate(0);
return_pix1 = pixAffinePtaWithAlpha(pixs_payload, ptad, ptas, pix1, 0.9, 1);
pixDestroy(&pix1);
ptaDestroy(&ptas);
ptaDestroy(&ptad);
pixDestroy(&return_pix1);
ptas = ptaCreate(0);
ptad = ptaCreate(0);
return_pix1 = pixAffineSequential(pixs_payload, ptad, ptas, 3, 3);
ptaDestroy(&ptas);
ptaDestroy(&ptad);
pixDestroy(&return_pix1);
pixDestroy(&pixs_payload);
return 0;
}
