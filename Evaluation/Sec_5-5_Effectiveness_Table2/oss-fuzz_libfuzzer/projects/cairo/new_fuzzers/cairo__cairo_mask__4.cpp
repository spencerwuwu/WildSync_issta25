#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include "fuzzer_temp_file.h"
static cairo_surface_t *
acquire (cairo_pattern_t *pattern, void *closure,
	     cairo_surface_t *target,
	     cairo_rectangle_int_t *extents)
{
    return cairo_image_surface_create_from_png(closure);
}
static void
release (cairo_pattern_t *pattern, void *closure, cairo_surface_t *surface)
{
    cairo_surface_destroy(surface);
}
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int) + 1) {
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

int shadow_height;
shadow_height = __fuzz_1;
int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int shadow_width;
shadow_width = __fuzz_2;
int __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int width;
width = __fuzz_3;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
cairo_t *cr;
cairo_surface_t *surface;
cairo_pattern_t *pattern;
cairo_content_t content;
cairo_status_t status;
int w, h;
char *tmpfile = fuzzer_get_tmpfile(data, size);
surface = cairo_image_surface_create_from_png(tmpfile);
status = cairo_surface_status (surface);
if( status!= CAIRO_STATUS_SUCCESS){
fuzzer_release_tmpfile(tmpfile);
return 0;
}
cr = cairo_create(surface);
content = cairo_surface_get_content(surface);
w = cairo_image_surface_get_width(surface);
h = cairo_image_surface_get_height(surface);
char *buf = (char *) calloc(size + 1, sizeof(char));
memcpy(buf, data, size);
buf[size] = '\0';
pattern = cairo_pattern_create_raster_source(buf, content, w, h);
/**** Start of extracted external code ****/
// Extracted from: weston: shared/cairo-util.c_0
 if ( height < 2 * shadow_height ){
}
 if ( shadow_width > 0 && shadow_height ){
}
 if ( width < 2 * shadow_width ){
}
 if ( shadow_height > 0 && shadow_width ){
cairo_mask( cr, pattern);
}

/**** End of extracted external code ****/
cairo_raster_source_pattern_set_acquire (pattern, acquire, release);
cairo_set_source(cr, pattern);
cairo_pdf_surface_set_page_label(surface, buf);
cairo_pdf_surface_set_metadata(surface, CAIRO_PDF_METADATA_KEYWORDS, buf);
cairo_paint(cr);
cairo_destroy(cr);
cairo_pattern_destroy(pattern);
cairo_surface_destroy(surface);
free(buf);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
