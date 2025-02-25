#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include "config/aom_config.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom_ports/mem_ops.h"
#define IVF_FRAME_HDR_SZ (4 + 8)  
#define IVF_FILE_HDR_SZ 32
#   define VLC_FOURCC( a, b, c, d ) \
        ( ((uint32_t)a) | ( ((uint32_t)b) << 8 ) \
           | ( ((uint32_t)c) << 16 ) | ( ((uint32_t)d) << 24 ) )
#define VLC_CODEC_I420_10L        VLC_FOURCC('I','0','A','L')
extern "C" void usage_exit(void) { exit(EXIT_FAILURE); }
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(unsigned) + sizeof(unsigned) + sizeof(aom_img_fmt_t) + sizeof(unsigned char*) + 1) {
	return 0;
}
unsigned __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(unsigned));
_BUF_POINTER += sizeof(unsigned);

unsigned __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(unsigned));
_BUF_POINTER += sizeof(unsigned);

aom_img_fmt_t __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(aom_img_fmt_t));
_BUF_POINTER += sizeof(aom_img_fmt_t);

size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

unsigned char* __fuzz_3 = (unsigned char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_3_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_3_LEN;

unsigned char* __p_pict__p0_p_pixels;
__p_pict__p0_p_pixels = __fuzz_3;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( size<= IVF_FILE_HDR_SZ){
return 0;
}
unsigned int tile_mode = (data[IVF_FILE_HDR_SZ - 1] & 2) != 0;
unsigned int ext_tile_debug = (data[IVF_FILE_HDR_SZ - 1] & 4) != 0;
unsigned int is_annexb = (data[IVF_FILE_HDR_SZ - 1] & 8) != 0;
int output_all_layers = (data[IVF_FILE_HDR_SZ - 1] & 0x10) != 0;
int operating_point = data[IVF_FILE_HDR_SZ - 2] & 0x1F;
aom_codec_iface_t *codec_interface = aom_codec_av1_dx();
aom_codec_ctx_t codec;
unsigned int threads = (data[IVF_FILE_HDR_SZ] & 0x3f) + 1;
aom_codec_dec_cfg_t cfg = { threads, 0, 0, !FORCE_HIGHBITDEPTH_DECODING };
if( aom_codec_dec_init(& codec, codec_interface,& cfg,0)){
return 0;
}
AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1_SET_TILE_MODE, tile_mode);
AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_EXT_TILE_DEBUG, ext_tile_debug);
AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_IS_ANNEXB, is_annexb);
AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_OUTPUT_ALL_LAYERS,
                                output_all_layers);
AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_OPERATING_POINT,
                                operating_point);
data += IVF_FILE_HDR_SZ;
size -= IVF_FILE_HDR_SZ;
while( size> IVF_FRAME_HDR_SZ){
size_t frame_size = mem_get_le32(data);
size -= IVF_FRAME_HDR_SZ;
data += IVF_FRAME_HDR_SZ;
frame_size = std::min(size, frame_size);
aom_codec_err_t err =
        aom_codec_decode(&codec, data, frame_size, nullptr);
static_cast<void>(err);
aom_codec_iter_t iter = nullptr;
aom_image_t *img = nullptr;
while(( img= aom_codec_get_frame(& codec,& iter))!=nullptr){

}
/**** Start of extracted external code ****/
// Extracted from: vlc: modules/codec/aom.c_0
// TODO: very hacky. Who knows whether this will work
int __aom_img_fmt_t = 5;
 unsigned i_w = __fuzz_1;
 unsigned i_h = __fuzz_0;
 aom_img_fmt_t img_fmt = __fuzz_2 == VLC_CODEC_I420_10L ? AOM_IMG_FMT_I42016:
 AOM_IMG_FMT_I420;
 if ( ( !aom_img_wrap(img,  img_fmt,  i_w,  i_h,  1,  __p_pict__p0_p_pixels) ) ){
}
{
}
{
}

/**** End of extracted external code ****/
data += frame_size;
size -= frame_size;
}
aom_codec_destroy(&codec);
return 0;
}
