#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include "fuzzer_temp_file.h"
int glyph_range = 9;
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(double) + sizeof(double) + 1) {
	return 0;
}
double __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(double));
_BUF_POINTER += sizeof(double);

double __line__y1;
__line__y1 = __fuzz_0;
double __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(double));
_BUF_POINTER += sizeof(double);

double __line__x1;
__line__x1 = __fuzz_1;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size< glyph_range){
return 0;
}
cairo_t *cr;
cairo_surface_t *surface;
cairo_status_t status;
cairo_text_extents_t extents;
cairo_text_cluster_t cluster;
char *tmpfile = fuzzer_get_tmpfile(data, size);
surface = cairo_image_surface_create_from_png(tmpfile);
status = cairo_surface_status(surface);
if( status!= CAIRO_STATUS_SUCCESS){
fuzzer_release_tmpfile(tmpfile);
return 0;
}
char *buf = (char *) calloc(size + 1, sizeof(char));
memcpy(buf, data, size);
buf[size] = '\0';
cr = cairo_create(surface);
/**** Start of extracted external code ****/
// Extracted from: netsurf: frontends/gtk/print.c_0
// TODO: very hacky. Who knows whether this will work
int __style__stroke_type = 5;
 switch ( __style__stroke_type ){
}
cairo_line_to(cr,  __line__x1 + 0.5,  __line__y1 + 0.5);

/**** End of extracted external code ****/
cairo_text_extents(cr, buf, &extents);
cluster.num_bytes = size;
cluster.num_glyphs = 1;
for(int i=0;
 i< glyph_range;
 i++){
cairo_glyph_t glyph = {
            (long int)data[i], 10 * i, 25
        };
cairo_show_text_glyphs(cr, buf, size, &glyph, 1, &cluster, 1, 0);
}
cairo_destroy(cr);
cairo_surface_destroy(surface);
free(buf);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
