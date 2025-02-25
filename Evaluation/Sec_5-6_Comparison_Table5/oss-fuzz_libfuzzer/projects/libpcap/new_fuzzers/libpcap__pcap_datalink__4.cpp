#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap/pcap.h>
void fuzz_openFile(const char * name){
     
}
 int  LLVMFuzzerTestOneInput( const uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(unsigned int*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

unsigned int* __fuzz_0 = (unsigned int*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

unsigned int* linktype;
linktype = __fuzz_0;
const uint8_t* Data = _IN_BUFFER + _BUF_POINTER;
size_t Size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
pcap_t * pkts;
struct bpf_program bpf;
char * filter;
if( Size<1){
return 0;
}
pkts = pcap_open_dead(Data[Size-1], 0xFFFF);
if( pkts==NULL){
printf("pcap_open_dead failed\n");
return 0;
}
filter = malloc(Size);
memcpy(filter, Data, Size);
filter[Size-1] = 0;
if( pcap_compile( pkts,& bpf, filter,1, PCAP_NETMASK_UNKNOWN)==0){
/**** Start of extracted external code ****/
// Extracted from: etherape: src/capture/capctl.c_0
 * linktype =pcap_datalink(pkts);

/**** End of extracted external code ****/
pcap_setfilter(pkts, &bpf);
pcap_close(pkts);
pcap_freecode(&bpf);
}
else {
pcap_close(pkts);
}
free(filter);
return 0;
}
