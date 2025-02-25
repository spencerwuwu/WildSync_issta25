#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include "zlib.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t dataLen = _IN_REMAIN_LEN;

int  ret;

/**** End of fuzz variables initialization code ****/
uint32_t crc0 = crc32(0L, NULL, 0);
uint32_t crc1 = crc0;
uint32_t crc2 = crc0;
uint32_t adler0 = adler32(0L, NULL, 0);
uint32_t adler1 = adler0;
uint32_t adler2 = adler0;
uint32_t buffSize = data[0];
uint32_t offset = 0;
uint32_t op;
static size_t kMaxSize = 1024 * 1024;
if( dataLen<1|| dataLen> kMaxSize)return 0;
if( buffSize==0)++buffSize;
op = crc32_combine_gen(buffSize);
for( offset=0;
 offset+ buffSize<= dataLen;
 offset+= buffSize){
/**** Start of extracted external code ****/
// Extracted from: nodejs: zlib/contrib/tests/infcover.cc_0
 ret =inflateBack( Z_NULL,  Z_NULL, data + offset,  Z_NULL, data + offset);

/**** End of extracted external code ****/
uint32_t crc3 = crc32_z(crc0, data + offset, buffSize);
uint32_t crc4 = crc32_combine_op(crc1, crc3, op);
crc1 = crc32_z(crc1, data + offset, buffSize);
assert(crc1 == crc4);
}
crc1 = crc32_z(crc1, data + offset, dataLen % buffSize);
crc2 = crc32(crc2, data, (uint32_t) dataLen);
assert(crc1 == crc2);
assert(crc32_combine(crc1, crc2, dataLen) ==
         crc32_combine(crc1, crc1, dataLen));
op = crc32_combine_gen(dataLen);
assert(crc32_combine_op(crc1, crc2, op) ==
         crc32_combine_op(crc2, crc1, op));
assert(crc32_combine(crc1, crc2, dataLen) ==
         crc32_combine_op(crc2, crc1, op));
for( offset=0;
 offset+ buffSize<= dataLen;
 offset+= buffSize)adler1 = adler32_z(adler1, data + offset, buffSize);
adler1 = adler32_z(adler1, data + offset, dataLen % buffSize);
adler2 = adler32(adler2, data, (uint32_t) dataLen);
assert(adler1 == adler2);
assert(adler32_combine(adler1, adler2, dataLen) ==
         adler32_combine(adler1, adler1, dataLen));
return 0;
}
