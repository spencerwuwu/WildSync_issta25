#include "libsoup/soup.h"
#include <stdint.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput(unsigned char  *_IN_BUFFER, size_t _IN_BUFFER_SIZE);

#ifdef FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION
static GLogWriterOutput
empty_logging_func (GLogLevelFlags log_level, const GLogField *fields,
                    gsize n_fields, gpointer user_data)
{
  return G_LOG_WRITER_HANDLED;
}
#endif

/* Disables logging for oss-fuzz. Must be used with each target. */
static void
fuzz_set_logging_func (void)
{
#ifdef FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION
  g_log_set_writer_func (empty_logging_func, NULL, NULL);
#endif
}

