#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libyang.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(uint32_t) + sizeof(struct ly_set**) + sizeof(char*) + sizeof(int*) + 1) {
	return 0;
}
uint32_t __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(uint32_t));
_BUF_POINTER += sizeof(uint32_t);

uint32_t options;
options = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 4;

struct ly_set** __fuzz_1 = (struct ly_set**) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

struct ly_set** set;
set = __fuzz_1;
char* __fuzz_2 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_2_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_2_LEN;

char* xpath;
xpath = malloc(__fuzz_2_LEN + 1);
memcpy(xpath, __fuzz_2, __fuzz_2_LEN);
xpath[__fuzz_2_LEN] = 0;
int* __fuzz_3 = (int*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_3_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_3_LEN;

int* valid;
valid = __fuzz_3;
uint8_t* buf = _IN_BUFFER + _BUF_POINTER;
size_t len = _IN_REMAIN_LEN;

LY_ERR  lyrc;

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
// Extracted from: sysrepo: src/ly_wrap.c_0
 if ( valid ){
}
 if ( ( ( lyrc =lys_find_xpath_atoms(ctx,  NULL,  xpath,  options,  set) ) ) ){
}

/**** End of extracted external code ****/
ly_ctx_destroy(ctx);
free(data);
return 0;
}
