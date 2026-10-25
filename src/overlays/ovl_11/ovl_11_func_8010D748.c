#include "common.h"
#include "game_types.h"
#include "random.h"

s32 ovl_11_func_80110944(s16 area, u16 mode);
s32 ovl_11_func_800DF010(Ovl11ActionDispatchView *arg0, s32 arg1);
s32 ovl_11_func_800E0C24(Ovl11ActionDispatchView *arg0, s32 arg1);

s32 ovl_11_func_8010D748(u8 *arg0, s16 area) {
    s32 random;
    s32 failed;
    s32 i;
    s32 count;
    s16 index;
    s32 offset;
    u8 *base;
    Ovl30AnimalRecordB8 *animals;
    Ovl11ClearFlagRecord *flags;

    random = Rand(3);
    count = (u16)(*(u16 *)(arg0 + 0xB2) / 100U) + random;
    failed = 0;
    base = D_80074838;
    flags = (Ovl11ClearFlagRecord *)(base + 0x1F0);
    animals = (Ovl30AnimalRecordB8 *)(base + 0x1BC);
    for (i = 0; i < count; i++) {
        index = ovl_11_func_80110944(area, 2);
        if (index == -1) {
            failed = 1;
            break;
        }
        offset = index * 0xB8;
        if (!(*(s32 *)((u8 *)flags + offset) & 0x10000)) {
            ovl_11_func_800DF010((Ovl11ActionDispatchView *)(offset + (u32)animals), 6);
        } else {
            ovl_11_func_800E0C24((Ovl11ActionDispatchView *)(offset + (u32)animals), 6);
        }
    }
    return failed;
}
