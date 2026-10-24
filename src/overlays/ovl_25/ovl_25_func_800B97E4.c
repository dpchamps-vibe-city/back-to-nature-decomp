#include "common.h"

s32 ovl_25_func_800B9A10(s32 arg0);
void ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);
s32 ovl_25_func_800B9A84(s32 arg0);

void ovl_25_func_800B97E4(s16 *arg0) {
    s16 temp_v1;
    s16 temp_v1_2;
    s32 var_a1;
    s16 var_a2;

    var_a2 = (s16) (ovl_25_func_800B9A10(*(s32 *) ((u8 *) arg0 + 0xC)) / D_800BFE44_view.unk4D4);
    temp_v1 = arg0[0];
    if (temp_v1 == 5) {
        temp_v1_2 = D_800BFE44_view.unkC / 30;
        var_a2 = D_800BFE44_view.unk4DE / 30;
        for (var_a1 = 0; var_a1 < temp_v1_2; var_a1++) {
            var_a2 = (var_a2 * (0x64 - D_800BFE44_view.unk4E0)) / 100;
        }
        if (var_a2 <= 0) {
            var_a2 = 1;
        }
        (*(s16 *) ((u8 *) arg0 + 4)) = 1;
    } else if (temp_v1 == 4) {
        var_a2 = D_800BFE44_view.unk4E2 / 30;
        (*(s16 *) ((u8 *) arg0 + 4)) = 1;
    } else if (temp_v1 != 0) {
        var_a2 = (var_a2 * D_800BFE44_view.unk4E4) / 100;
    }
    arg0[8] += var_a2;
    if ((*(s16 *) ((u8 *) arg0 + 4)) <= 0) {
        ovl_25_func_800B9B00(arg0, 2, 0);
    }
    if ((u32) ((u16) arg0[0] - 4) >= 2U) {
        (*(s32 *) ((u8 *) arg0 + 0xC)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0xC)) + ((s32) (ovl_25_func_800B9A84(*(s32 *) ((u8 *) arg0 + 0xC)) << 0xC) / D_800BFE44_view.unk4D4));
    }
}
