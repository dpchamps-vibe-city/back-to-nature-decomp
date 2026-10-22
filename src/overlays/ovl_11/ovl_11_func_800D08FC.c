#include "common.h"
#include "game_types.h"

Ovl11DF72CPair *ovl_11_func_800D08FC(s32 arg0) {
    Ovl11DF72CPair *ret;
    s16 *ptr;
    s16 *firstptr;
    u8 *base;
    u8 *flagbase;
    s32 i;
    s32 offset;
    s32 last;

    ret = 0;
    if (arg0 == 1) {
        i = 0;
        base = (u8 *)D_80076200;
        firstptr = (s16 *)base;
        last = 1;
        flagbase = (u8 *)D_8006C838;
        for (; i < 2 && (i != last || *(u16 *)(flagbase + 0x44D0) != 0); i++) {
            offset = i * 4;
            if (*firstptr == -1) {
                ret = (Ovl11DF72CPair *)(offset + (u32)base);
                break;
            }
            firstptr += 2;
        }
    } else {
        i = 2;
        base = (u8 *)D_8006C838;
        ptr = (s16 *)(base + 0x99D0);
        for (; i < 4 && (i != 3 || *(u16 *)(base + 0x44D2) != 0); i++) {
            offset = i * 4;
            if (*ptr == -1) {
                ret = (Ovl11DF72CPair *)((u8 *)D_80076200 + offset);
                break;
            }
            ptr += 2;
        }
    }
    return ret;
}
