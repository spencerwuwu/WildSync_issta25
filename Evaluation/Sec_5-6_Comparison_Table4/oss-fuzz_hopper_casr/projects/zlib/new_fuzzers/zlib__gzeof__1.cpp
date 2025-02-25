#include "zlib.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#define GZ_SUFFIX ".gz"
#define SUFFIX_LEN (sizeof(GZ_SUFFIX)-1)
#define BUFLEN      16384         
#define BUFLENW     (BUFLEN * 3)  
#define MAX_NAME_LEN 1024
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t dataLen = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
char *inFileName = "/tmp/minigzip_fuzzer.out";
char *outFileName = "/tmp/minigzip_fuzzer.out.gz";
char outmode[20];
FILE *in;
char buf[BUFLEN];
uint32_t offset = 0;
static size_t kMaxSize = 1024 * 1024;
if( dataLen<1|| dataLen> kMaxSize)return 0;
in = fopen(inFileName, "w");
if( fwrite( data,1,(unsigned) dataLen, in)!= dataLen)return 0;
if( fclose( in))return 0;
memset(outmode, 0, sizeof(outmode));
snprintf(outmode, sizeof(outmode), "%s", "wb");
outmode[2] = data[0] % 10;
uint8_t cmp = data[0];
if( cmp%4==0){
outmode[3] = 0;
}
else if( cmp%4==1){
outmode[3] = 'f';
}
else if( cmp%4==2){
outmode[3] = 'h';
}
else if( cmp%4==3){
outmode[3] = 'R';
}
{
char outfile[MAX_NAME_LEN];
FILE  *in;
gzFile out;
if( strlen( inFileName)+ strlen( GZ_SUFFIX)>=sizeof( outfile)){
goto end_compress;
}
snprintf(outfile, sizeof(outfile), "%s%s", inFileName, GZ_SUFFIX);
in = fopen(inFileName, "rb");
if( in==NULL){
goto end_compress;
}
out = gzopen(outfile, outmode);
if( out==NULL){
goto end_compress;
}
{
char buf[BUFLEN];
int len;
int err;
memset(buf, 0, sizeof(buf));
for(;
;
){
len = (int)fread(buf, 1, sizeof(buf), in);
if( ferror( in)){
goto end_compress;
}
if( len==0)break;
if( gzwrite( out, buf,(unsigned) len)!= len)return 0;
}
fclose(in);
/**** Start of extracted external code ****/
// Extracted from: gambas3-gb-compress: gb.compress.zlib/src/main.c_1
gzeof(out);

/**** End of extracted external code ****/
if( gzclose( out)!= Z_OK)return 0;
}
unlink(inFileName);
goto end_compress;
}
end_compress:
     
    {
        char buf[MAX_NAME_LEN];
        char *infile, *outfile;
        FILE  *out;
        gzFile in;
        size_t len = strlen(outFileName);

        if (len + strlen(GZ_SUFFIX) >= sizeof(buf)) {
            goto end_uncompress;
        }

        snprintf(buf, sizeof(buf), "%s", outFileName);

        if (len > SUFFIX_LEN && strcmp(outFileName+len-SUFFIX_LEN, GZ_SUFFIX) == 0) {
            infile = outFileName;
            outfile = buf;
            outfile[len-3] = '\0';
        } else {
            outfile = outFileName;
            infile = buf;
            snprintf(buf + len, sizeof(buf) - len, "%s", GZ_SUFFIX);
        }
        in = gzopen(infile, "rb");
        if (in == NULL) {
            goto end_uncompress;
        }
        out = fopen(outfile, "wb");
        if (out == NULL) {
            perror(outFileName);
        }

         
        {
            char buf[BUFLENW];
            int len;
            int err;

            for (;;) {
                len = gzread(in, buf, sizeof(buf));
                if (len < 0) error (gzerror(in, &err));
                if (len == 0) break;

                if ((int)fwrite(buf, 1, (unsigned)len, out) != len) {
                    return 0;
                }
            }
            if (fclose(out)) 
                return 0;

            if (gzclose(in) != Z_OK) 
                return 0;
        }

        unlink(infile);
    }
end_uncompress:

     
    in = fopen(inFileName, "rb");
if( in==NULL){
perror(inFileName);
return 0;
}
memset(buf, 0, sizeof(buf));
for(;
;
){
int len = (int)fread(buf, 1, sizeof(buf), in);
if( ferror( in)){
perror("fread");
return 0;
}
if( len==0)break;
assert(0 == memcmp(data + offset, buf, len));
offset += len;
}
if( fclose( in))return 0;
return 0;
}
