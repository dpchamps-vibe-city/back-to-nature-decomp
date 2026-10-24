#include "common.h"
#include "game_types.h"

void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_17_func_800B91B8(void) {
    s16 y;
    s16 i;
    s32 screenX;
    s16 mirror;
    u8 *base;
    s16 *yFields;

    base = D_800BD848;
    i = 0;
    do {
        yFields = (s16 *)(base + 0x2DC);
        y = yFields[i * 4];
        if (y != -1) {
            mirror = (i - (s16)(i / 12) * 12) >= 6;
            screenX = 0xA0 -
                (((Ovl17QueueView *)base)->entries[i].word -
                 *(s32 *)(base + 0x24)) / 0x1000;
            func_80015EE8(D_8005E3C0->field_D8 + 0x54,
                         (s32)(base + 0x22C), 3, mirror, (s16)screenX, y);
        }
        i++;
    } while (i < 90);
}
