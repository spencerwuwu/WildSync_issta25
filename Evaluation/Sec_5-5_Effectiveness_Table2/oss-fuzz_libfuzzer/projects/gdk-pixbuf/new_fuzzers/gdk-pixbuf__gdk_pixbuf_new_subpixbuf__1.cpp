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
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __fuzz_4;
memcpy(&__fuzz_4, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int width;
width = __fuzz_4;
int __fuzz_5;
memcpy(&__fuzz_5, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int height;
height = __fuzz_5;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

int  offx;
int  offy;
GdkPixbuf * output_cropped;

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
// Extracted from: libavif: contrib/gdk-pixbuf/loader.c_0
// TODO: very hacky. Who knows whether this will work
int __image__clap_widthD = 5;
int __image__clap_heightD = 5;
int __image__clap_horizOffD = 5;
int __image__clap_vertOffD = 5;
 int new_width, new_height;
 new_width = ( int ) ( ( double ) ( __fuzz_0 ) / ( __image__clap_widthD ) + 0.5 );
 if ( new_width > width ){
 new_width = width;
}
 new_height = ( int ) ( ( double ) ( __fuzz_2 ) / ( __image__clap_heightD ) + 0.5 );
 if ( new_height > height ){
 new_height = height;
}
 if ( new_width > 0 && new_height > 0 ){
 int offx, offy;
 offx = ( ( double ) ( ( int32_t ) __fuzz_3 ) ) / ( __image__clap_horizOffD ) + ( width - new_width ) / 2.0 + 0.5;
 if ( offx < 0 ){
 offx = 0;
}
 else if ( offx > ( width - new_width ) ){
 offx = width - new_width;
}
 offy = ( ( double ) ( ( int32_t ) __fuzz_1 ) ) / ( __image__clap_vertOffD ) + ( height - new_height ) / 2.0 + 0.5;
 if ( offy < 0 ){
 offy = 0;
}
 else if ( offy > ( height - new_height ) ){
 offy = height - new_height;
}
 output_cropped =gdk_pixbuf_new_subpixbuf(pixbuf,  offx,  offy,  new_width,  new_height);
}

/**** End of extracted external code ****/
gdk_pixbuf_get_height(pixbuf);
g_object_unref(pixbuf);
}
free(buf);
g_object_unref(anim);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
