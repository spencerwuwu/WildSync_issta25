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

int height;
height = __fuzz_0;
int __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int left_offset;
left_offset = __fuzz_1;
int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int source_v_position;
source_v_position = __fuzz_2;
int __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int __fuzz_4;
memcpy(&__fuzz_4, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int dest_v_position;
dest_v_position = __fuzz_4;
int __fuzz_5;
memcpy(&__fuzz_5, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int source_width;
source_width = __fuzz_5;
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
// Extracted from: nemo: eel/eel-graphic-effects.c_0
 int remaining_width, h_offset, slab_width;
 h_offset = 0;
 while ( remaining_width > 0 ){
 slab_width = __fuzz_3 > source_width ? source_width:
 remaining_width;
gdk_pixbuf_copy_area(pixbuf,  left_offset,  source_v_position,  slab_width,  height, pixbuf,  left_offset + h_offset,  dest_v_position);
 h_offset += slab_width;
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
