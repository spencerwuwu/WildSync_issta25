#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdint>
#include <vorbis/vorbisfile.h>
struct vorbis_data {
  uint8_t *current;
  uint8_t *data;
  size_t size;
}
;
size_t read_func(void *ptr, size_t size1, size_t size2, void *datasource) {
  vorbis_data* vd = (vorbis_data *)(datasource);
  size_t len = size1 * size2;
  if (vd->current + len > vd->data + vd->size) {
      len = vd->data + vd->size - vd->current;
  }
  memcpy(ptr, vd->current, len);
  vd->current += len;
  return len;
}
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* Data = _IN_BUFFER + _BUF_POINTER;
size_t Size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
ov_callbacks memory_callbacks = {0};
memory_callbacks.read_func = read_func;
vorbis_data data_st;
data_st.size = Size;
data_st.current = Data;
data_st.data = Data;
OggVorbis_File vf;
int result = ov_open_callbacks(&data_st, &vf, NULL, 0, memory_callbacks);
if( result<0){
return 0;
}
int current_section = 0;
int eof = 0;
char buf[4096];
int read_result;
while(! eof){
/**** Start of extracted external code ****/
// Extracted from: libretro-genesis-plus-gx: core/tremor/vorbisfile.c_1
// TODO: very hacky. Who knows whether this will work
int __vf__pcm_offset = 5;
 __vf__pcm_offset =ov_pcm_total(&vf,  -1);

/**** End of extracted external code ****/
read_result = ov_read(&vf, buf, sizeof(buf), 0, 2, 1, &current_section);
if( read_result!= OV_HOLE&& read_result<=0){
eof = 1;
}
}
ov_clear(&vf);
return 0;
}
