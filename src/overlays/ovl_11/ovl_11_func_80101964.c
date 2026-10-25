#include "common.h"

void ovl_11_func_800F6D14 (s32 *arg0, u16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800F6F7C (s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800F7010 (s32 arg0, s16 arg1, s16 arg2);

void ovl_11_func_80101964(void) {
    s32 sp10;
    u16 var_s2;
    u16 var_s4;
    s32 *temp_s6;
    s32 temp_s5;
    s32 temp_v0;
    u8 var_s3;
    s32 var_s7;

    var_s2 = 0x84;
    var_s4 = 0x5E;
    var_s3 = 1;
    temp_v0 = gActiveRenderContext->field_D8;
    temp_s6 = (s32 *)(temp_v0 + 0x68);
    temp_s5 = temp_v0 + 0x6C;
    sp10 = temp_v0 + 0x60;
    for (var_s7 = 0; var_s7 < 7; var_s7++) {
        if (((Ovl11IconFlagsView *)D_8006C838)->flags & var_s3) {
            ovl_11_func_800F6D14(temp_s6, D_801273F4[var_s7], var_s2, var_s4);
            if (D_801273EE_flags & var_s3) {
                ovl_11_func_800F6F7C(sp10, var_s2, var_s4);
            }
        } else {
            ovl_11_func_800F7010(temp_s5, var_s2, var_s4);
        }
        var_s2 += 0x22;
        if ((u32) var_s2 >= 0xEBU) {
            var_s2 = 0x84;
            var_s4 = (var_s4 + 0x22) & 0xFFFF;
        }
        var_s3 <<= 1;
    }
    if (((Ovl11IconFlagsView *)D_8006C838)->flags & 0x80) {
        ovl_11_func_800F6D14(temp_s6, 0x79U, var_s2, var_s4);
        if (D_801273EE_flags & 0xF80) {
            ovl_11_func_800F6F7C(sp10, var_s2, var_s4);
        }
    } else {
        ovl_11_func_800F7010(temp_s5, var_s2, var_s4);
    }
}
