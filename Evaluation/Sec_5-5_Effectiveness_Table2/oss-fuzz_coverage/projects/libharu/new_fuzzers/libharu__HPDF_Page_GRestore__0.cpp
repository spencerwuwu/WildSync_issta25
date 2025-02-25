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


/**** End of fuzz variables initialization code ****/
char *samp_text  = malloc(size + 1);
memcpy(samp_text, data, size);
samp_text[size] = '\0';
HPDF_Doc  pdf;
HPDF_Font font;
HPDF_Page page;
float tw;
float fsize;
pdf = HPDF_New (error_handler, NULL);
if(! pdf){
printf ("error: cannot create PdfDoc object\n");
return 1;
}
if( setjmp( env)){
HPDF_Free (pdf);
return 1;
}
HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
font = HPDF_GetFont (pdf, "Helvetica", NULL);
page = HPDF_AddPage (pdf);
char *page_title = "Text Demo";
HPDF_Page_SetFontAndSize (page, font, 24);
tw = HPDF_Page_TextWidth (page, page_title);
/**** Start of extracted external code ****/
// Extracted from: blender: source/blender/io/grease_pencil/intern/grease_pencil_io_export_pdf.cc_0
HPDF_Page_GRestore(page);

/**** End of extracted external code ****/
HPDF_Page_BeginText (page);
HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                HPDF_Page_GetHeight (page) - 50, page_title);
HPDF_Page_EndText (page);
HPDF_Page_BeginText (page);
HPDF_Page_MoveTextPos (page, 60, HPDF_Page_GetHeight(page) - 60);
fsize = 8;
while( fsize<60){
char buf[50];
int len;
HPDF_Page_SetFontAndSize(page, font, fsize);
HPDF_Page_MoveTextPos (page, 0, -5 - fsize);
strcpy(buf, samp_text);
len = HPDF_Page_MeasureText (page, samp_text,
                        HPDF_Page_GetWidth(page) - 120, HPDF_FALSE, NULL);
buf[len] = 0x00;
HPDF_Page_ShowText (page, buf);
HPDF_Page_MoveTextPos (page, 0, -10);
HPDF_Page_SetFontAndSize(page, font, 8);
snprintf(buf, 50, "Fontsize=%.0f", fsize);
HPDF_Page_ShowText (page, buf);
fsize *= 1.5;
}
HPDF_Free (pdf);
return 0;
}
