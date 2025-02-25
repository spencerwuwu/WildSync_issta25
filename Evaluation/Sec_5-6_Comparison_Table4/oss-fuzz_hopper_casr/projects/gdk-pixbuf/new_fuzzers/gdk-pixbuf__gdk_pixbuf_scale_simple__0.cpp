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
if (_IN_BUFFER_SIZE < sizeof(gdouble) + 1) {
	return 0;
}
gdouble __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(gdouble));
_BUF_POINTER += sizeof(gdouble);

size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

gint  i_width;
gint  i_height;

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
// Extracted from: eog: src/eog-print-preview.c_0
// TODO: very hacky. Who knows whether this will work
int __gdouble = 5;
int __priv__image_scaled = 5;
int __allocation_width = 5;
int __allocation_height = 5;
int __priv__area = 5;
 if ( ! __priv__image_scaled ){
 gint i_width, i_height;
 i_width =gdk_pixbuf_get_width ( pixbuf );
 i_height =gdk_pixbuf_get_height ( pixbuf );
 if ( ( i_width > __allocation_width ) || ( i_height > __allocation_height ) ){
 gdouble scale;
 scale = __fuzz_0;
 scale *=gtk_widget_get_scale_factor (GTK_WIDGET ( __priv__area ) );
 __priv__image_scaled =gdk_pixbuf_scale_simple(pixbuf,  i_width * scale,  i_height * scale,  GDK_INTERP_TILES);
}
}

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
