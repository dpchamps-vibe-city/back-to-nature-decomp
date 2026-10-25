#include "common.h"
#include "psyq/stddef.h"
#include "game_flags.h"

s32 ovl_11_func_800E65AC(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E698C(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
/* The caller TU declares the full 4-argument interface (a3 is cleared before
 * the jal), matching the ovl_11_func_800E5ADC call shape. */
s32 ovl_11_func_800E63C8(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E648C(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 temp_s0;
    s32 var_s1;

    var_s1 = 0;
    temp_s0 = ((struct struct_8006C838_800E7660 *)D_8006C838)->field_51EA;
    if (temp_s0 != -1) {
        if (arg0 != -1) {
            gOvl11ValueSlots[arg0] = temp_s0;
        }
        var_s1 = 1;
        if (arg3 != 0) {
            var_s1 = GetGameFlag(0x46U);
        }
        if (var_s1 == 1) {
            if (arg1 != 0) {
                ovl_11_func_800E65AC(temp_s0, 0, arg1 - 1, 0);
            }
            if (arg2 != 0) {
                ovl_11_func_800E698C(0x2C, temp_s0, 1, 0);
            }
        }
    }
    if ((arg3 == 0) || (var_s1 == 1)) {
        ((struct struct_8006C838_800E7660 *)D_8006C838)->field_51EA = -1;
        SetGameFlag(0x46U, 0U);
    }
    ovl_11_func_800E63C8(var_s1 == 0, 1, 0, 0);
    return var_s1;
}
