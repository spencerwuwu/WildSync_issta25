
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
if (_IN_BUFFER_SIZE < sizeof(char*) + sizeof(char*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 3;

char* __fuzz_0 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

char* groupname;
groupname = __fuzz_0;
char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* correlation_data;
correlation_data = __fuzz_1;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

cJSON * j_data;
cJSON * j_group;

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
json = cJSON_ParseWithOpts((char*)data + offset, NULL, require_termination);
if( json==NULL)return 0;
if( buffered){
/**** Start of extracted external code ****/
// Extracted from: mosquitto: plugins/dynamic-security/groups.c_2
 if ( (cJSON_AddStringToObject ( json,"command","getAnonymousGroup" ) == NULL || ( j_data =cJSON_AddObjectToObject(json, "data") ) == NULL || ( j_group =cJSON_AddObjectToObject(j_data, "group") ) == NULL ||cJSON_AddStringToObject ( j_group,"groupname", groupname ) == NULL || ( correlation_data &&cJSON_AddStringToObject ( json,"correlationData", correlation_data ) == NULL ) ) ){
}

/**** End of extracted external code ****/
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
                    