#include "common.h"
#include "game_types.h"

s16 ovl_11_func_800F4240(Ovl11ItemCollisionObject *arg0);
s32 ovl_11_func_800D7504(s16 arg0);

s32 ovl_11_func_800BE94C(s32 **arg0, Ovl11ItemCollisionObject *arg1) {
    s16 temp_s0;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 lower;
    s16 upper;
    void *temp_a1;
    s32 *temp_a1_2;
    s32 *temp_a2;
    s32 *temp_v0;
    s32 *temp_v0_2;
    s32 *temp_v1_3;
    s32 temp_a0;
    s32 temp_a0_2;
    void *item_base;

    if (arg1->unk2 != 0) {
        if (arg1->unk4 & D_80122E6C[((struct Ovl11ItemCollisionGlobals *) D_8006C838)->selector]) {
            temp_a2 = *arg0;
            item_base = ((struct Ovl11ItemCollisionGlobals *) D_8006C838)->items;
            temp_a1 = (void *) ((u16) arg1->unk2 * 0x28 + (s32) item_base);
            temp_s0 = *(s16 *) ((u8 *) temp_a1 + 0x24);
            temp_v1 = *(s16 *) ((u8 *) arg1 + 8);
            temp_a0 = *temp_a2;
            lower = temp_v1 - temp_s0;
            upper = temp_v1 + temp_s0;
            if (lower < temp_a0) {
                if (temp_a0 < upper) {
                    temp_s0 = *(s16 *) ((u8 *) temp_a1 + 0x26);
                    temp_v1_2 = *(s16 *) ((u8 *) arg1 + 0x10);
                    temp_a0_2 = *(s32 *) ((u8 *) temp_a2 + 8);
                    lower = temp_v1_2 - temp_s0;
                    upper = temp_v1_2 + temp_s0;
                    if (lower < temp_a0_2) {
                        if ((temp_a0_2 < upper) && (ovl_11_func_800F4240(arg1) != 0)) {
                            temp_a1_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                            item_base = ((struct Ovl11ItemCollisionGlobals *) D_8006C838)->items;
                            temp_s0 = *(s16 *) ((u16) arg1->unk2 * 0x28 + (u8 *) item_base);
                            *temp_a1_2 |= 0x4000;
                            *(*(Ovl11ItemCollisionObject ***) ((u8 *) arg0 + 8)) = arg1;
                            if ((u32) ((u16) arg1->unk2 - 0x64) < 2U) {
                                temp_v0 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v0 &= ~0x4000;
                            }
                            if (ovl_11_func_800D7504(arg1->unk2) != 0) {
                                temp_v0_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v0_2 &= ~0x4000;
                            }
                            if (temp_s0 & 0x8000) {
                                temp_v1_3 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v1_3 |= 0x2000;
                            }
                            return 1;
                        }
                        return 0;
                    }
                    goto block_14;
                }
                return 0;
            }
            goto block_14;
        }
        return 0;
    }
block_14:
    return 0;
}
