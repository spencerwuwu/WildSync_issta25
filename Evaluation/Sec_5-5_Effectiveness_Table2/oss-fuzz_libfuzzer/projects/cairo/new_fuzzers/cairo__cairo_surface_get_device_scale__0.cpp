#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include "fuzzer_temp_file.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
cairo_surface_t *image;
cairo_surface_t *surface;
cairo_status_t status;
cairo_format_t format;
char *tmpfile = fuzzer_get_tmpfile(data, size);
image = cairo_image_surface_create_from_png(tmpfile);
/**** Start of extracted external code ****/
// Extracted from: gjs: modules/cairo-surface.cpp_0
 double x_scale, y_scale;
cairo_surface_get_device_scale(image,  & x_scale,  & y_scale);

/**** End of extracted external code ****/
status = cairo_surface_status (image);
if( status!= CAIRO_STATUS_SUCCESS){
fuzzer_release_tmpfile(tmpfile);
return 0;
}
format = cairo_image_surface_get_format(image);
surface = cairo_image_surface_create_for_data((unsigned char*)data, format, 1, 1, size);
status = cairo_surface_status (surface);
if( status!= CAIRO_STATUS_SUCCESS){
cairo_surface_destroy(image);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
cairo_surface_write_to_png(surface, tmpfile);
cairo_surface_destroy(surface);
cairo_surface_destroy(image);
fuzzer_release_tmpfile(tmpfile);
return 0;
}
