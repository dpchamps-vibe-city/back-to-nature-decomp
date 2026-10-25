#include "common.h"
#include "screen_color_ramp.h"
#include "ovl11_clock_calendar.h"

void ovl_11_func_800BFE3C(void) {
    char *base;
    s32 s1;

    base = (char *)&D_8006C838;
    s1 = Ovl11ClassifyClockHour(*(s16 *)(base + 0x44C0));
    Ovl11SetScreenColorRampTarget(1, &D_80128A80[s1 * 3]);
    base += 0x8000;
    *(s32 *)(base + 0x6768) = s1;
}
