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
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

GdkPixbufAnimation * pixbuf_animation;

/**** End of fuzz variables initialization code ****/
GError *error = NULL;
GdkPixbuf *pixbuf;
GFile *file;
GInputStream *stream;
char *tmpfile = fuzzer_get_tmpfile(data, size);
file = g_file_new_for_path(tmpfile);
stream = (GInputStream *) g_file_read(file, NULL, &error);
if( error!=NULL){
g_clear_error(&error);
g_object_unref(file);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
/**** Start of extracted external code ****/
// Extracted from: pqiv: backends/gdkpixbuf.c_0
 pixbuf_animation =gdk_pixbuf_animation_new_from_stream(stream, NULL, &error);

/**** End of extracted external code ****/
pixbuf = gdk_pixbuf_new_from_stream(stream, NULL, &error);
if( pixbuf!=NULL){
g_object_unref(pixbuf);
}
g_clear_error(&error);
g_object_unref(stream);
g_object_unref(file);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
