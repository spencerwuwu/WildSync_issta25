#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "fuzzer_temp_file.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(GAsyncResult*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

GAsyncResult* __fuzz_0 = (GAsyncResult*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

GAsyncResult* res;
res = __fuzz_0;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<1){
return 0;
}
GError *error = NULL;
GdkPixbuf *pixbuf;
char *tmpfile = fuzzer_get_tmpfile(data, size);
/**** Start of extracted external code ****/
// Extracted from: libchamplain: champlain/champlain-image-renderer.c_0
 pixbuf =gdk_pixbuf_new_from_stream_finish( res, &error);

/**** End of extracted external code ****/
pixbuf = gdk_pixbuf_new_from_file_at_scale(tmpfile, 1, size, TRUE, &error);
g_clear_error(&error);
pixbuf = gdk_pixbuf_new_from_file_at_scale(tmpfile, 1, size, FALSE, &error);
if( pixbuf!=NULL){
g_clear_object(&pixbuf);
}
else {
g_clear_error(&error);
}
fuzzer_release_tmpfile(tmpfile);
return 0;
}
