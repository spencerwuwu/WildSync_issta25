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
if (_IN_BUFFER_SIZE < sizeof(int) + sizeof(char*) + sizeof(char*) + sizeof(char*) + sizeof(char*) + sizeof(char*) + 1) {
	return 0;
}
int __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(int));
_BUF_POINTER += sizeof(int);

int argc;
argc = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 6;

char* __fuzz_1 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_1_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_1_LEN;

char* inputfile;
inputfile = malloc(__fuzz_1_LEN + 1);
memcpy(inputfile, __fuzz_1, __fuzz_1_LEN);
inputfile[__fuzz_1_LEN] = 0;
char* __fuzz_2 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_2_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_2_LEN;

char* dumpfile;
dumpfile = malloc(__fuzz_2_LEN + 1);
memcpy(dumpfile, __fuzz_2, __fuzz_2_LEN);
dumpfile[__fuzz_2_LEN] = 0;
char* __fuzz_3 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_3_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_3_LEN;

char* hexdumpfilename;
hexdumpfilename = malloc(__fuzz_3_LEN + 1);
memcpy(hexdumpfilename, __fuzz_3, __fuzz_3_LEN);
hexdumpfilename[__fuzz_3_LEN] = 0;
char* __fuzz_4 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_4_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_4_LEN;

char* not_ip_dumpfile;
not_ip_dumpfile = malloc(__fuzz_4_LEN + 1);
memcpy(not_ip_dumpfile, __fuzz_4, __fuzz_4_LEN);
not_ip_dumpfile[__fuzz_4_LEN] = 0;
char* __fuzz_5 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_5_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_5_LEN;

char* bad_dns_dumpfile;
bad_dns_dumpfile = malloc(__fuzz_5_LEN + 1);
memcpy(bad_dns_dumpfile, __fuzz_5, __fuzz_5_LEN);
bad_dns_dumpfile[__fuzz_5_LEN] = 0;
const uint8_t* Data = _IN_BUFFER + _BUF_POINTER;
size_t Size = _IN_REMAIN_LEN;

int  i;
u_char * cur;

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
// Extracted from: ldns: examples/ldns-dpa.c_0
// TODO: very hacky. Who knows whether this will work
int __verbosity = 5;
int __errno = 5;
int __dumper = 5;
int __hexdumpfile = 5;
int __not_ip_dump = 5;
int __bad_dns_dump = 5;
 struct pcap_pkthdr cur_hdr;
 for ( i = 1;
 i < argc;
 i ++ ){
}
 if ( ! inputfile ){
}
 if ( __verbosity >= 5 ){
}
 if ( ! pkts ){
 if ( __errno != 0 ){
}
}
 if ( dumpfile ){
 if ( ! __dumper ){
}
}
 if ( hexdumpfilename ){
 if (strncmp ( hexdumpfilename,"-", 2 ) != 0 ){
}
 if ( ! __hexdumpfile ){
}
}
 if ( not_ip_dumpfile ){
 if ( ! __not_ip_dump ){
}
}
 if ( bad_dns_dumpfile ){
 if ( ! __bad_dns_dump ){
}
}
 while ( ( ( cur =pcap_next(pkts,  & cur_hdr) ) ) ){
}

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
