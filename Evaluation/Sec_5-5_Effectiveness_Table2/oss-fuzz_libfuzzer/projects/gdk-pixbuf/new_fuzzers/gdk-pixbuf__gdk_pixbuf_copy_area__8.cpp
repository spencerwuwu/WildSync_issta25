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

int dest_h_position;
dest_h_position = __fuzz_0;
int __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int width;
width = __fuzz_1;
int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int top_offset;
top_offset = __fuzz_2;
int __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int source_h_position;
source_h_position = __fuzz_3;
int __fuzz_4;
memcpy(&__fuzz_4, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __fuzz_5;
memcpy(&__fuzz_5, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int source_height;
source_height = __fuzz_5;
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
// Extracted from: nemo: eel/eel-graphic-effects.c_1
 int remaining_height, v_offset, slab_height;
 v_offset = 0;
 while ( remaining_height > 0 ){
 slab_height = __fuzz_4 > source_height ? source_height:
 remaining_height;
gdk_pixbuf_copy_area(pixbuf,  source_h_position,  top_offset,  width,  slab_height, pixbuf,  dest_h_position,  top_offset + v_offset);
 v_offset += slab_height;
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
