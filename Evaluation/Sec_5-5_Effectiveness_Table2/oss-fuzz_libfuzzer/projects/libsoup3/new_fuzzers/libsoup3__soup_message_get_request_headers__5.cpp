#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "new_fuzz.h"
 int  LLVMFuzzerTestOneInput( unsigned char  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(char*) + sizeof(char*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 3;

char* __fuzz_0 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

char* key;
key = __fuzz_0;
char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* value;
value = __fuzz_1;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
fuzz_set_logging_func ();
GBytes *bytes = g_bytes_new (data, size);
SoupContentSniffer *sniffer = soup_content_sniffer_new ();
SoupMessage *msg = soup_message_new (SOUP_METHOD_GET, "https://example.org");
char *content_type = soup_content_sniffer_sniff (sniffer, msg, bytes, NULL);
/**** Start of extracted external code ****/
// Extracted from: grilo: libs/net/grl-net-wc.c_0
soup_message_headers_append (soup_message_get_request_headers(msg), key, value );

/**** End of extracted external code ****/
g_bytes_unref (bytes);
g_object_unref (sniffer);
g_object_unref (msg);
g_free (content_type);
return 0;
}
