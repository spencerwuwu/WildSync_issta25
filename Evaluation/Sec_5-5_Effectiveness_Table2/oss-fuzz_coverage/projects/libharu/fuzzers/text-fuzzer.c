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

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {

    char *samp_text  = malloc(size + 1);
    memcpy(samp_text, data, size);
    samp_text[size] = '\0';

    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;
    float tw;
    float fsize;

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }

    /* set compression mode */
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* create default-font */
    font = HPDF_GetFont (pdf, "Helvetica", NULL);

    /* add a new page object. */
    page = HPDF_AddPage (pdf);

    const char *page_title = "Text Demo";

    /* print the title of the page (with positioning center). */
    HPDF_Page_SetFontAndSize (page, font, 24);
    tw = HPDF_Page_TextWidth (page, page_title);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                HPDF_Page_GetHeight (page) - 50, page_title);
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, 60, HPDF_Page_GetHeight(page) - 60);

    /*
     * font size
     */
    fsize = 8;
    while (fsize < 60) {
        char buf[50];
        int len;

        /* set style and size of font. */
        HPDF_Page_SetFontAndSize(page, font, fsize);

        /* set the position of the text. */
        HPDF_Page_MoveTextPos (page, 0, -5 - fsize);

        /* measure the number of characters which included in the page. */
        strcpy(buf, samp_text);
        len = HPDF_Page_MeasureText (page, samp_text,
                        HPDF_Page_GetWidth(page) - 120, HPDF_FALSE, NULL);

        /* truncate the text. */
        buf[len] = 0x00;

        HPDF_Page_ShowText (page, buf);

        /* print the description. */
        HPDF_Page_MoveTextPos (page, 0, -10);
        HPDF_Page_SetFontAndSize(page, font, 8);
        snprintf(buf, 50, "Fontsize=%.0f", fsize);
        HPDF_Page_ShowText (page, buf);

        fsize *= 1.5;
    }


    /* clean up */
    HPDF_Free (pdf);

    return 0;
}
