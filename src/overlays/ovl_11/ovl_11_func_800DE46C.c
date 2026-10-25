#include "common.h"
#include "ovl11_clock_calendar.h"
#include "game_flags.h"

void ovl_11_func_800DE46C(void) {
    char *base;

    if (GetGameFlag(0x49) != 0) {
        base = (char *)&D_8006C838;
        *(u16 *)(base + 0x51F4) = *(u16 *)(base + 0x44CA);
        *(s32 *)(base + 0x4450) |= 4;
        Ovl11RequestClockAdvanceToSix(0x168);
        SetGameFlag(0x49, 0);
    }
}
