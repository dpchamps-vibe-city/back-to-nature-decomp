#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800E7504(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5C60(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

struct struct_8006C838_800E5D00 {
    char pad_0000[0x52C6];
    s16 field_52C6;
    char pad_52C8[0x99EE - 0x52C8];
    s16 field_99EE;
    char pad_99F0[0x9A16 - 0x99F0];
    s16 field_9A16;
};

s32 ovl_11_func_800E5D00(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 var_a0;
    s16 var_t1;
    s16 fld;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_v0;
    struct struct_8006C838_800E5D00 *temp_v0;

    if (arg0 == 0x2A) {
        var_a0 = 1;
        var_s0 = 0x20000;
        var_s1 = 0x10000;
        do {
            ovl_11_func_800E5D00(var_a0, arg1, arg2, arg3);
            var_s2 = var_s0;
            var_s0 += var_s1;
            var_a0 = (s16) (var_s2 >> 0x10);
        } while (var_a0 < 0x25);
        return 1;
    }
    if (arg1 == -1) {
        var_t1 = ((struct struct_8006C838_800E5D00 *)D_8006C838)->field_52C6;
    } else {
        var_t1 = arg1;
    }
    if (arg2 != -1) {
        gOvl11ValueSlots[arg2] = (s32) ((struct struct_8006C838_800E5D00 *)((u8 *)D_8006C838 + arg0 * 0x1D4))->field_9A16;
    }
    {
        struct struct_8006C838_800E5D00 *base = (struct struct_8006C838_800E5D00 *)D_8006C838;
        temp_v0 = (struct struct_8006C838_800E5D00 *)((u8 *)base + arg0 * 0x1D4);
    }
    fld = temp_v0->field_9A16;
    var_v0 = 0;
    if (fld == var_t1) {
        if (arg3 > 0) {
            ovl_11_func_800E7504(arg0, -1, -1, -1);
            var_v0 = 1;
            if (arg3 >= 2) {
                ovl_11_func_800E5C60(arg0, 0, 0, 0);
                var_v0 = 1;
                if (arg3 >= 3) {
                    ovl_11_func_800E5B84(arg0, 0, (s32) temp_v0->field_99EE, 1);
                    var_v0 = 1;
                }
            }
            return var_v0;
        }
        return 1;
    }
    return var_v0;
}
