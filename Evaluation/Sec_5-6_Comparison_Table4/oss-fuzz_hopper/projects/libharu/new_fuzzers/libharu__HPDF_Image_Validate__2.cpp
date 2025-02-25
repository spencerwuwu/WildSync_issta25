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
char filename[256];
sprintf(filename, "/tmp/libfuzzer.png");
FILE *fp = fopen(filename, "wb");
if(! fp)return 0;
fwrite(data, size, 1, fp);
fclose(fp);
HPDF_Doc  pdf;
HPDF_Font font;
HPDF_Page page;
HPDF_Destination dst;
HPDF_Image image;
double x;
double y;
double iw;
double ih;
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
HPDF_Page_SetWidth (page, 550);
HPDF_Page_SetHeight (page, 500);
dst = HPDF_Page_CreateDestination (page);
HPDF_Destination_SetXYZ (dst, 0, HPDF_Page_GetHeight (page), 1);
HPDF_SetOpenAction(pdf, dst);
HPDF_Page_BeginText (page);
HPDF_Page_SetFontAndSize (page, font, 20);
HPDF_Page_MoveTextPos (page, 220, HPDF_Page_GetHeight (page) - 70);
HPDF_Page_EndText (page);
image = HPDF_LoadPngImageFromFile (pdf, filename);
iw = HPDF_Image_GetWidth (image);
/**** Start of extracted external code ****/
// Extracted from: vtk: ThirdParty/libharu/vtklibharu/src/hpdf_image.c_2
 if ( ( !HPDF_Image_Validate(image) ) ){
}

/**** End of extracted external code ****/
ih = HPDF_Image_GetHeight (image);
HPDF_Page_SetLineWidth (page, 0.5);
x = 100;
y = HPDF_Page_GetHeight (page) - 150;
HPDF_Page_DrawImage (page, image, x, y, iw, ih);
HPDF_Page_GSave (page);
HPDF_Free (pdf);
return 0;
}
