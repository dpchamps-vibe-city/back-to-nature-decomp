#include "common.h"

extern s16 gCalendarSeason;

s32 ovl_11_func_800F581C(void) {
    switch (gCalendarSeason) {
    case 0:
    default:
        return 2;
    case 1:
        return 4;
    case 2:
        return 8;
    case 3:
        return 0x10;
    }
}
