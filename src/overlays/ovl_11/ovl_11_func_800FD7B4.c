#include "common.h"

void func_80024A10(s32 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800FD968(s32 *arg0, s16 arg1);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FD7B4(s16 arg0, s16 arg1) {
    s16 temp_v0;
    s32 temp_a0;
    s32 var_a0;
    s32 test;
    s32 var_a3_2;
    void *var_a1;
    s32 drawing = gActiveRenderContext->field_D8 + 0x54;

    temp_v0 = ((struct Ovl11PairedValues *) D_8006C838)->field_44BA;
    temp_v0 += 4;
    temp_v0 -= arg0;
    func_80024A10(drawing, 0x18, arg1, temp_v0 % 4);
    ovl_11_func_800FD968(((struct Ovl11PairedValues *) D_8006C838)->pairs[arg0], arg1 + 2);
    temp_a0 = ((struct Ovl11PairedValues *) D_8006C838)->pairs[arg0][1];
    var_a3_2 = 0;
    if (temp_a0 < 0x1388) {
        var_a3_2 = 2;
        if (temp_a0 >= 0x7D0) {
            var_a3_2 = 1;
        }
    }
    test = ((struct Ovl11PairedValues *) D_8006C838)->pairs[arg0][0] < 0x1388;
    var_a0 = 0;
    if (test) {
        var_a0 = 2;
        if (((struct Ovl11PairedValues *) D_8006C838)->pairs[arg0][0] >= 0x7D0) {
            var_a0 = 1;
        }
    }
    test = ((struct Ovl11PairedValues *) D_8006C838)->pairs[arg0][0];
    if (test == 0 &&
        ((struct Ovl11PairedValues *) D_8006C838)->pairs[arg0][1] == 0) {
        var_a1 = D_80053770 + gTextBankOffsets[0];
    } else {
        var_a1 = (u8 *) D_8005175C + (gTextBankOffsets[0] + D_8012735C[var_a3_2][var_a0]);
    }
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) var_a1, 0xAC, (s32) arg1);
}
