#include "common.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"

void ovl_11_func_800BFADC(Ovl11CounterPair *arg0);
s32 ovl_11_func_800BFC20(s16 *arg0);
/* Executable Rand at 0x80012A34; overlay symbol spelling. */
u32 Rand(s32 arg0);

s16 ovl_11_func_800BF8B8(void) {
    Ovl11CounterPair next;
    Ovl11CounterPair after;
    s32 random;
    u8 *base;

    base = (u8 *)D_8006C838;
    next.unk0 = *(u16 *)(base + 0x44BA);
    next.unk2 = *(u16 *)(base + 0x44BC);
    ovl_11_func_800BFADC(&next);
    *(u16 *)(base + 0xE4CA) = 0;
    after.unk0 = next.unk0;
    after.unk2 = next.unk2;
    ovl_11_func_800BFADC(&after);
    if (Ovl11FindCalendarDate(next.unk0, next.unk2) == 0 &&
        (next.unk2 != *(s16 *)(base + 0x548C) ||
         next.unk0 != *(s16 *)(base + 0x548A)) &&
        ovl_11_func_800BFC20((s16 *)&next) != 1) {
        switch (next.unk0) {
        case 0:
            random = (s32)Rand(10) + 1;
            if (random % 3 == 0) {
                *(u16 *)(base + 0xE4CA) = 1;
            }
            break;
        case 1:
            random = Rand(20);
            if (random == 14) {
                *(u16 *)(base + 0xE4CA) = 3;
            } else if (random == 3) {
                *(u16 *)(base + 0xE4CA) = 1;
            }
            break;
        case 2:
            if (next.unk2 != 12 && Rand(5) == 3) {
                *(u16 *)(base + 0xE4CA) = 1;
            }
            break;
        case 3:
            random = Rand(10);
            if (random == 7) {
                *(u16 *)(base + 0xE4CA) = 4;
            } else if (random & 1) {
                *(u16 *)(base + 0xE4CA) = 2;
            }
            break;
        }
        if (Ovl11FindCalendarDate(after.unk0, after.unk2) != 0 &&
            (s16)((Ovl11BF8B8View *)D_8006C838)->field_E4CA >= 3) {
            ((Ovl11BF8B8View *)D_8006C838)->field_E4CA -= 2;
        }
    }
    return (s16)((Ovl11BF8B8View *)D_8006C838)->field_E4CA;
}
