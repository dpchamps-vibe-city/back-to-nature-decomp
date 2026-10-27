#include "common.h"
#include "game_types.h"

s32 ovl_11_func_801096FC(Ovl11PaddedVec3 position, s16 key, s32 radius, s32 *flag, s32 **out) {
    Ovl11RecD749F4 *base;
    s32 result;
    s32 i;
    s16 sel;
    s32 r;
    s32 lo0;
    s32 hi0;
    s32 lo1;
    s32 hi1;
    s32 one;
    s32 notmatch;
    s32 mask;

    result = 0;
    i = 0;
    notmatch = 0x12;
    mask = 0x10000;
    base = (Ovl11RecD749F4 *)&gTaggedRecordBase;
    sel = key;
    r = radius;
    lo0 = position.vx - r;
    hi0 = position.vx + r;
    lo1 = position.vz - r;
    hi1 = position.vz + r;
    one = 1;
    do {
        if (base[i].unk0 != 0 && sel == base[i].unk30 && base[i].unk26 != notmatch && lo0 < base[i].unk38 && base[i].unk38 < hi0 && lo1 < base[i].unk40 && base[i].unk40 < hi1) {
            if (base[i].unk34 & mask) {
                *flag = 0;
                *out = &base[i];
                result = 1;
            } else {
                *flag = one;
                *out = &base[i];
                result = 1;
            }
            break;
        }
        i++;
    } while (i < 20);
    return result;
}
