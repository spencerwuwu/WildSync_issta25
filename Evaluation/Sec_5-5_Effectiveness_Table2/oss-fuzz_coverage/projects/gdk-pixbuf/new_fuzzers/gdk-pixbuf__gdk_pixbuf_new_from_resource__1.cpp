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


/**** End of fuzz variables initialization code ****/
if( size<1){
return 0;
}
GError *error = NULL;
GdkPixbuf *pixbuf;
GdkPixbufAnimation *anim;
char *tmpfile = fuzzer_get_tmpfile(data, size);
/**** Start of extracted external code ****/
// Extracted from: libshumate: demos/shumate-demo-window.c_0
// TODO: very hacky. Who knows whether this will work
int __sprites_pixbuf = 5;
 __sprites_pixbuf =gdk_pixbuf_new_from_resource(tmpfile, &error);

/**** End of extracted external code ****/
anim = gdk_pixbuf_animation_new_from_file(tmpfile, &error);
if( error!=NULL){
g_clear_error(&error);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
char *buf = (char *) calloc(size + 1, sizeof(char));
memcpy(buf, data, size);
buf[size] = '\0';
pixbuf = gdk_pixbuf_animation_get_static_image(anim);
if( pixbuf!=NULL){
unsigned int rot_amount = ((unsigned int) data[0]) % 4;
pixbuf = gdk_pixbuf_rotate_simple(pixbuf, rot_amount * 90);
gdk_pixbuf_set_option(pixbuf, buf, buf);
gdk_pixbuf_get_pixels(pixbuf);
gdk_pixbuf_get_width(pixbuf);
gdk_pixbuf_get_height(pixbuf);
g_object_unref(pixbuf);
}
free(buf);
g_object_unref(anim);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
