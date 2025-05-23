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
if (_IN_BUFFER_SIZE < sizeof(char*) + 1) {
	return 0;
}
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 2;

char* __fuzz_0 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_0_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_0_LEN;

char* __encoder__name;
__encoder__name = malloc(__fuzz_0_LEN + 1);
memcpy(__encoder__name, __fuzz_0, __fuzz_0_LEN);
__encoder__name[__fuzz_0_LEN] = 0;
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
/**** Start of extracted external code ****/
// Extracted from: vtk: ThirdParty/libharu/vtklibharu/src/hpdf_doc.c_0
 if ( (HPDF_Doc_FindEncoder(pdf,  __encoder__name) != NULL ) ){
}

/**** End of extracted external code ****/
font = HPDF_GetFont (pdf, "Helvetica", NULL);
page = HPDF_AddPage (pdf);
char *page_title = "Text Demo";
HPDF_Page_SetFontAndSize (page, font, 24);
tw = HPDF_Page_TextWidth (page, page_title);
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
