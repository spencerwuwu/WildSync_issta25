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
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(int) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __priv__crop_x;
__priv__crop_x = __fuzz_0;
int __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __priv__crop_y;
__priv__crop_y = __fuzz_1;
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
/**** Start of extracted external code ****/
// Extracted from: libcheese: libcheese/um-crop-area.c_0
// TODO: very hacky. Who knows whether this will work
int __priv__crop_width = 5;
int __priv__crop_height = 5;
 gint width, height;
 width =gdk_pixbuf_get_width ( pixbuf );
 height =gdk_pixbuf_get_height ( pixbuf );
 width =MIN ( __priv__crop_width, width - __priv__crop_x );
 height =MIN ( __priv__crop_height, height - __priv__crop_y );
gdk_pixbuf_new_subpixbuf(pixbuf,  __priv__crop_x,  __priv__crop_y,  width,  height);

/**** End of extracted external code ****/
gdk_pixbuf_get_height(pixbuf);
g_object_unref(pixbuf);
}
free(buf);
g_object_unref(anim);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
