#include <stdlib.h>
#include <stdio.h>
#include "autoconf.h"
#include <krb5.h>
#include <gssapi.h>
#include <string.h>
#define kMinInputLength 2
#define kMaxInputLength 1024
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

OM_uint32  major;

/**** End of fuzz variables initialization code ****/
gss_OID doid;
OM_uint32 minor, ret_flags, time_rec;
gss_name_t client = GSS_C_NO_NAME;
gss_ctx_id_t context = GSS_C_NO_CONTEXT;
gss_cred_id_t deleg_cred = GSS_C_NO_CREDENTIAL;
gss_buffer_desc data_in, data_out = GSS_C_EMPTY_BUFFER;
if( size< kMinInputLength|| size> kMaxInputLength)return 0;
data_in.length = size;
data_in.value = (void *)data;
gss_accept_sec_context(&minor, &context, GSS_C_NO_CREDENTIAL,
                           &data_in, GSS_C_NO_CHANNEL_BINDINGS, &client,
                           &doid, &data_out, &ret_flags, &time_rec,
                           &deleg_cred);
/**** Start of extracted external code ****/
// Extracted from: openconnect: gssapi.c_0
 major =gss_import_name(&minor, &data_in,  ( gss_OID ) GSS_C_NT_HOSTBASED_SERVICE, &client);

/**** End of extracted external code ****/
gss_release_buffer(&minor, &data_out);
if( context!= GSS_C_NO_CONTEXT)gss_delete_sec_context(&minor, &context, GSS_C_NO_BUFFER);
return 0;
}
