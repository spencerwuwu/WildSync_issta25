#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <tiffio.h>
#include <tiffio.hxx>
#define __TIFFSafeMultiply(t, v, m)                                            \
    ((((t)(m) != (t)0) && (((t)(((v) * (m)) / (m))) == (t)(v)))                \
         ? (t)((v) * (m))                                                      \
         : (t)0)
uint64_t MAX_SIZE = 500000000;
extern "C" void handle_error(char *unused, char *unused2,
                             va_list unused3)
{
    return;
}
 extern "C"  int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
if (_IN_BUFFER_SIZE < sizeof(uint32_t) + 1) {
	return 0;
}
uint32_t __fuzz_0;
memcpy(&__fuzz_0, _IN_BUFFER + _BUF_POINTER, sizeof(uint32_t));
_BUF_POINTER += sizeof(uint32_t);

uint32_t defrowsperstrip;
defrowsperstrip = __fuzz_0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* Data = _IN_BUFFER + _BUF_POINTER;
size_t Size = _IN_REMAIN_LEN;

uint32_t  rowsperstrip;

/**** End of fuzz variables initialization code ****/
#if defined( __has_feature)
#if  __has_feature( memory_sanitizer)
setenv("JSIMD_FORCENONE", "1", 1);
#endif
#endif
std::istringstream s(std::string(Data, Data + Size));
TIFF *tif = TIFFStreamOpen("MemTIFF", &s);
if(! tif){
return 0;
}
uint32_t w, h;
uint32_t *raster;
TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
/**** Start of extracted external code ****/
// Extracted from: graphicsmagick: tiff/tools/fax2tiff.c_0
 rowsperstrip = ( defrowsperstrip ) ? defrowsperstrip:
TIFFDefaultStripSize(tif,  0);

/**** End of extracted external code ****/
if( TIFFTileSize64( tif)> MAX_SIZE){
TIFFClose(tif);
return 0;
}
uint64_t bufsize = TIFFTileSize64(tif);
if( bufsize> MAX_SIZE|| bufsize==0){
TIFFClose(tif);
return 0;
}
uint32_t tilewidth = 0;
uint32_t imagewidth = 0;
TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imagewidth);
tilewidth = __TIFFSafeMultiply(uint32_t, tilewidth, 2);
imagewidth = __TIFFSafeMultiply(uint32_t, imagewidth, 2);
if( tilewidth*2> MAX_SIZE|| imagewidth*2> MAX_SIZE|| tilewidth==0|| imagewidth==0){
TIFFClose(tif);
return 0;
}
uint32_t size = __TIFFSafeMultiply(uint32_t, w, h);
if( size> MAX_SIZE|| size==0){
TIFFClose(tif);
return 0;
}
raster = (uint32_t *)_TIFFmalloc(size * sizeof(uint32_t));
if( raster!=NULL){
TIFFReadRGBAImage(tif, w, h, raster, 0);
_TIFFfree(raster);
}
TIFFClose(tif);
return 0;
}
#ifdef STANDALONE

template <class T> static void CPL_IGNORE_RET_VAL(T) {}

static void Usage(int, char *argv[])
{
    fprintf(stderr, "%s [--help] [-repeat N] filename.\n", argv[0]);
    exit(1);
}

int main(int argc, char *argv[])
{
    int nRet = 0;
    void *buf = NULL;
    int nLen = 0;
    int nLoops = 1;
    char *pszFilename = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (i + 1 < argc && strcmp(argv[i], "-repeat") == 0)
        {
            nLoops = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-dummy") == 0)
        {
            uint8_t dummy = ' ';
            return LLVMFuzzerTestOneInput(&dummy, 1);
        }
        else if (strcmp(argv[i], "--help") == 0)
        {
            Usage(argc, argv);
        }
        else if (argv[i][0] == '-')
        {
            fprintf(stderr, "Unrecognized option: %s", argv[i]);
            Usage(argc, argv);
        }
        else
        {
            pszFilename = argv[i];
        }
    }
    if (pszFilename == nullptr)
    {
        fprintf(stderr, "No filename specified\n");
        Usage(argc, argv);
    }
    FILE *f = fopen(pszFilename, "rb");
    if (!f)
    {
        fprintf(stderr, "%s does not exist.\n", pszFilename);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    nLen = (int)ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(nLen);
    if (!buf)
    {
        fprintf(stderr, "malloc failed.\n");
        fclose(f);
        exit(1);
    }
    CPL_IGNORE_RET_VAL(fread(buf, nLen, 1, f));
    fclose(f);
    for (int i = 0; i < nLoops; i++)
    {
        nRet = LLVMFuzzerTestOneInput(static_cast<uint8_t *>(buf), nLen);
        if (nRet != 0)
            break;
    }
    free(buf);
    return nRet;
}

#endif