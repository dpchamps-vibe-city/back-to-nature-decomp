#include "common.h"
#include "game_types.h"

void ovl_11_func_800FE558 (s16 arg0, s16 arg1, s16 arg2);
/* The original passes the signed row index; the callee masks it to a byte. */
void ovl_11_func_800FC7F0 (s32 arg0, s32 arg1, u8 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FD3CC(s16 arg0, s16 arg1) {
    s16 *var_a1;
    s32 var_a3;
    s32 temp_s4;
    s32 var_a2;
    u16 *var_s1;
    u16 temp_v1;
    u8 *temp_s2;

    temp_s2 = D_8007A638 + (arg0 * 0xF8);
    temp_s4 = D_8005E3C0->field_D8 + 0x68;
    ovl_11_func_800FE558((s16) (arg0 + 1), 0x18, (s16) (arg1 + 4));
    ovl_11_func_800FC7F0((s32) (s16) (((s16) *(u16 *) (temp_s2 + 0x16)) / 25), 0, 7U, arg0, (s32) ((void *) ((u8 *) temp_s2 + 4)), 0x28, (s16) (s32) arg1);
    if (*(u16 *) (temp_s2 + 0xB8) & 0x20) {
        func_80015EE8(temp_s4, (s32) D_8012CDF8, 8, 0, 0x96, (s16) (s32) arg1);
    }
    temp_v1 = *(u16 *) (temp_s2 + 0xB8);
    var_a2 = 0xD;
    if (!(temp_v1 & 2)) {
        var_a2 = 0xE;
        if (!(temp_v1 & 4)) {
            var_a2 = -1;
            if (temp_v1 & 1) {
                var_a2 = 0xF;
            }
        }
    }
    if (var_a2 >= 0) {
        var_s1 = (u16 *) ((u8 *) &D_80053618 + *D_80054BBC);
        func_80015EE8(temp_s4, (s32) D_8012CE88, var_a2 & 0xFF, 0, 0x96, (s16) (s32) arg1);
        var_a1 = func_8001A970((s32) *(u16 *) (temp_s2 + 0xB4), &D_8012A028, 2);
        *var_a1 = 0xFFD;
        var_a1++;
        var_a3 = arg1 + 4;
        while ((u16) (*var_a1 = *var_s1) != 0xFFFF) {
            var_s1++;
            var_a1++;
        }
        var_a1 = &D_8012A028;
    } else {
        var_a3 = arg1 + 4;
        var_a1 = (s16 *) ((u8 *) &D_8005362C + *D_80054BBC);
    }
    func_80017B3C(D_8005E3C0->field_D8 + 0x54, (s32) var_a1, 0xC6, (s16) var_a3);
}
