#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include "gd.h"
#include "gdfontg.h"
#include "gdfontl.h"
#include "gdfontmb.h"
#include "gdfonts.h"
#include "gdfontt.h"
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(int) + sizeof(int) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __args1;
__args1 = __fuzz_0;
int __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __args0;
__args0 = __fuzz_1;
int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __args2;
__args2 = __fuzz_2;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

int  color;

/**** End of fuzz variables initialization code ****/
FuzzedDataProvider stream(data, size);
uint8_t slate_width = stream.ConsumeIntegral<uint8_t>();
uint8_t slate_height = stream.ConsumeIntegral<uint8_t>();
gdImagePtr slate_image = gdImageCreateTrueColor(slate_width, slate_height);
if( slate_image==nullptr){
return 0;
}
int x_position = stream.ConsumeIntegral<int>();
int y_position = stream.ConsumeIntegral<int>();
int text_color = stream.ConsumeIntegral<int>();
gdFontPtr font_ptr = stream.PickValueInArray(
        {gdFontGetGiant(), gdFontGetLarge(), gdFontGetMediumBold(),
        gdFontGetSmall(), gdFontGetTiny()});
std::string text = stream.ConsumeRemainingBytesAsString();
/**** Start of extracted external code ****/
// Extracted from: graphviz: tclpkg/gdtclft/gdtclft.c_0
 color =gdImageColorExact(slate_image,  __args0,  __args1,  __args2);

/**** End of extracted external code ****/
gdImageString(slate_image, font_ptr, x_position, y_position,
                  reinterpret_cast<uint8_t*>(const_cast<char*>(text.c_str())),
                  text_color);
gdImageDestroy(slate_image);
return 0;
}
