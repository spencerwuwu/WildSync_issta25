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
if (_IN_BUFFER_SIZE < sizeof(gint) + sizeof(gint) + sizeof(gint) + sizeof(gint) + 1) {
	return 0;
}
gint __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(gint));
_BUF_POINTER += sizeof(gint);

gint __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(gint));
_BUF_POINTER += sizeof(gint);

gint __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(gint));
_BUF_POINTER += sizeof(gint);

gint __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(gint));
_BUF_POINTER += sizeof(gint);

size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

GdkPixbuf * pixbuf_sub;

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
// Extracted from: libgpod: src/itdb_artwork.c_0
// TODO: very hacky. Who knows whether this will work
int __gint = 5;
int __img_info__width = 5;
int __img_info__height = 5;
 gint pad_x = __fuzz_1;
 gint pad_y = __fuzz_2;
 gint width = __fuzz_3;
 gint height = __fuzz_0;
 width = width -abs ( pad_x );
 height = height -abs ( pad_y );
 if ( pad_x < 0 ){
}
 if ( pad_y < 0 ){
}
 if ( pad_x + width > __img_info__width ) width = __img_info__width - pad_x;
 if ( pad_y + height > __img_info__height ) height = __img_info__height - pad_y;
 pixbuf_sub =gdk_pixbuf_new_subpixbuf(pixbuf,  pad_x,  pad_y,  width,  height);

/**** End of extracted external code ****/
gdk_pixbuf_get_height(pixbuf);
g_object_unref(pixbuf);
}
free(buf);
g_object_unref(anim);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
