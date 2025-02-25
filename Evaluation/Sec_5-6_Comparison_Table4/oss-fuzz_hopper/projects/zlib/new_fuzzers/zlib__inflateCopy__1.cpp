#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include "zlib.h"
#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        fprintf(stderr, "%s error: %d\n", msg, err); \
        return 0; \
    } \
}
static uint8_t *data;
static size_t dataLen;
static alloc_func zalloc = NULL;
static free_func zfree = NULL;
int test_deflate(unsigned char *compr, size_t comprLen) {
  z_stream c_stream;  
  int err;
  unsigned long len = dataLen;

  c_stream.zalloc = zalloc;
  c_stream.zfree = zfree;
  c_stream.opaque = (void *)0;

  err = deflateInit(&c_stream, Z_DEFAULT_COMPRESSION);
  CHECK_ERR(err, "deflateInit");

  c_stream.next_in = (Bytef *)data;
  c_stream.next_out = compr;

  while (c_stream.total_in != len && c_stream.total_out < comprLen) {
    c_stream.avail_in = c_stream.avail_out = 1;  
    err = deflate(&c_stream, Z_NO_FLUSH);
    CHECK_ERR(err, "deflate small 1");
  }
   
  for (;;) {
    c_stream.avail_out = 1;
    err = deflate(&c_stream, Z_FINISH);
    if (err == Z_STREAM_END)
      break;
    CHECK_ERR(err, "deflate small 2");
  }

  err = deflateEnd(&c_stream);
  CHECK_ERR(err, "deflateEnd");
  return 0;
}
int test_inflate(unsigned char *compr, size_t comprLen, unsigned char *uncompr,
                  size_t uncomprLen) {
  int err;
  z_stream d_stream;  

  d_stream.zalloc = zalloc;
  d_stream.zfree = zfree;
  d_stream.opaque = (void *)0;

  d_stream.next_in = compr;
  d_stream.avail_in = 0;
  d_stream.next_out = uncompr;

  err = inflateInit(&d_stream);
  CHECK_ERR(err, "inflateInit");

  while (d_stream.total_out < uncomprLen && d_stream.total_in < comprLen) {
    d_stream.avail_in = d_stream.avail_out = 1;  
    err = inflate(&d_stream, Z_NO_FLUSH);
    if (err == Z_STREAM_END)
      break;
    CHECK_ERR(err, "inflate");
  }

  err = inflateEnd(&d_stream);
  CHECK_ERR(err, "inflateEnd");

  if (memcmp(uncompr, data, dataLen)) {
    fprintf(stderr, "bad inflate\n");
    return 0;
  }
  return 0;
}
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* d = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

int  ret;

/**** End of fuzz variables initialization code ****/
size_t comprLen = compressBound(size);
size_t uncomprLen = size;
uint8_t *compr, *uncompr;
static size_t kMaxSize = 1024 * 1024;
if( size<1|| size> kMaxSize)return 0;
data = d;
dataLen = size;
compr = (uint8_t *)calloc(1, comprLen);
uncompr = (uint8_t *)calloc(1, uncomprLen);
{
z_stream c_stream;
int err;
unsigned long len = dataLen;
c_stream.zalloc = zalloc;
c_stream.zfree = zfree;
c_stream.opaque = (void *)0;
err = deflateInit(&c_stream, Z_DEFAULT_COMPRESSION);
CHECK_ERR(err, "deflateInit");
c_stream.next_in = (Bytef *)data;
c_stream.next_out = compr;
while( c_stream.total_in!= len&& c_stream.total_out< comprLen){
c_stream.avail_in = c_stream.avail_out = 1;
/**** Start of extracted external code ****/
// Extracted from: ghostscript: zlib/test/infcover.c_1
 ret =inflateCopy(&c_stream, &c_stream);

/**** End of extracted external code ****/
err = deflate(&c_stream, Z_NO_FLUSH);
CHECK_ERR(err, "deflate small 1");
}
for(;
;
){
c_stream.avail_out = 1;
err = deflate(&c_stream, Z_FINISH);
if( err== Z_STREAM_END)break;
CHECK_ERR(err, "deflate small 2");
}
err = deflateEnd(&c_stream);
CHECK_ERR(err, "deflateEnd");
}
end_test_deflate:
   
   
  {
  int err;
  z_stream d_stream;  

  d_stream.zalloc = zalloc;
  d_stream.zfree = zfree;
  d_stream.opaque = (void *)0;

  d_stream.next_in = compr;
  d_stream.avail_in = 0;
  d_stream.next_out = uncompr;

  err = inflateInit(&d_stream);
  CHECK_ERR(err, "inflateInit");

  while (d_stream.total_out < uncomprLen && d_stream.total_in < comprLen) {
    d_stream.avail_in = d_stream.avail_out = 1;  
    err = inflate(&d_stream, Z_NO_FLUSH);
    if (err == Z_STREAM_END)
      break;
    CHECK_ERR(err, "inflate");
  }

  err = inflateEnd(&d_stream);
  CHECK_ERR(err, "inflateEnd");

  if (memcmp(uncompr, data, dataLen)) {
    fprintf(stderr, "bad inflate\n");
     
    goto end_test_inflate;
  }

  }
end_test_inflate:

  free(compr);
free(uncompr);
return 0;
}
