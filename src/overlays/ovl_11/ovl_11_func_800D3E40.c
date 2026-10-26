#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D3E40(Ovl11PaddedVec3 position, s16 key, s32 radius, s32 *flag, void **out) {
    s32 result;
    u8 *p;
    s32 lo1;
    s32 lo0;
    s16 sel;
    s32 i;
    u8 *base;
    s32 r;
    s32 hi0;
    s32 hi1;

    result = -1;
    i = 0;
    base = (u8 *)&gTaggedRecordBase;
    sel = key;
    r = radius;
    lo0 = position.field_0 - r;
    hi0 = position.field_0 + r;
    lo1 = position.field_8 - r;
    hi1 = position.field_8 + r;
    do {
        do {
            p = base + i * 0xB8;
            if (*(u16 *)p == 0 || sel != *(s16 *)(p + 0x30)) {
                goto next_large;
            }
        } while (0);
        if ((*(s32 *)(p + 0x34) & 0x20000) != 0) {
            goto next_large;
        }
        do {
            if (!(lo0 < *(s32 *)(p + 0x38) && *(s32 *)(p + 0x38) < hi0)) {
                goto next_large;
            }
            if (!(lo1 < *(s32 *)(p + 0x40) && *(s32 *)(p + 0x40) < hi1)) {
                goto next_large;
            }
            do {
                do {
                    result = i;
                } while (0);
            } while (0);
        } while (0);
        if (*(s32 *)(p + 0x34) & 0x10000) {
            *flag = 0;
            *out = p;
        } else {
            *flag = 1;
            *out = p;
        }
        break;
next_large:
        i++;
    } while (i < 0x14);
    if (result == -1) {
        p = (u8 *)&D_800742EC;
        i = 0;
        do {
            do {
                if (*(u16 *)p == 0 || sel != *(s16 *)(p + 0x30)) {
                    goto next_small;
                }
            } while (0);
            if ((*(s32 *)(p + 0x34) & 0x20000) != 0) {
                goto next_small;
            }
            do {
                if (!(lo0 < *(s32 *)(p + 0x38) && *(s32 *)(p + 0x38) < hi0)) {
                    goto next_small;
                }
                if (!(lo1 < *(s32 *)(p + 0x40) && *(s32 *)(p + 0x40) < hi1)) {
                    goto next_small;
                }
                do {
                    do {
                        result = i;
                    } while (0);
                } while (0);
            } while (0);
            *flag = 2;
            *out = p;
            break;
next_small:
            i++;
            p += 0xB4;
        } while (i < 0xA);
    }
    return result;
}
