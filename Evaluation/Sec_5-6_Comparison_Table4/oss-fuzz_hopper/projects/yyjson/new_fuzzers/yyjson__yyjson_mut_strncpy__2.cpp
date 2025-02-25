#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <yyjson.h>
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(size_t) + sizeof(char*) + 1) {
	return 0;
}
size_t __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(size_t));
_BUF_POINTER += sizeof(size_t);

size_t __buf__length;
__buf__length = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* __buf__chars;
__buf__chars = __fuzz_1;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
yyjson_read_flag rflags[2] = {YYJSON_READ_NOFLAG,
                    YYJSON_READ_ALLOW_TRAILING_COMMAS |
                    YYJSON_READ_ALLOW_COMMENTS |
                    YYJSON_READ_ALLOW_INF_AND_NAN};
yyjson_write_flag wflags[2] = {YYJSON_WRITE_NOFLAG, 
                    YYJSON_WRITE_PRETTY |
                    YYJSON_WRITE_ESCAPE_UNICODE |
                    YYJSON_WRITE_ESCAPE_SLASHES |
                    YYJSON_WRITE_ALLOW_INF_AND_NAN};
for(int i=0;
 i<2;
 i++){
yyjson_read_flag rflag = rflags[i];
yyjson_read_flag wflag = wflags[i];
yyjson_doc *idoc = yyjson_read((char *)data, size, rflag);
yyjson_mut_doc *mdoc = yyjson_doc_mut_copy(idoc, NULL);
char *ijson = yyjson_write(idoc, wflag, NULL);
if( ijson)free((void *)ijson);
char *mjson = yyjson_mut_write(mdoc, wflag, NULL);
/**** Start of extracted external code ****/
// Extracted from: fastfetch: src/common/jsonconfig.h_0
yyjson_mut_strncpy(mdoc,  __buf__chars,  __buf__length);

/**** End of extracted external code ****/
if( mjson)free((void *)mjson);
yyjson_doc_free(idoc);
yyjson_mut_doc_free(mdoc);
}
return 0;
}
