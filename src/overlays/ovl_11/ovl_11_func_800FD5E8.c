#include "common.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void ovl_11_func_800FE614(u16 *arg0, s32 arg1);
void ovl_11_func_800FD7B4(s16 arg0, s16 arg1);

void ovl_11_func_800FD5E8(void) {
    s16 var_a1;
    s32 var_s0;
    s32 temp_s3;
    s32 var_a0;
    s32 value;

    temp_s3 = gActiveRenderContext->field_D8 + 0x54;
    ((struct Ovl11NumberTextHeader *) &D_8012A028)->marker = 0xFFD;
    func_80017B3C(temp_s3, (s32) (D_80053636 + D_80054BBC[0]), 0xB2, 0x14);
    func_80017B3C(temp_s3, (s32) (D_80053636 + 0x28 + D_80054BBC[0]), 0x20, 0x40);
    value = ((struct Ovl11PairedValues *) D_8006C838)->pairs[0][0];
    if (value >= -9999999) {
        var_a0 = value;
        if (var_a0 > 9999999) {
            var_a0 = 9999999;
        }
    } else {
        var_a0 = -9999999;
    }
    D_8012A028 = 0x68;
    ovl_11_func_800FE614((u16 *) func_8001A970(var_a0, &D_8012A028 + 2, 7), 0x1E86);
    func_80017B3C(temp_s3, (s32) &D_8012A028, 0xAC, 0x40);
    value = ((struct Ovl11PairedValues *) D_8006C838)->pairs[0][1];
    if (value >= -9999999) {
        var_a0 = value;
        if (var_a0 > 9999999) {
            var_a0 = 9999999;
        }
    } else {
        var_a0 = -9999999;
    }
    D_8012A028 = 0x69;
    ovl_11_func_800FE614((u16 *) func_8001A970(var_a0, &D_8012A028 + 2, 7), 0x1E86);
    func_80017B3C(temp_s3, (s32) &D_8012A028, 0xAC, 0x4E);
    var_a1 = 0x68;
    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        ovl_11_func_800FD7B4(var_s0, var_a1);
        var_a1 += 0x20;
    }
}
