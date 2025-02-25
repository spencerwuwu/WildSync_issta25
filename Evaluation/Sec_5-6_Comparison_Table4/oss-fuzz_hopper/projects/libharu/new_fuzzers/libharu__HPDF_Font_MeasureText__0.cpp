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
if (_IN_BUFFER_SIZE < sizeof(HPDF_REAL) + sizeof(HPDF_REAL) + sizeof(HPDF_BOOL) + sizeof(char*) + sizeof(HPDF_REAL) + sizeof(HPDF_REAL) + sizeof(HPDF_REAL*) + 1) {
	return 0;
}
HPDF_REAL __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(HPDF_REAL));
_BUF_POINTER += sizeof(HPDF_REAL);

HPDF_REAL width;
width = __fuzz_0;
HPDF_REAL __fuzz_1;
memcpy(&__fuzz_1, _IN_BUFFER + _BUF_POINTER, sizeof(HPDF_REAL));
_BUF_POINTER += sizeof(HPDF_REAL);

HPDF_REAL __attr__gstate__word_space;
__attr__gstate__word_space = __fuzz_1;
HPDF_BOOL __fuzz_2;
memcpy(&__fuzz_2, _IN_BUFFER + _BUF_POINTER, sizeof(HPDF_BOOL));
_BUF_POINTER += sizeof(HPDF_BOOL);

HPDF_BOOL wordwrap;
wordwrap = __fuzz_2;
HPDF_REAL __fuzz_4;
memcpy(&__fuzz_4, _IN_BUFFER + _BUF_POINTER, sizeof(HPDF_REAL));
_BUF_POINTER += sizeof(HPDF_REAL);

HPDF_REAL __attr__gstate__char_space;
__attr__gstate__char_space = __fuzz_4;
HPDF_REAL __fuzz_5;
memcpy(&__fuzz_5, _IN_BUFFER + _BUF_POINTER, sizeof(HPDF_REAL));
_BUF_POINTER += sizeof(HPDF_REAL);

HPDF_REAL __attr__gstate__font_size;
__attr__gstate__font_size = __fuzz_5;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 3;

char* __fuzz_3 = (char*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_3_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_3_LEN;

char* text;
text = malloc(__fuzz_3_LEN + 1);
memcpy(text, __fuzz_3, __fuzz_3_LEN);
text[__fuzz_3_LEN] = 0;
HPDF_REAL* __fuzz_6 = (HPDF_REAL*) (_IN_BUFFER + _BUF_POINTER);
size_t __fuzz_6_LEN = _IN_REMAIN_LEN;
_BUF_POINTER += __fuzz_6_LEN;

HPDF_REAL* real_width;
real_width = __fuzz_6;
uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;

HPDF_UINT  ret;

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
/**** Start of extracted external code ****/
// Extracted from: vtk: ThirdParty/libharu/vtklibharu/src/hpdf_pages.c_0
 HPDF_UINT len =HPDF_StrLen ( text, HPDF_LIMIT_MAX_STRING_LEN + 1 );
 ret =HPDF_Font_MeasureText(font,  ( HPDF_BYTE * ) text,  len,  width,  __attr__gstate__font_size,  __attr__gstate__char_space,  __attr__gstate__word_space,  wordwrap,  real_width);

/**** End of extracted external code ****/
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
