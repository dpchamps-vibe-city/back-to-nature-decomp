#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C1224(s16 arg0, s16 arg1);

s32 ovl_11_func_800BE2C4(s32 **arg0, u16 *arg1) {
    ItemData *temp_a2;
    ItemData *item_base;
    s16 temp_a0;
    s16 temp_s2;
    s16 temp_v0;
    s16 temp_v1;
    s16 lower;
    s16 upper;
    s16 z_lower;
    s32 *temp_a0_5;
    Vec3 *temp_a1;
    s32 *temp_a1_2;
    s32 *temp_v0_2;
    s32 temp_a0_2;
    u16 temp_v1_2;

    temp_s2 = (s16) *arg1;
    if ((*(s16 *) ((u8 *) arg1 + 0x30)) == (*(Ovl11ItemCollisionObject **) ((u8 *) arg0 + 4))->unk2) {
        if ((*(u16 **) ((u8 *) arg0 + 0xC)) != arg1) {
            if (!((*(s32 *) ((u8 *) arg1 + 0x34)) & 0x02020400) || ((*arg1 == 0x106) && (ovl_11_func_800C1224(((struct struct_8006C838_button *) D_8006C838)->field_44BA, (s16) ((struct Ovl11MenuDateView *) D_8006C838)->field_44BC) != 8))) {
                temp_v1 = *(s16 *) ((u8 *) arg1 + 0x38);
                item_base = D_8006C858;
                temp_a2 = (ItemData *) (temp_s2 * sizeof(ItemData) + (u32) item_base);
                temp_a0 = *(s16 *) ((u8 *) temp_a2 + 0x24);
                temp_a1 = (Vec3 *) *arg0;
                temp_a0_2 = temp_a1->x;
                lower = temp_v1 - temp_a0;
                upper = temp_v1 + temp_a0;
                if ((lower < temp_a0_2) && (temp_a0_2 < upper)) {
                    temp_v0 = *(s16 *) ((u8 *) arg1 + 0x40);
                    temp_a0 = *(s16 *) ((u8 *) temp_a2 + 0x26);
                    z_lower = temp_v0 - temp_a0;
                    upper = temp_v0 + temp_a0;
                    if (z_lower < temp_a1->z) {
                        if (temp_a1->z < upper) {
                            *(*(s32 **) ((u8 *) arg0 + 0x10)) = (*(s32 *) ((u8 *) arg0 + 0x18)) | 0x2000;
                            if ((u32) (*arg1 - 0x160) < 7U) {
                                temp_a0_5 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_a0_5 &= ~0x2000;
                                temp_a1_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_a1_2 |= 0x14000;
                            }
                            temp_v1_2 = *arg1;
                            if ((temp_v1_2 == 0x108) || (temp_v1_2 == 0x17A) || (temp_v1_2 == 0x17B) || (temp_v1_2 == 0x15B)) {
                                temp_v0_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v0_2 &= ~0x2000;
                            }
                            *(*(u16 ***) ((u8 *) arg0 + 8)) = arg1;
                            return 1;
                        }
                        return 0;
                    }
                }
                goto block_17;
            }
            return 0;
        }
block_17:
        return 0;
    }
    return 0;
}
