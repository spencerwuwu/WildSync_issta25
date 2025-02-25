#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mxml.h>
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(char*) + sizeof(int) + 1) {
	return 0;
}
int __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int maxstrlen;
maxstrlen = __fuzz_1;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

char* __fuzz_0 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

char* par;
par = __fuzz_0;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
char filename[256];
sprintf(filename, "/tmp/libfuzzer.%d", getpid());
char filename2[256];
sprintf(filename, "/tmp/libfuzzer_s.%d", getpid());
FILE *fp = fopen(filename, "wb");
if(! fp){
return 0;
}
fwrite(data, size, 1, fp);
fclose(fp);
mxml_node_t *tree = NULL;
tree = mxmlLoadFilename(NULL, NULL, filename);
if( tree!=NULL){
mxmlSaveFilename(tree, NULL, filename2);
/**** Start of extracted external code ****/
// Extracted from: zynaddsubfx: src/Misc/XMLwrapper.cpp_0
snprintf ( par, maxstrlen,"%s",mxmlGetText(tree,  NULL) );

/**** End of extracted external code ****/
free(tree);
}
unlink(filename);
return 0;
}
