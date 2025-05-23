#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "config.h"
#ifdef HAVE_GIO_UNIX
#include <glib-unix.h>
#endif
#include <gio/gio.h>
#include <locale.h>
#include "xb-builder.h"
#include "xb-node.h"
#include "xb-silo-export.h"
#include "xb-silo-query.h"
typedef struct {
	GCancellable *cancellable;
	GMainLoop *loop;
	GPtrArray *cmd_array;
	gboolean force;
	gboolean wait;
	gboolean profile;
	gchar **tokenize;
} XbToolPrivate;
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
char filename[256];
sprintf(filename, "/tmp/libfuzzer.xmlb");
FILE *fp = fopen(filename, "wb");
if(! fp)return 0;
fwrite(data, size, 1, fp);
fclose(fp);
XbSiloLoadFlags flags = XB_SILO_LOAD_FLAG_NONE;
flags |= XB_SILO_LOAD_FLAG_NO_MAGIC;
gchar *str = NULL;
GError* error = NULL;
GFile* file = g_file_new_for_path(filename);
XbSilo* silo = xb_silo_new();
int ret = xb_silo_load_from_file(silo, file, flags, NULL, &error);
/**** Start of extracted external code ****/
// Extracted from: appstream: src/as-cache.c_0
// TODO: very hacky. Who knows whether this will work
int __node = 5;
 __node =xb_silo_get_root(silo);

/**** End of extracted external code ****/
if( ret){
str = xb_silo_to_string(silo, &error);

}
return 0;
}
