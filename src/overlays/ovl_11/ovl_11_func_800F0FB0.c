#include "common.h"
#include "ovl11_clock_calendar.h"

extern s32 D_8012961C;

s32 func_8001AF44(u32 arg0);

void ovl_11_func_800F0FB0(s32 arg0) {
    if ((D_8012961C == 1) || (func_8001AF44(2U) == 0)) {
        D_8012961C = arg0;
    }
    if ((D_80070CF8_word == 6) && (Ovl11HasQueuedClockEvent() != 0) && (D_8012961C != 8)) {
        D_8012961C = 0;
    }
}
