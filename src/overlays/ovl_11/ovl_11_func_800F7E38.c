#include "common.h"
#include "game_types.h"
#include "ovl11_selection_texture_transfer.h"

s32 ovl_11_func_800F8404(s32 arg0);
s32 ovl_11_func_800D5810(s16 arg0);
s32 ovl_11_func_800D57A4(s16 arg0);
s8 ovl_11_func_800D583C(s16 arg0);
void ovl_11_func_800F8480(Ovl11CopyRecord *arg0, Ovl11CopyRecord *arg1);
void ovl_11_func_800F84D4(Ovl11CopyRecord *arg0, Ovl11CopyRecord *arg1);
void ovl_11_func_800D5740(s16 *arg0);

s32 ovl_11_func_800F7E38(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_v0;
    s8 temp_v0_2;

    temp_v0 = ovl_11_func_800F8404(arg3);
    if (temp_v0 != 1) {
        if (temp_v0 < 2) {
            if (temp_v0 != 0) {
                return 0;
            }
            goto block_9;
        }
        var_v0 = 0;
        if (temp_v0 == 2) {
            var_v0 = 0;
            if (ovl_11_func_800D5810((s16) arg0[0]) != 0) {
                var_v0 = 0;
                if (gOvl11InteractionPhase != ovl_11_func_800D57A4((s16) arg0[0])) {
                    return 0;
                }
                Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 0);
                ovl_11_func_800F8480((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
                return 1;
            }
        }
        /* Duplicate return node #17. Try simplifying control flow for better match */
        return var_v0;
    }
    var_v0 = 0;
    if (ovl_11_func_800D5810((s16) arg0[0]) != 0) {
block_9:
        temp_v0_2 = ovl_11_func_800D583C((s16) arg0[0]);
        if (temp_v0_2 != 0) {
            if (temp_v0_2 >= 0) {
                var_v0 = 0;
                if (temp_v0_2 < 3) {
                    Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 0);
                    ovl_11_func_800F8480((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
                    return 1;
                }
                /* Duplicate return node #17. Try simplifying control flow for better match */
                return var_v0;
            }
            var_v0 = 0;
            /* Duplicate return node #17. Try simplifying control flow for better match */
            return var_v0;
        }
        Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 0);
        ovl_11_func_800F84D4((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
        (*(s16 *) ((u8 *) arg2 + 4)) = 1;
        var_v0 = 1;
        if (--((s16 *) arg0)[2] <= 0) {
            ovl_11_func_800D5740((s16 *) arg0);
            return 1;
        }
        /* Duplicate return node #17. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}
