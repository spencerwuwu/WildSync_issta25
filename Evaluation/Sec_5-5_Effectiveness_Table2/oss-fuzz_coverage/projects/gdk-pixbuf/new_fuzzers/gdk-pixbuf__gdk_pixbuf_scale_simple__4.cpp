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
if (_IN_BUFFER_SIZE < sizeof(size_t) + sizeof(size_t) + sizeof(size_t) + sizeof(size_t) + sizeof(int32_t) + sizeof(int32_t) + 1) {
	return 0;
}
size_t __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(size_t));
_BUF_POINTER += sizeof(size_t);

size_t processed_width;
processed_width = __fuzz_0;
size_t __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(size_t));
_BUF_POINTER += sizeof(size_t);

size_t processed_height;
processed_height = __fuzz_1;
size_t __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(size_t));
_BUF_POINTER += sizeof(size_t);

size_t height;
height = __fuzz_2;
size_t __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(size_t));
_BUF_POINTER += sizeof(size_t);

size_t width;
width = __fuzz_3;
int32_t __fuzz_4;
memcpy(&__fuzz_4, _IN_BUFFER + _BUF_POINTER, sizeof(int32_t));
_BUF_POINTER += sizeof(int32_t);

int32_t ow;
ow = __fuzz_4;
int32_t __fuzz_5;
memcpy(&__fuzz_5, _IN_BUFFER + _BUF_POINTER, sizeof(int32_t));
_BUF_POINTER += sizeof(int32_t);

int32_t oh;
oh = __fuzz_5;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<1){
return 0;
}
GdkPixbuf *pixbuf;
GError *error = NULL;
char *tmpfile = fuzzer_get_tmpfile(data, size);
pixbuf = gdk_pixbuf_new_from_file(tmpfile, &error);
if( error!=NULL){
g_clear_error(&error);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
char *buf = (char *) calloc(size + 1, sizeof(char));
memcpy(buf, data, size);
buf[size] = '\0';
gdk_pixbuf_get_width(pixbuf);
/**** Start of extracted external code ****/
// Extracted from: darktable: src/views/view.c_0
 int32_t w = 0, h = 0;
 if ( processed_width < processed_height ){
 h = height;
 w = processed_width * ( ( float ) h / ( float ) processed_height );
}
 else{
 w = width;
 h = processed_height * ( ( float ) w / ( float ) processed_width );
}
 if ( w > width ){
 w = width;
 h = h * ( ( float ) w / ( float ) ow );
}
 if ( h > height ){
 h = height;
 w = w * ( ( float ) h / ( float ) oh );
}
 GdkPixbuf * scaled =gdk_pixbuf_scale_simple(pixbuf,  w,  h,  GDK_INTERP_HYPER);

/**** End of extracted external code ****/
gdk_pixbuf_get_height(pixbuf);
gdk_pixbuf_get_bits_per_sample(pixbuf);
gdk_pixbuf_scale(pixbuf, pixbuf,
            0, 0, 
            gdk_pixbuf_get_width(pixbuf) / 4, 
            gdk_pixbuf_get_height(pixbuf) / 4,
            0, 0, 0.5, 0.5,
            GDK_INTERP_NEAREST);
unsigned int rot_amount = ((unsigned int) data[0]) % 4;
pixbuf = gdk_pixbuf_rotate_simple(pixbuf, rot_amount * 90);
gdk_pixbuf_set_option(pixbuf, buf, buf);
gdk_pixbuf_get_option(pixbuf, buf);
free(buf);
g_clear_object(&pixbuf);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
