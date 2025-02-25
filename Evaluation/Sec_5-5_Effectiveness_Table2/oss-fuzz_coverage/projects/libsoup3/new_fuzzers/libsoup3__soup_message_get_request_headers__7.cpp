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
fuzz_set_logging_func ();
GBytes *bytes = g_bytes_new (data, size);
SoupContentSniffer *sniffer = soup_content_sniffer_new ();
SoupMessage *msg = soup_message_new (SOUP_METHOD_GET, "https://example.org");
char *content_type = soup_content_sniffer_sniff (sniffer, msg, bytes, NULL);
/**** Start of extracted external code ****/
// Extracted from: gssdp: libgupnp/gupnp-service-proxy.c_1
 SoupMessageHeaders * request_headers =soup_message_get_request_headers(msg);

/**** End of extracted external code ****/
g_bytes_unref (bytes);
g_object_unref (sniffer);
g_object_unref (msg);
g_free (content_type);
return 0;
}
