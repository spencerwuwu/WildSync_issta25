#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <json.h>
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
char *data1 = reinterpret_cast<char *>(data);
json_tokener *tok = json_tokener_new();
json_object *obj = json_tokener_parse_ex(tok, data1, size);
/**** Start of extracted external code ****/
// Extracted from: ndctl: daxctl/device.c_1
// TODO: very hacky. Who knows whether this will work
int __mapsi_end = 5;
 __mapsi_end =json_object_get_int64(obj);

/**** End of extracted external code ****/
json_object_put(obj);
json_tokener_free(tok);
return 0;
}
