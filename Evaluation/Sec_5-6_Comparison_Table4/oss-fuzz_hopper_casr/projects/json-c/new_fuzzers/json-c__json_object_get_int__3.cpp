#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <json.h>
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

int* __fuzz_0 = (int*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

int* value;
value = __fuzz_0;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
char *data1 = reinterpret_cast<char *>(data);
json_tokener *tok = json_tokener_new();
json_object *obj = json_tokener_parse_ex(tok, data1, size);
/**** Start of extracted external code ****/
// Extracted from: swayimg: src/sway.c_0
 * value =json_object_get_int(obj);

/**** End of extracted external code ****/
json_object_put(obj);
json_tokener_free(tok);
return 0;
}
