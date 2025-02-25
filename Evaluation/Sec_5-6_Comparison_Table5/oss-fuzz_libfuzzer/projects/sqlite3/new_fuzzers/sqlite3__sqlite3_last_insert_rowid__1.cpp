#include <stdlib.h>
#include <stddef.h>
#if !defined(_MSC_VER)
# include <stdint.h>
#endif
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#if defined(_MSC_VER)
typedef unsigned char uint8_t;
#endif
static unsigned mDebug = 0;
#define FUZZ_SQL_TRACE       0x0001    
#define FUZZ_SHOW_MAX_DELAY  0x0002    
#define FUZZ_SHOW_ERRORS     0x0004    
void ossfuzz_set_debug_flags(unsigned x){
  mDebug = x;
}
static sqlite3_int64 timeOfDay(void){
  static sqlite3_vfs *clockVfs = 0;
  sqlite3_int64 t;
  if( clockVfs==0 ){
    clockVfs = sqlite3_vfs_find(0);
    if( clockVfs==0 ) return 0;
  }
  if( clockVfs->iVersion>=2 && clockVfs->xCurrentTimeInt64!=0 ){
    clockVfs->xCurrentTimeInt64(clockVfs, &t);
  }else{
    double r;
    clockVfs->xCurrentTime(clockVfs, &r);
    t = (sqlite3_int64)(r*86400000.0);
  }
  return t;
}
typedef struct FuzzCtx {
  sqlite3 *db;                
  sqlite3_int64 iCutoffTime;  
  sqlite3_int64 iLastCb;      
  sqlite3_int64 mxInterval;   
  unsigned nCb;               
  unsigned execCnt;           
} FuzzCtx;
static int progress_handler(void *pClientData) {
  FuzzCtx *p = (FuzzCtx*)pClientData;
  sqlite3_int64 iNow = timeOfDay();
  int rc = iNow>=p->iCutoffTime;
  sqlite3_int64 iDiff = iNow - p->iLastCb;
  if( iDiff > p->mxInterval ) p->mxInterval = iDiff;
  p->nCb++;
  return rc;
}
static int block_debug_pragmas(
  void *Notused,
  int eCode,
  char *zArg1,
  char *zArg2,
  char *zArg3,
  char *zArg4
){
  if( eCode==SQLITE_PRAGMA
   && (sqlite3_strnicmp("vdbe_", zArg1, 5)==0
        || sqlite3_stricmp("parser_trace", zArg1)==0)
  ){
    return SQLITE_DENY;
  }
  return SQLITE_OK;
}
static int exec_handler(void *pClientData, int argc, char **argv, char **namev){
  FuzzCtx *p = (FuzzCtx*)pClientData;
  int i;
  if( argv ){
    for(i=0; i<argc; i++) sqlite3_free(sqlite3_mprintf("%s", argv[i]));
  }
  return (p->execCnt--)<=0 || progress_handler(pClientData);
}
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
char *zErrMsg = 0;
uint8_t uSelector;
int rc;
char *zSql;
FuzzCtx cx;
memset(&cx, 0, sizeof(cx));
if( size<3)return 0;
if( data[1]=='\n'){
uSelector = data[0];
data += 2;
size -= 2;
}
else {
uSelector = 0xfd;
}
if( sqlite3_initialize())return 0;
rc = sqlite3_open_v2("fuzz.db", &cx.db,
           SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 0);
if( rc)return 0;
cx.iLastCb = timeOfDay();
cx.iCutoffTime = cx.iLastCb + 10000;
#ifndef  SQLITE_OMIT_PROGRESS_CALLBACKsqlite3_progress_handler(cx.db, 10, progress_handler, (void*)&cx);
#endif
sqlite3_limit(cx.db, SQLITE_LIMIT_VDBE_OP, 25000);
sqlite3_hard_heap_limit64(20000000);
sqlite3_limit(cx.db, SQLITE_LIMIT_LENGTH, 50000);
sqlite3_db_config(cx.db, SQLITE_DBCONFIG_ENABLE_FKEY, uSelector&1, &rc);
uSelector >>= 1;
sqlite3_set_authorizer(cx.db, block_debug_pragmas, 0);
cx.execCnt = uSelector + 1;
zSql = sqlite3_mprintf("%.*s", (int)size, data);
#ifndef  SQLITE_OMIT_COMPLETEsqlite3_complete(zSql);
#endif
sqlite3_exec(cx.db, zSql, exec_handler, (void*)&cx, &zErrMsg);
/**** Start of extracted external code ****/
// Extracted from: gogglesmm: src/GMDatabase.cpp_0
sqlite3_last_insert_rowid(cx.db);

/**** End of extracted external code ****/
if(( mDebug& FUZZ_SHOW_ERRORS)!=0&& zErrMsg){
printf("Error: %s\n", zErrMsg);
}
sqlite3_free(zErrMsg);
sqlite3_free(zSql);
sqlite3_exec(cx.db, "PRAGMA temp_store_directory=''", 0, 0, 0);
sqlite3_close(cx.db);
if( mDebug& FUZZ_SHOW_MAX_DELAY){
printf("Progress callback count....... %d\n", cx.nCb);
printf("Max time between callbacks.... %d ms\n", (int)cx.mxInterval);
}
return 0;
}
