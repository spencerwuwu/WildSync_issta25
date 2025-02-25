#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libyang.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(uint32_t) + 1) {
	return 0;
}
uint32_t __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(uint32_t));
_BUF_POINTER += sizeof(uint32_t);

uint32_t i;
i = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* buf = _IN_BUFFER + _BUF_POINTER;
size_t len = _IN_REMAIN_LEN;

char * mod_origin;

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
// Extracted from: sysrepo: src/lyd_mods.c_0
 mod_origin = ( i <ly_ctx_internal_modules_count(ctx) ) ?"Libyang":
"Sysrepo";

/**** End of extracted external code ****/
ly_ctx_destroy(ctx);
free(data);
return 0;
}
