#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pcap/pcap.h>
FILE * outfile = NULL;
static int bufferToFile(const char * name, const uint8_t *Data, size_t Size) {
    FILE * fd;
    if (remove(name) != 0) {
        if (errno != ENOENT) {
            printf("failed remove, errno=%d\n", errno);
            return -1;
        }
    }
    fd = fopen(name, "wb");
    if (fd == NULL) {
        printf("failed open, errno=%d\n", errno);
        return -2;
    }
    if (fwrite (Data, 1, Size, fd) != Size) {
        fclose(fd);
        return -3;
    }
    fclose(fd);
    return 0;
}
void fuzz_openFile(const char * name) {
    if (outfile != NULL) {
        fclose(outfile);
    }
    outfile = fopen(name, "w");
}
 int  LLVMFuzzerTestOneInput( const uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(struct pcap_pkthdr*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

struct pcap_pkthdr* __fuzz_0 = (struct pcap_pkthdr*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

struct pcap_pkthdr* h;
h = __fuzz_0;
const uint8_t* Data = _IN_BUFFER + _BUF_POINTER;
size_t Size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
pcap_t * pkts;
char errbuf[PCAP_ERRBUF_SIZE];
char filename[FILENAME_MAX] = { 0 };
const u_char *pkt;
struct pcap_pkthdr *header;
struct pcap_stat stats;
int fd = -1, r;
if( outfile==NULL){
outfile = fopen("/dev/null", "w");
if( outfile==NULL){
return 0;
}
}
snprintf(filename, FILENAME_MAX, "/tmp/libpcap_fuzz_pcap.XXXXXX");
if(( fd= mkstemp( filename))<0){
return 0;
}
close(fd);
if( bufferToFile( filename, Data, Size)<0){
unlink(filename);
return 0;
}
pkts = pcap_open_offline(filename, errbuf);
if( pkts==NULL){
fprintf(outfile, "Couldn't open pcap file %s\n", errbuf);
unlink(filename);
return 0;
}
r = pcap_next_ex(pkts, &header, &pkt);
/**** Start of extracted external code ****/
// Extracted from: masscan: src/stub-pcap.c_0
pcap_next(pkts,  h);

/**** End of extracted external code ****/
while( r>0){
fprintf(outfile, "packet length=%d/%d\n",header->caplen, header->len);
r = pcap_next_ex(pkts, &header, &pkt);
}
if( pcap_stats( pkts,& stats)==0){
fprintf(outfile, "number of packets=%d\n", stats.ps_recv);
}
pcap_close(pkts);
unlink(filename);
return 0;
}
