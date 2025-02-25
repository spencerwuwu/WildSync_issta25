#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mxml.h>
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

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
// Extracted from: libomemo: src/libomemo.c_1
mxmlAdd(tree,  MXML_ADD_AFTER, tree, tree);

/**** End of extracted external code ****/
free(tree);
}
unlink(filename);
return 0;
}
