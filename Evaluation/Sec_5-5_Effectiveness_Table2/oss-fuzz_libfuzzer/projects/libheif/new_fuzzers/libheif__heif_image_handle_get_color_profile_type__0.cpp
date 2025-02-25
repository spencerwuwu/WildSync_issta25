#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include "libheif/heif.h"
static const enum heif_colorspace kFuzzColorSpace = heif_colorspace_YCbCr;
static const enum heif_chroma kFuzzChroma = heif_chroma_420;
static void TestDecodeImage(struct heif_context* ctx,
                            const struct heif_image_handle* handle, size_t filesize)
{
  struct heif_image* image = nullptr;
  struct heif_error err;

  bool primary = heif_image_handle_is_primary_image(handle);
  (void) primary;
  int width = heif_image_handle_get_width(handle);
  int height = heif_image_handle_get_height(handle);
  assert(width >= 0);
  assert(height >= 0);
  int metadata_count = heif_image_handle_get_number_of_metadata_blocks(handle, nullptr);
  assert(metadata_count >= 0);
  assert(static_cast<size_t>(metadata_count) < filesize / sizeof(heif_item_id));
  heif_item_id* metadata_ids = static_cast<heif_item_id*>(malloc(metadata_count * sizeof(heif_item_id)));
  assert(metadata_ids);
  int metadata_ids_count = heif_image_handle_get_list_of_metadata_block_IDs(handle, nullptr, metadata_ids,
                                                                            metadata_count);
  assert(metadata_count == metadata_ids_count);
  for (int i = 0; i < metadata_count; i++) {
    heif_image_handle_get_metadata_type(handle, metadata_ids[i]);
    heif_image_handle_get_metadata_content_type(handle, metadata_ids[i]);
    size_t metadata_size = heif_image_handle_get_metadata_size(handle, metadata_ids[i]);
    assert(metadata_size < filesize);
    uint8_t* metadata_data = static_cast<uint8_t*>(malloc(metadata_size));
    assert(metadata_data);
    heif_image_handle_get_metadata(handle, metadata_ids[i], metadata_data);
    free(metadata_data);
  }
  free(metadata_ids);

  err = heif_decode_image(handle, &image, kFuzzColorSpace, kFuzzChroma, nullptr);
  if (err.code != heif_error_Ok) {
    heif_image_release(image);
    return;
  }

  assert(heif_image_get_colorspace(image) == kFuzzColorSpace);
  assert(heif_image_get_chroma_format(image) == kFuzzChroma);

   

  heif_image_release(image);
}
static int clip_int(size_t size)
{
  return size > INT_MAX ? INT_MAX : static_cast<int>(size);
}
 extern "C"  int  LLVMFuzzerTestOneInput( const uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

const uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
struct heif_context* ctx;
struct heif_error err;
struct heif_image_handle* primary_handle = nullptr;
int images_count;
heif_item_id* image_IDs = NULL;
bool explicit_init = size == 0 || data[size - 1] & 1;
if( explicit_init){
heif_init(nullptr);
}
heif_check_filetype(data, clip_int(size));
heif_main_brand(data, clip_int(size));
heif_get_file_mime_type(data, clip_int(size));
ctx = heif_context_alloc();
assert(ctx);
err = heif_context_read_from_memory(ctx, data, size, nullptr);
if( err.code!= heif_error_Ok){
goto quit;
}
err = heif_context_get_primary_image_handle(ctx, &primary_handle);
if( err.code== heif_error_Ok){
/**** Start of extracted external code ****/
// Extracted from: gthumb: extensions/cairo_io/cairo-image-surface-avif.c_0
// TODO: very hacky. Who knows whether this will work
int __heif_color_profile_type_not_present = 5;
 if ( (heif_image_handle_get_color_profile_type(primary_handle) != __heif_color_profile_type_not_present ) ){
}

/**** End of extracted external code ****/
assert(heif_image_handle_is_primary_image(primary_handle));
TestDecodeImage(ctx, primary_handle, size);
heif_image_handle_release(primary_handle);
primary_handle = nullptr;
}
images_count = heif_context_get_number_of_top_level_images(ctx);
if(! images_count){
goto quit;
}
image_IDs = (heif_item_id*) malloc(images_count * sizeof(heif_item_id));
assert(image_IDs);
images_count = heif_context_get_list_of_top_level_image_IDs(ctx, image_IDs, images_count);
if(! images_count){
goto quit;
}
for(int i=0;
 i< images_count;
++ i){
struct heif_image_handle* image_handle = nullptr;
err = heif_context_get_image_handle(ctx, image_IDs[i], &image_handle);
if( err.code!= heif_error_Ok){
heif_image_handle_release(image_handle);
continue;
}
TestDecodeImage(ctx, image_handle, size);
int num_thumbnails = heif_image_handle_get_number_of_thumbnails(image_handle);
for(int t=0;
 t< num_thumbnails;
++ t){
struct heif_image_handle* thumbnail_handle = nullptr;
heif_image_handle_get_thumbnail(image_handle, t, &thumbnail_handle);
if( thumbnail_handle){
TestDecodeImage(ctx, thumbnail_handle, size);
heif_image_handle_release(thumbnail_handle);
}
}
heif_image_handle_release(image_handle);
}
quit:
  heif_image_handle_release(primary_handle);
heif_context_free(ctx);
free(image_IDs);
if( explicit_init){
heif_deinit();
}
return 0;
}
