#include "../utils.h"

#include <libplacebo/dummy.h>
#include <libplacebo/shaders/custom.h>


int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
    pl_gpu gpu = pl_gpu_dummy_create(NULL, NULL);
    const struct pl_hook *hook;

    hook = pl_mpv_user_shader_parse(gpu, (char *) buf, len);
    pl_mpv_user_shader_destroy(&hook);

    pl_gpu_dummy_destroy(&gpu);

    return 0;
}
