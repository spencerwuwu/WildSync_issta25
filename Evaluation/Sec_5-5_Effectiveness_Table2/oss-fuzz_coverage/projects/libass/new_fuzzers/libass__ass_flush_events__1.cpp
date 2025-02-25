#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ass.h"
#include "ass_types.h"
ASS_Library *ass_library = NULL;
ASS_Renderer *ass_renderer = NULL;
uint8_t hval = 0;
void msg_callback(int level, char *fmt, va_list va, void *data)
{
 
 
 
 
 
 
     
     
    char msg[2048];
    int l = vsnprintf(msg, sizeof(msg), fmt, va) - 1;
    l = l >= sizeof(msg) ? sizeof(msg) - 1 : l;
    l = l < 0 ? 0 : l;
    hval ^= *(msg + l);
 
}
static int RWIDTH  = 854;
static int RHEIGHT = 480;
static bool init_renderer(void)
{
    if (ass_renderer)
        return true;

    ass_renderer = ass_renderer_init(ass_library);
    if (!ass_renderer)
        return false;

    ass_set_fonts(ass_renderer, NULL, "sans-serif",
                  ASS_FONTPROVIDER_AUTODETECT, NULL, 1);
    ass_set_frame_size(ass_renderer, RWIDTH, RHEIGHT);
    ass_set_storage_size(ass_renderer, RWIDTH, RHEIGHT);

    return true;
}
static bool init(void)
{
    ass_library = ass_library_init();
    if (!ass_library) {
        printf("ass_library_init failed!\n");
        return false;
    }

    ass_set_message_cb(ass_library, msg_callback, NULL);

    if (!init_renderer()) {
        ass_library_done(ass_library);
        ass_library = NULL;
        printf("ass_renderer_init failed!\n");
        return false;
    }

    return true;
}
static inline void process_image(ASS_Image* imgs)
{
    for (; imgs; imgs = imgs->next) {
        assert(imgs->w >= 0 && imgs->h >= 0 &&
               imgs->dst_x >= 0 && imgs->dst_y >= 0 &&
               imgs->dst_x + imgs->w <= RWIDTH &&
               imgs->dst_y + imgs->h <= RHEIGHT &&
               imgs->stride >= imgs->w);
 
         
        if (imgs->w && imgs->h)
            hval ^= *(imgs->bitmap + imgs->stride * (imgs->h - 1) + imgs->w - 1);
 
 
 
 
 
 
 
    }
}
 int  LLVMFuzzerTestOneInput( uint8_t  * _IN_BUFFER , size_t  _IN_BUFFER_SIZE){
/**** Start of fuzz variables initialization code ****/
size_t _BUF_POINTER = 0;
size_t _IN_REMAIN_LEN = (_IN_BUFFER_SIZE - _BUF_POINTER) / 1;

uint8_t* data = _IN_BUFFER + _BUF_POINTER;
size_t size = _IN_REMAIN_LEN;


/**** End of fuzz variables initialization code ****/
ASS_Track *track = NULL;
if(! init())return 0;
track = ass_read_memory(ass_library, (char *)data, size, NULL);
if( track){
for(int n=0;
 n< track->n_events;
++ n){
int change;
ASS_Event event = track->events[n];
/**** Start of extracted external code ****/
// Extracted from: mpv: sub/osd_libass.c_0
ass_flush_events(track);

/**** End of extracted external code ****/
process_image(ass_render_frame(ass_renderer, track, event.Start, &change));
if( event.Duration>1){
process_image(ass_render_frame(ass_renderer, track, event.Start + event.Duration/2, &change));
process_image(ass_render_frame(ass_renderer, track, event.Start + event.Duration-1, &change));
}
}
ass_free_track(track);
}
ass_renderer_done(ass_renderer);
ass_library_done(ass_library);
ass_renderer = NULL;
ass_library = NULL;
return 0;
}
