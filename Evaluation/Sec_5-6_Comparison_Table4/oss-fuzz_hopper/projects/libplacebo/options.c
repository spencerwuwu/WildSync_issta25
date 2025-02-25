#include "../utils.h"

#include <libplacebo/options.h>


int LLVMFuzzerTestOneInput(uint8_t *data, size_t size) {
    pl_options opts = pl_options_alloc(NULL);
    data[size - 1] = '\0'; // ensure proper null termination
    pl_options_load(opts, (const char *) data);
    pl_options_save(opts);
    pl_options_reset(opts, NULL);
    return 0;
}
