#include "hpdf.h"
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
jmp_buf env;
#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler  (HPDF_STATUS   error_no,
                HPDF_STATUS   detail_no,
                void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

HPDF_STATUS  ret;

/**** End of fuzz variables initialization code ****/
char *uri  = malloc(size + 1);
memcpy(uri, data, size);
uri[size] = '\0';
HPDF_Doc  pdf;
HPDF_Font font;
HPDF_Page index_page;
HPDF_Page page[9];
HPDF_Destination dst;
HPDF_Rect rect;
HPDF_Point tp;
HPDF_Annotation annot;
HPDF_Outline root;
HPDF_Outline outline[1];
HPDF_UINT i;
pdf = HPDF_New (error_handler, NULL);
if(! pdf){
printf ("error: cannot create PdfDoc object\n");
return 1;
}
if( setjmp( env)){
HPDF_Free (pdf);
return 1;
}
font = HPDF_GetFont (pdf, "Helvetica", NULL);
/**** Start of extracted external code ****/
// Extracted from: vtk: ThirdParty/libharu/vtklibharu/src/hpdf_encoder_cns.c_0
// TODO: very hacky. Who knows whether this will work
int __pdf__mmgr = 5;
int __GBK_EUC_H_Init = 5;
int __GBK_EUC_V_Init = 5;
 HPDF_Encoder encoder;
 encoder =HPDF_CMapEncoder_New ( __pdf__mmgr,"GBK-EUC-H", __GBK_EUC_H_Init );
 encoder =HPDF_CMapEncoder_New ( __pdf__mmgr,"GBK-EUC-V", __GBK_EUC_V_Init );
 if ( ( ( ret =HPDF_Doc_RegisterEncoder(pdf,  encoder) ) != HPDF_OK ) ){
}

/**** End of extracted external code ****/
index_page = HPDF_AddPage (pdf);
HPDF_Page_SetWidth (index_page, 300);
HPDF_Page_SetHeight (index_page, 220);
HPDF_Page_BeginText (index_page);
HPDF_Page_SetFontAndSize (index_page, font, 10);
HPDF_Page_MoveTextPos (index_page, 15, 200);
HPDF_Page_ShowText (index_page, "Link Annotation Demo");
HPDF_Page_EndText (index_page);
tp = HPDF_Page_GetCurrentTextPos (index_page);
HPDF_Page_ShowText (index_page, "Jump to Page7 (bold border)");
rect.left = tp.x - 4;
rect.bottom = tp.y - 4;
rect.right = HPDF_Page_GetCurrentTextPos (index_page).x + 4;
rect.top = tp.y + 10;
HPDF_Page_MoveToNextLine (index_page);
dst = HPDF_Page_CreateDestination (page[6]);
annot = HPDF_Page_CreateLinkAnnot (index_page, rect, dst);
HPDF_LinkAnnot_SetBorderStyle (annot, 2, 0, 0);
root = HPDF_CreateOutline (pdf, NULL, "OutlineRoot", NULL);
HPDF_Outline_SetOpened (root, HPDF_TRUE);
outline[0] = HPDF_CreateOutline (pdf, root, "page1", NULL);
dst = HPDF_Page_CreateDestination (page[0]);
HPDF_Destination_SetXYZ(dst, 0, HPDF_Page_GetHeight(page[0]), 1);
HPDF_Outline_SetDestination(outline[0], dst);
tp = HPDF_Page_GetCurrentTextPos (index_page);
HPDF_Page_ShowText (index_page, "URI (");
HPDF_Page_ShowText (index_page, uri);
HPDF_Page_ShowText (index_page, ")");
rect.left = tp.x - 4;
rect.bottom = tp.y - 4;
rect.right = HPDF_Page_GetCurrentTextPos (index_page).x + 4;
rect.top = tp.y + 10;
HPDF_Page_CreateURILinkAnnot (index_page, rect, uri);
HPDF_Page_EndText (index_page);
HPDF_Free (pdf);
return 0;
}
