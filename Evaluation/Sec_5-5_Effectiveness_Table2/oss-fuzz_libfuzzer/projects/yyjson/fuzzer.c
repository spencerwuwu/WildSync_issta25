#include <yyjson.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    yyjson_read_flag rflags[2] = {YYJSON_READ_NOFLAG,
                    YYJSON_READ_ALLOW_TRAILING_COMMAS |
                    YYJSON_READ_ALLOW_COMMENTS |
                    YYJSON_READ_ALLOW_INF_AND_NAN};
    yyjson_write_flag wflags[2] = {YYJSON_WRITE_NOFLAG, 
                    YYJSON_WRITE_PRETTY |
                    YYJSON_WRITE_ESCAPE_UNICODE |
                    YYJSON_WRITE_ESCAPE_SLASHES |
                    YYJSON_WRITE_ALLOW_INF_AND_NAN};
    for (int i=0; i < 2; i++) {
        yyjson_read_flag rflag = rflags[i];
        yyjson_read_flag wflag = wflags[i];

        yyjson_doc *idoc = yyjson_read((const char *)data, size, rflag);
        yyjson_mut_doc *mdoc = yyjson_doc_mut_copy(idoc, NULL);
        char *ijson = yyjson_write(idoc, wflag, NULL);
        if (ijson) free((void *)ijson);
        char *mjson = yyjson_mut_write(mdoc, wflag, NULL);
        if (mjson) free((void *)mjson);
        yyjson_doc_free(idoc);
        yyjson_mut_doc_free(mdoc);

    }
    return 0;
}
