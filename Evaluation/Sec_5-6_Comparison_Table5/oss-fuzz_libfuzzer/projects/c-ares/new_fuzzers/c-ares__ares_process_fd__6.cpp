#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_nameser.h"
 int  LLVMFuzzerTestOneInput( unsigned char  * _IN_BUFFER , unsigned long _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
ares_channel *channel = NULL;
char           *csv;
ares_library_init(ARES_LIB_INIT_ALL);
ares_init(&channel);
csv = malloc(size + 1);
memcpy(csv, data, size);
csv[size] = '\0';
ares_set_servers_csv(channel, csv);
/**** Start of extracted external code ****/
// Extracted from: pgbouncer: src/dnslookup.c_0
ares_process_fd(channel,  ARES_SOCKET_BAD,  ARES_SOCKET_BAD);

/**** End of extracted external code ****/
free(csv);
ares_destroy(channel);
ares_library_cleanup();
return 0;
}
