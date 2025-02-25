#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <rfb/rfb.h>
static int initialized = 0;
rfbScreenInfoPtr server;
char *fakeargv[] = {"fuzz_server"};
extern size_t fuzz_offset;
extern size_t fuzz_size;
extern uint8_t *fuzz_data;
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(char*) + sizeof(char*) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int len;
len = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 3;

char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* rcmd;
rcmd = __fuzz_1;
char* __fuzz_2 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_2_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_2_LEN;

char* result;
result = __fuzz_2;
uint8_t* Data = _IN_BUFFER + _BUF_POINTER;
size_t Size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
if( initialized==0){
int fakeargc=1;
server=rfbGetScreen(&fakeargc,fakeargv,400,300,8,3,4);
server->frameBuffer=malloc(400*300*4);
rfbInitServer(server);
initialized = 1;
}
/**** Start of extracted external code ****/
// Extracted from: x11vnc: src/xevents.c_0
// TODO: very hacky. Who knows whether this will work
int __use_threads = 5;
int __client_connect_file = 5;
int __remote_prefix = 5;
 char * tmp = ( char * )calloc ( len + 8, 1 );
 if (strstr ( rcmd, " cmd= " ) != rcmd &&strstr ( rcmd, " qry= " ) != rcmd ){
}
 if ( __use_threads ){
 if ( __client_connect_file ){
}
}
 if ( result == NULL ){
}
 tmp = ( char * )calloc (strlen ( __remote_prefix ) +strlen ( result ) + 1, 1 );
rfbSendServerCutText(server,  tmp, strlen ( tmp ));

/**** End of extracted external code ****/
rfbClientPtr cl = rfbNewClient(server, RFB_INVALID_SOCKET - 1);
fuzz_data = Data;
fuzz_offset = 0;
fuzz_size = Size;
while( cl->sock!= RFB_INVALID_SOCKET){
rfbProcessClientMessage(cl);
}
rfbClientConnectionGone(cl);
return 0;
}
