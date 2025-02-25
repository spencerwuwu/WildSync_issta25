#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lmdb.h"
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* input = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

uint  flags;

/**** End of fuzz variables initialization code ****/
if( size<5){
return 0;
}
char *new_str = (char *)malloc(size+1);
if( new_str==NULL){
return 0;
}
memcpy(new_str, input, size);
new_str[size] = '\0';
int rc;
MDB_env *env;
MDB_dbi dbi;
MDB_val key, data;
MDB_stat mst;
MDB_txn *txn;
MDB_cursor *cursor;
char sval[size+1];
rc = mdb_env_create(&env);
rc = mdb_env_open(env, "/tmp", 0, 0664);
rc = mdb_txn_begin(env, NULL, 0, &txn);
rc = mdb_dbi_open(txn, NULL, MDB_CREATE, &dbi);
key.mv_size = sizeof(int);
key.mv_data = sval;
data.mv_size = sizeof(sval);
data.mv_data = sval;
sprintf(sval, "%s", new_str);
rc = mdb_put(txn, dbi, &key, &data, 0);
rc = mdb_txn_commit(txn);
rc = mdb_env_stat(env, &mst);
if( rc){
goto leave;
}
rc = mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
rc = mdb_cursor_open(txn, dbi, &cursor);
rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT);
/**** Start of extracted external code ****/
// Extracted from: 389-ds-base: ldap/servers/slapd/back-ldbm/db-mdb/mdb_layer.c_2
 rc =mdb_dbi_flags (mdb_cursor_txn ( cursor ),mdb_cursor_dbi(cursor), & flags );

/**** End of extracted external code ****/
mdb_cursor_close(cursor);
mdb_txn_abort(txn);
leave:
	mdb_dbi_close(env, dbi);
mdb_env_close(env);
free(new_str);
return 0;
}
