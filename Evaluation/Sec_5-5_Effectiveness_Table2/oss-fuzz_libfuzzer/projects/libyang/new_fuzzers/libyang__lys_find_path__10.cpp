#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libyang.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(char*) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int output;
output = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* xpath;
xpath = malloc(__fuzz_1_LEN + 1);
memcpy(xpath, __fuzz_1, __fuzz_1_LEN);
xpath[__fuzz_1_LEN] = 0;
uint8_t* buf = _IN_BUFFER + _BUF_POINTER;
size_t len = _IN_REMAIN_LEN;

struct lysc_node_leaf * sleaf;

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
// Extracted from: sysrepo: src/common.c_0
// TODO: very hacky. Who knows whether this will work
int __sr_val__type = 5;
 switch ( __sr_val__type ){
 sleaf = ( struct lysc_node_leaf * )lys_find_path(ctx,  NULL,  xpath,  output);
}

/**** End of extracted external code ****/
ly_ctx_destroy(ctx);
free(data);
return 0;
}
