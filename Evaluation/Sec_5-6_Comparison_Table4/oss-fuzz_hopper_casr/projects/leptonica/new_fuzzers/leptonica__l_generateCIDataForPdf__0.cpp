#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "leptfuzz.h"
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(char*) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int jpg_quality;
jpg_quality = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* filename;
filename = __fuzz_1;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

int  sad;

/**** End of fuzz variables initialization code ****/
if( size<3)return 0;
leptSetStdNullHandler();
PIX *pixs_payload = pixReadMemSpix(data, size);
if( pixs_payload==NULL)return 0;
/**** Start of extracted external code ****/
// Extracted from: tesseract: src/api/pdfrenderer.cpp_0
 L_Compressed_Data * cid = nullptr;
 if (pixGetInputFormat ( pixs_payload ) == IFF_PNG ){
 sad =pixGenerateCIData ( pixs_payload, L_FLATE_ENCODE, 0, 0, & cid );
}
 if ( ! cid ){
 sad =l_generateCIDataForPdf( filename, pixs_payload,  jpg_quality,  & cid);
}
{
}

/**** End of extracted external code ****/
pixSetChromaSampling(pixs_payload, 0);
pixDestroy(&pixs_payload);
return 0;
}
