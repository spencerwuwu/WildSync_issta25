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
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(sraRegionPtr) + sizeof(int) + sizeof(int) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int dy;
dy = __fuzz_0;
sraRegionPtr __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(sraRegionPtr));
_BUF_POINTER += sizeof(sraRegionPtr);

sraRegionPtr region;
region = __fuzz_1;
int __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int dx;
dx = __fuzz_2;
int __fuzz_3;
memcpy(&__fuzz_3, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int mode;
mode = __fuzz_3;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

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
// Extracted from: x11vnc: src/userinput.c_0
// TODO: very hacky. Who knows whether this will work
int __sraRegionPtr = 5;
int __rfb_fb = 5;
int __main_fb = 5;
int __rotating = 5;
int __DCR_Normal = 5;
int __debug_scroll = 5;
 if ( __rfb_fb == __main_fb && ! __rotating && mode == __DCR_Normal ){
 if ( 0 || __debug_scroll > 1 ){
}
rfbDoCopyRegion(server,  region,  dx,  dy);
}

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
