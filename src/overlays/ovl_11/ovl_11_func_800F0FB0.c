#include "common.h"
#include "ovl11_clock_calendar.h"
#include "game_flags.h"

extern s32 D_8012961C;

void ovl_11_func_800F0FB0(s32 arg0) {
    if ((D_8012961C == 1) || (GetGameFlag(2U) == 0)) {
        D_8012961C = arg0;
    }
    if ((D_80070CF8_word == 6) && (Ovl11HasQueuedClockEvent() != 0) && (D_8012961C != 8)) {
        D_8012961C = 0;
    }
}
