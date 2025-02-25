#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fribidi.h>
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
FriBidiStrIndex str_len = size / 4;
FriBidiChar *str = (FriBidiChar *) data;
FriBidiCharType *types = calloc(str_len, sizeof (FriBidiCharType));
FriBidiBracketType *btypes = calloc(str_len, sizeof (FriBidiBracketType));
FriBidiLevel *levels = calloc(str_len, sizeof (FriBidiLevel));
/**** Start of extracted external code ****/
// Extracted from: ffmpeg: libavfilter/vf_drawtext.c_0
 FriBidiLevel * embedding_levels = NULL;
 FriBidiArabicProp * ar_props = NULL;
 embedding_levels =calloc ( str_len, sizeof ( * embedding_levels ) );
 ar_props =calloc ( str_len, sizeof ( * ar_props ) );
fribidi_join_arabic(types, str_len,  embedding_levels,  ar_props);
{
}

/**** End of extracted external code ****/
fribidi_get_bidi_types(str, str_len, types);
fribidi_get_bracket_types(str, str_len, types, btypes);
FriBidiParType par_type = FRIBIDI_PAR_ON;
FriBidiLevel level = fribidi_get_par_embedding_levels_ex(types, btypes, str_len, &par_type, levels);
if((0))assert (level);
free (types);
free (btypes);
free (levels);
return 0;
}
