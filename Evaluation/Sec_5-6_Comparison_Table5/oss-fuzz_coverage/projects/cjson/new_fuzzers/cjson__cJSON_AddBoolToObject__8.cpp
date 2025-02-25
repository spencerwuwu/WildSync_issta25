
#ifdef __cplusplus
extern "C" {
#endif
                    #include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../cJSON.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(char*) + sizeof(char*) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int argc;
argc = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 3;

char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* access;
access = __fuzz_1;
char* __fuzz_2 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_2_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_2_LEN;

char* acltype;
acltype = __fuzz_2;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
cJSON *json;
size_t offset = 4;
unsigned char *copied;
char *printed_json = NULL;
int minify, require_termination, formatted, buffered;
if( size<= offset)return 0;
if( data[ size-1]!='\0')return 0;
if( data[0]!='1'&& data[0]!='0')return 0;
if( data[1]!='1'&& data[1]!='0')return 0;
if( data[2]!='1'&& data[2]!='0')return 0;
if( data[3]!='1'&& data[3]!='0')return 0;
minify              = data[0] == '1' ? 1 : 0;
require_termination = data[1] == '1' ? 1 : 0;
formatted           = data[2] == '1' ? 1 : 0;
buffered            = data[3] == '1' ? 1 : 0;
/**** Start of extracted external code ****/
// Extracted from: mosquitto: apps/mosquitto_ctrl/dynsec.c_0
 cJSON * j_acls, * j_acl;
 if ( argc == 2 ){
}
 if ( !strcasecmp ( access,"allow" ) ){
}
 j_acl =cJSON_CreateObject ( );
 if ( (cJSON_AddStringToObject ( j_acl,"acltype", acltype ) == NULL ||cJSON_AddBoolToObject( j_acl, "allow", require_termination) == NULL ) ){
}

/**** End of extracted external code ****/
json = cJSON_ParseWithOpts((char*)data + offset, NULL, require_termination);
if( json==NULL)return 0;
if( buffered){
printed_json = cJSON_PrintBuffered(json, 1, formatted);
}
else {
if( formatted){
printed_json = cJSON_Print(json);
}
else {
printed_json = cJSON_PrintUnformatted(json);
}
}
if( printed_json!=NULL)free(printed_json);
if( minify){
copied = (unsigned char*)malloc(size);
if( copied==NULL)return 0;
memcpy(copied, data, size);
cJSON_Minify((char*)copied + offset);
free(copied);
}
cJSON_Delete(json);
return 0;
}

#ifdef __cplusplus
}
#endif
                    