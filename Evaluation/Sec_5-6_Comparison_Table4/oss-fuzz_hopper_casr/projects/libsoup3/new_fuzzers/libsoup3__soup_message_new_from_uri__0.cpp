#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "new_fuzz.h"
 int  LLVMFuzzerTestOneInput( unsigned char  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if(! size|| data[ size-1]!='\0')return 0;
fuzz_set_logging_func ();
SoupCookie *cookie = soup_cookie_parse ((char*)data, NULL);
/**** Start of extracted external code ****/
// Extracted from: grilo: libs/net/grl-net-wc.c_0
// TODO: very hacky. Who knows whether this will work
int __rr__message = 5;
 __rr__message =soup_message_new_from_uri( SOUP_METHOD_GET, NULL);

/**** End of extracted external code ****/
g_clear_pointer (&cookie, soup_cookie_free);
return 0;
}
