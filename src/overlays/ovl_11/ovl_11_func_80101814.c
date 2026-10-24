#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_11_func_800F6D14 (s32 *arg0, u16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800F7010 (s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800F8CF8 (s32 *arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800F6A24 (u16 arg0);
void ovl_11_func_800F6C78 (s32 *arg0, u16 arg1, s16 arg2, s16 arg3);
void func_80015840(ObjectState *obj, u8 arg1);
void func_8001585C(ObjectState *obj, u8 arg1);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_11_func_80101814(void) {
    u16 var_s0;
    u16 var_s1;
    s32 *temp_s3;
    s32 temp_s2;
    s32 temp_v0;
    s32 var_s4;
    u8 var_s6;

    var_s0 = 0x84;
    var_s1 = 0x14;
    var_s6 = 1;
    temp_v0 = D_8005E3C0->field_D8;
    for (var_s4 = 0; var_s4 < 7; var_s4++) {
        temp_s3 = (s32 *)(temp_v0 + 0x68);
        temp_s2 = temp_v0 + 0x6C;
        if (D_801273EE_flags & var_s6) {
            ovl_11_func_800F6D14(temp_s3, D_801273F4[var_s4], var_s0, var_s1);
        } else {
            ovl_11_func_800F7010(temp_s2, var_s0, var_s1);
        }
        var_s0 += 0x22;
        if ((u32) var_s0 >= 0xEBU) {
            var_s0 = 0x84;
            var_s1 = (var_s1 + 0x22) & 0xFFFF;
        }
        var_s6 <<= 1;
    }
    if (D_801273EE_flags & 0xF80) {
        ovl_11_func_800F6D14(temp_s3, 0x79U, var_s0, var_s1);
        return;
    }
    ovl_11_func_800F7010(temp_s2, var_s0, var_s1);
}
