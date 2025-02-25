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
if (_IN_BUFFER_SIZE < sizeof(char*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

char* __fuzz_0 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

char* fname;
fname = malloc(__fuzz_0_LEN + 1);
memcpy(fname, __fuzz_0, __fuzz_0_LEN);
fname[__fuzz_0_LEN] = 0;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t dataLen = _IN_REMAIN_LEN;


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
/**** Start of extracted external code ****/
// Extracted from: mold: third-party/zlib/test/example.c_0
 gzFile file;
 file =gzopen ( fname,"wb" );
 if ( file == NULL ){
}
 if (gzputs ( file,"ello" ) != 4 ){
}
 if (gzprintf ( file,", %s!","hello" ) != 8 ){
}
gzseek( file, buffSize,  SEEK_CUR);

/**** End of extracted external code ****/
op = crc32_combine_gen(buffSize);
for( offset=0;
 offset+ buffSize<= dataLen;
 offset+= buffSize){
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
