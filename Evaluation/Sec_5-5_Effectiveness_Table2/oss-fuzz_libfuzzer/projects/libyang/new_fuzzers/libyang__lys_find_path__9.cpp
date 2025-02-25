#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libyang.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(char*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

char* __fuzz_0 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

char* path;
path = malloc(__fuzz_0_LEN + 1);
memcpy(path, __fuzz_0, __fuzz_0_LEN);
path[__fuzz_0_LEN] = 0;
uint8_t* buf = _IN_BUFFER + _BUF_POINTER;
size_t len = _IN_REMAIN_LEN;

struct lysc_node * snode;

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
// Extracted from: sysrepo: src/sysrepo.c_0
// TODO: very hacky. Who knows whether this will work
int __session__ds = 5;
int __session__dtsession__ds_edit = 5;
int __session__dtsession__ds = 5;
int __snode__nodetype = 5;
int __snode__name = 5;
 if ( ! __session__dtsession__ds_edit ){
}
 if ( ( ( path[strlen(path) - 1] !=' ]' ) && ( snode =lys_find_path(ctx,  NULL,  path,  0) ) && ( __snode__nodetype & ( LYS_LEAFLIST | LYS_LIST ) ) && !strcmp ( ( path +strlen ( path ) ) -strlen ( __snode__name ), __snode__name ) ) ){
}

/**** End of extracted external code ****/
ly_ctx_destroy(ctx);
free(data);
return 0;
}
