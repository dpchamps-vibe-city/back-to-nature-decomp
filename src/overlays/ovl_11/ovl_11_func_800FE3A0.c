#include "common.h"

s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void func_8001ABF0(u16 *dst, u16 *src);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FE3A0(s32 input_level, u32 input_value, s32 input_x, s32 input_y) {
    s16 arg0;
    u16 arg1;
    s16 arg2;
    s16 arg3;
    s16 *var_a1;
    s16 var_a3;
    u16 temp_a1;
    u32 temp_t0;

    arg0 = input_level;
    arg1 = input_value;
    arg2 = input_x;
    arg3 = input_y;
    temp_t0 = arg1 & 0xFFFF;
    if (arg0 < 0) {
        var_a1 = (s16 *) ((u8 *) D_8005378C + *D_80054BBC);
    } else {
        var_a3 = 1;
        if (arg0 == 5) {
            var_a1 = (s16 *) ((u8 *) D_800537A2 + *D_80054BBC);
        } else {
            for (; var_a3 < 5 && temp_t0 >= D_80127214[var_a3]; var_a3++) {
            }
            temp_a1 = D_80127214[var_a3 - 1];
            func_8001ABF0(func_8001A970(((s32) ((temp_t0 - temp_a1) * 100) /
                                              (s32) (D_80127214[var_a3] - temp_a1) +
                                          (var_a3 - arg0) * 100) &
                                            0xFFFF,
                                        &D_8012A028, 4),
                          (u16 *) ((u8 *) D_800535D0 + *D_80054BBC));
            var_a1 = &D_8012A028;
        }
    }
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) var_a1, (s32) arg2,
                  (s32) arg3);
}
