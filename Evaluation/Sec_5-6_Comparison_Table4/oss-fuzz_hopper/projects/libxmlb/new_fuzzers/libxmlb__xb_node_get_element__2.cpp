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
sprintf(filename, "/tmp/libfuzzer.xml");
FILE *fp = fopen(filename, "wb");
if(! fp)return 0;
fwrite(data, size/2, 1, fp);
fclose(fp);
size_t msg_len = size - size/2;
char* msg = malloc(msg_len+1);
memcpy(msg, data+size/2, msg_len);
msg[size/2] = '\0';
gchar **locales = g_get_language_names();
XbBuilder* builder = xb_builder_new();
XbSilo* silo = NULL;
GFile* file_dst = NULL;
GError* error = NULL;
for(guint i=0;
 locales[ i]!=NULL;
 i++){
xb_builder_add_locale(builder, locales[i]);

}
GFile* file = g_file_new_for_path(filename);
XbBuilderSource* source = xb_builder_source_new();
int ret = xb_builder_source_load_file(source,
				     file,
				     XB_BUILDER_SOURCE_FLAG_WATCH_FILE |
				     XB_BUILDER_SOURCE_FLAG_LITERAL_TEXT,
				     NULL,
				     &error);
if(! ret){
return 0;
}
xb_builder_import_source(builder, source);
file_dst = g_file_new_for_path("/tmp/libfuzzer.xmlb");
xb_builder_set_profile_flags(builder,
				 XB_SILO_PROFILE_FLAG_APPEND);
silo = xb_builder_ensure(builder,
			     file_dst,
			     XB_BUILDER_COMPILE_FLAG_WATCH_BLOB |
				 XB_BUILDER_COMPILE_FLAG_IGNORE_INVALID |
				 XB_BUILDER_COMPILE_FLAG_NATIVE_LANGS,
			     NULL,
			     &error);
if( silo){
GPtrArray* results = NULL;
XbQuery* query = NULL;
XbQueryContext* context = XB_QUERY_CONTEXT_INIT();
query = xb_query_new_full(silo, msg, XB_QUERY_FLAG_OPTIMIZE, &error);
if( query==NULL)return 0;
results = xb_silo_query_with_context(silo, query, &context, &error);
if(! results)return 0;
for(guint i=0;
 i< results->len;
 i++){
XbNode *n = g_ptr_array_index(results, i);
gchar *xml = NULL;
xml = xb_node_export(n,
			  XB_NODE_EXPORT_FLAG_FORMAT_MULTILINE |
			  XB_NODE_EXPORT_FLAG_FORMAT_INDENT,
			  &error);
/**** Start of extracted external code ****/
// Extracted from: fwupd: plugins/modem-manager/fu-firehose-updater.c_1
g_warn_if_fail (g_strcmp0 (xb_node_get_element(n),"response" ) == 0 );

/**** End of extracted external code ****/

}
}
return 0;
}
