#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_800F06D8(s32 *arg0, s32 *arg1);
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5C60(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E65AC(s16 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 var_s2;
    s32 *var_a1;
    s32 var_a2;
    s32 var_s0;
    u8 temp_a0;

    var_s2 = arg2;
    if ((arg1 << 16) != 0) {
        var_s0 = gOvl11ValueSlots[arg0];
    } else {
        var_s0 = arg0;
    }
    if (arg3 == 1) {
        var_a1 = (s32 *)((char *)D_8007A670 + var_s0 * 0xF8);
    } else {
        var_a1 = (s32 *)((char *)D_80076280 + var_s0 * 0x1D4);
    }
    var_a2 = ovl_11_func_800F06D8(gObjectWorkPosition, var_a1);
    switch (arg3) {
    case 1:
        ((struct struct_8006C838_800E65AC_b *)((u8 *)gObjectWorkPosition - 0x52C8 + var_s0 * 0xF8))->field_DE22 = (s16)var_a2;
        goto done;
    case 2:
        temp_a0 = ((struct struct_8006C838_800E65AC_a *)((u8 *)gObjectWorkPosition - 0x52C8 + var_s0 * 0x1D4))->field_9A6C;
        if ((u32)(temp_a0 - 5) < 5U) {
            var_s2 = 5;
        } else if (temp_a0 == 0xA) {
            var_s2 = 0xB;
            var_a2 = 0;
        }
        break;
    }
    ovl_11_func_800E5B84((s16)(var_s0 << 16 >> 16), var_s2, var_a2, 1);
    ovl_11_func_800E5C60((s16)(var_s0 << 16 >> 16), 0, 0, 0);
done:
    return 1;
}
