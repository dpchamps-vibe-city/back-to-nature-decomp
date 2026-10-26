#include "common.h"
#include "psyq/abs.h"

/* The callee consumes two arguments; this legacy call supplies four. */
s32 ovl_11_func_800E9284();
void ovl_11_func_800CD670(s16 arg0);

s32 ovl_11_func_800ED634(s16 arg0, s16 arg1) {
    s16 var_a0_2;
    s32 var_a0;
    s32 var_a0_3;
    s32 var_a1;
    s32 var_v0;

    switch (arg0) {
    case 2:
        var_a0 = abs(gObjectWorkPosition[0]);
        var_v0 = abs(gObjectWorkPosition[2]);
        if (var_v0 < var_a0) {
            var_a0_2 = 1;
            if (gObjectWorkPosition[0] > 0) {
                var_a0_2 = 3;
            }
        } else {
            var_a0_2 = (gObjectWorkPosition[2] > 0) * 2;
        }
        ovl_11_func_800E9284(var_a0_2, 0, 0, 0);
        /* fallthrough */
    case 1:
        if (arg1 != 0) {
            var_a0_3 = arg1 / 2;
        } else {
            var_a0_3 = 0xC;
        }
        ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_5234 |= 0x800000;
        ovl_11_func_800CD670((s16) var_a0_3);
        var_a1 = 1;
        break;
    case -1:
        ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_51F6 = 0;
        ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_5234 &= 0xFF7FFFF7;
        var_a1 = 1;
        break;
    default:
        var_a1 = 0;
        if (!(gPrimaryObjectFlags & 8)) {
            var_a1 = 1;
        }
        break;
    }
    return var_a1;
}
