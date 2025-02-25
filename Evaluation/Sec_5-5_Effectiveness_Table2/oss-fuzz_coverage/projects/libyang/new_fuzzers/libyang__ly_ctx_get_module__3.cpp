#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libyang.h"
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

char* __del_mod__revision;
__del_mod__revision = malloc(__fuzz_0_LEN + 1);
memcpy(__del_mod__revision, __fuzz_0, __fuzz_0_LEN);
__del_mod__revision[__fuzz_0_LEN] = 0;
char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* __del_mod__name;
__del_mod__name = malloc(__fuzz_1_LEN + 1);
memcpy(__del_mod__name, __fuzz_1, __fuzz_1_LEN);
__del_mod__name[__fuzz_1_LEN] = 0;
uint8_t* buf = _IN_BUFFER + _BUF_POINTER;
size_t len = _IN_REMAIN_LEN;

uint32_t  i;
struct lys_module * ly_mod;

/**** End of fuzz variables initialization code ****/
struct ly_ctx *ctx = NULL;
static bool log = false;
char *data = NULL;
LY_ERR err;
if(! log){
ly_log_options(0);
log = true;
}
err = ly_ctx_new(NULL, 0, &ctx);
if( err!= LY_SUCCESS){
fprintf(stderr, "Failed to create context\n");
exit(EXIT_FAILURE);
}
data = malloc(len + 1);
if( data==NULL){
return 0;
}
memcpy(data, buf, len);
data[len] = 0;
lys_parse_mem(ctx, data, LYS_IN_YANG, NULL);
/**** Start of extracted external code ****/
// Extracted from: sysrepo: src/context_change.c_0
// TODO: very hacky. Who knows whether this will work
int __mod_set__count = 5;
 for ( i = 0;
 i < __mod_set__count;
 ++ i ){
 ly_mod =ly_ctx_get_module(ctx,  __del_mod__name,  __del_mod__revision);
}

/**** End of extracted external code ****/
ly_ctx_destroy(ctx);
free(data);
return 0;
}
