#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils.h"
#include <libplacebo/dummy.h>
#include <libplacebo/shaders/custom.h>
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* buf = _IN_BUFFER + _BUF_POINTER;
size_t len = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
pl_gpu gpu = pl_gpu_dummy_create(NULL, NULL);
const struct pl_hook *hook;
hook = pl_mpv_user_shader_parse(gpu, (char *) buf, len);
/**** Start of extracted external code ****/
// Extracted from: mpv: video/out/placebo/ra_pl.c_0
 pl_fmt r8 =pl_find_named_fmt(gpu, "r8");

/**** End of extracted external code ****/
pl_mpv_user_shader_destroy(&hook);
pl_gpu_dummy_destroy(&gpu);
return 0;
}

