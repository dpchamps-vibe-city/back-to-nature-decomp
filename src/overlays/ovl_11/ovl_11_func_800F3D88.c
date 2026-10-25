#include "common.h"
#include "ovl11_clock_calendar.h"
#include "game_flags.h"

s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);

s32 ovl_11_func_800F3D88(void) {
    u8 *base;
    u16 *p;

    if (GetGameFlag(0xA0) == 1) {
        return 1;
    }
    SetGameFlag(0xA0, 1);
    base = (u8 *)&D_8006C838;
    if (Ovl11FindCalendarDate(*(s16 *)(base + 0x44BA), *(s16 *)(base + 0x44BC)) != 0) {
        return 1;
    }
    p = (u16 *)(base + 0x498C);
    ovl_11_func_800F2354(*(s32 *)(p + 0x1A), 0);
    *(s32 *)(p + 0x1A) = 0;
    return 1;
}
