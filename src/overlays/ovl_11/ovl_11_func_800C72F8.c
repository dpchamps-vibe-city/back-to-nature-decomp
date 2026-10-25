#include "common.h"
#include "game_types.h"
#include "ovl11_grouped_value_mapping.h"

/* 800C73C4-C8 passes full-width arithmetic; 800C70CC stores its low half. */
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800C72F8(Ovl11C7010Object *arg0, u16 *arg1) {
    s32 temp_s2;
    s32 var_a2;
    s16 item;

    if ((u32) (arg1[0] - 1) < 0x19U) {
        *(s16 *) ((u8 *) arg0 + 0x18) = 1;
        item = (s16) arg1[0];
        D_80128C6C = 0;
        temp_s2 = Ovl11GetGroupedValueIndex(item);
        var_a2 = Ovl11MapGroupedValueCode((s16) arg1[0]) + (arg0->unk38 * 5);
        if ((u32) (arg1[0] - 0x15) < 5U) {
            if (D_80128C50 & 3) {
                arg0->unk6C |= 0x1800;
                ovl_11_func_800C70CC(arg0, 0x52,
                    (arg0->unk38 * 5) + *(s16 *) (arg1 + 1), -2);
                return 1;
            }
            if (*(s16 *) (arg1 + 2) == 0) {
                return 0;
            }
        }
        ovl_11_func_800C70CC(arg0, (temp_s2 * 2) + 0x16, var_a2, -2);
        return 1;
    }
    return 0;
}
