#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "libcue.h"
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
char* str = (char*)malloc (size + 1);
if(! str)return -1;
memcpy (str, data, size);
str[size] = '\0';
Cd *cd = cue_parse_string (str);
/**** Start of extracted external code ****/
// Extracted from: localsearch: src/tracker-extract/tracker-cue-sheet.c_0
cd_get_cdtext ( cd ); cd_get_rem(cd);

/**** End of extracted external code ****/
cd_delete (cd);
free (str);
return 0;
}
