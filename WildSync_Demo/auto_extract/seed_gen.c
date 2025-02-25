#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>


extern int errno;

void print_error() {
    // print which type of error have in a code 
    printf("Error Number % d\n", errno); 
    // print program detail "Success or failure" 
    perror("Program");
    exit(1);
}

int main(int argc, char *argv[]) {
    size_t inbuf_size = 0;
    //if (argc < 2) {
    //    return 0;
    //}

    inbuf_size = sizeof(long long) + sizeof(int) + 1;
    inbuf_size += 10; // just ramdomly picked
    char *inbuf = calloc(inbuf_size, sizeof(char));


    srand(time(NULL));   // Initialization, should only be called once.

    int fd = open("libass__ass_step_sub__fuzzer_seed_corpus/gen", O_RDWR | O_CREAT);
    if (fd < 0) {
        print_error();
        exit(1);
    }
    size_t buf_pointer = 0;

    long long long_0 = rand();
    memcpy(inbuf + buf_pointer, &long_0, sizeof(long long));
    buf_pointer += sizeof(long long);

    int int_0 = rand();
    memcpy(inbuf + buf_pointer, &int_0, sizeof(int));
    buf_pointer += sizeof(int);

    size_t written = 0;
    while (written < inbuf_size) {
        int ret = write(fd, inbuf + written, inbuf_size - written);
        if (ret >= 0) {
            written += ret;
        } else {
            print_error();
            exit(1);
        }
    }
    free(inbuf);
    close(fd);

    return 0;
}
