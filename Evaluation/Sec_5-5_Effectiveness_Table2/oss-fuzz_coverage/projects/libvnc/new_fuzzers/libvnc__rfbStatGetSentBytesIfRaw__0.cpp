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
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* Data = _IN_BUFFER + _BUF_POINTER;
size_t Size = _IN_REMAIN_LEN;

int  rbs;

/**** End of fuzz variables initialization code ****/
if( initialized==0){
int fakeargc=1;
server=rfbGetScreen(&fakeargc,fakeargv,400,300,8,3,4);
server->frameBuffer=malloc(400*300*4);
rfbInitServer(server);
initialized = 1;
}
rfbClientPtr cl = rfbNewClient(server, RFB_INVALID_SOCKET - 1);
fuzz_data = Data;
fuzz_offset = 0;
fuzz_size = Size;
while( cl->sock!= RFB_INVALID_SOCKET){
/**** Start of extracted external code ****/
// Extracted from: x11vnc: src/rates.c_0
 rbs =rfbStatGetSentBytesIfRaw(cl);

/**** End of extracted external code ****/
rfbProcessClientMessage(cl);
}
rfbClientConnectionGone(cl);
return 0;
}
