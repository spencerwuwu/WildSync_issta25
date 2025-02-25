#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <json.h>
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int nPolys;
nPolys = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

int  i;

/**** End of fuzz variables initialization code ****/
char *data1 = reinterpret_cast<char *>(data);
json_tokener *tok = json_tokener_new();
json_object *obj = json_tokener_parse_ex(tok, data1, size);
/**** Start of extracted external code ****/
// Extracted from: postgis: liblwgeom/lwin_geojson.c_5
if (json_object_get_type(obj) == json_type_array) {
 for ( int i = 0;
 i < nPolys;
 ++ i ){
 json_object * rings =json_object_array_get_idx(obj,  i);
}
}

/**** End of extracted external code ****/
json_object_put(obj);
json_tokener_free(tok);
return 0;
}
