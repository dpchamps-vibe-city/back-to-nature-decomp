#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_8011D438 (u8 *arg0);
void func_80019E14 (s32 arg0, s16 arg1, s16 arg2);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800D6014 (u16 arg0);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void ovl_11_func_8011CEE0 (u16 *dst);
void ovl_11_func_8011CF10 (u16 *dst);
void func_800136D4 (u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_80014E90 (u32 *arg0, s16 arg1, s16 arg2, s32 arg3, s16 arg4);
void func_80014F0C (u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6);
void func_80014FAC (s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s16 arg9);
s32 func_80011F5C (s32 arg0);
void func_80011FD8 (s32 arg0);
void func_800248B0 (s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800CBB9C (Ovl11C7010Object *arg0, u16 arg1, s32 arg2);
void ovl_11_func_800F3898 (u16 arg0, u16 arg1);
void ovl_11_func_8011D084 (s32 arg0, s32 arg1);
void ovl_11_func_8011CBC8 (void);
s32 ovl_11_func_8011D934 (StructD548 x);
void ovl_11_func_8011CD2C (Ovl11D8012D538 s, s16 arg3, s32 arg4, s16 arg5, s16 arg6);
void ovl_11_func_8011CBE8 (void);
void ovl_11_func_8011CC08 (s32 arg0);
u16 *func_80017E34 (u16 *arg0, u16 *arg1);
void func_80024810 (s32 arg0, s16 arg1, s16 arg2);
void func_80024860 (s32 arg0, s16 arg1, s16 arg2);
void func_8001ABF0 (u16 *dst, u16 *src);

s32 ovl_11_func_800D740C(u16 arg0);                 /* static */

void ovl_11_func_8011D474(s32 *arg0, s16 arg1, s16 arg2) {
    /* Static-chain caller frame-address/call-boundary-dead at 0x8011D478; census ovl_11:ovl_11_func_8011D474 (paired). */
    auto s32 nested_ovl_11_func_8011D438(u8 *arg0) __asm__("ovl_11_func_8011D438");

    s32 sp1C;
    s16 *var_s1;
    s32 *var_s0;
    s32 *var_s2;
    s32 *var_s4;
    s32 var_a0;
    s32 temp_a1;
    s32 temp_s0;
    s32 temp_s6;
    s32 var_s3;
    s32 var_s5;
    s32 var_v0;
    u8 temp_a0;
    u8 temp_a2;

    temp_a2 = *(u8 *) ((u8 *) arg0 + 0x5A);
    temp_s6 = D_8005E3C0->field_D8 + 0x3C;
    sp1C = (temp_a2 * 8) + 0x1C;
    temp_s0 = (*(u8 *) ((u8 *) arg0 + 0x5B)) - temp_a2;
    var_s5 = nested_ovl_11_func_8011D438((u8 *) (arg0 + 0xF));
    if (var_s5 == 0) {
        var_s5 = temp_s0 - 2;
    } else {
        var_s5 = temp_s0 - 7;
    }
    var_s0 = arg0;
    var_s2 = arg0 + 0xA;
    var_s4 = arg0 + 0xF;
    var_s3 = 0;
    var_a0 = 0;
    do {
        if (*var_s0 != 0) {
            if (var_a0 == (*(s16 *) ((u8 *) arg0 + 0x5C))) {
                func_80019E14(temp_s6, (s16) (arg1 + 4), (s16) (arg2 + (s16) ((var_s3 * 0xE) + 4)));
            }
            func_80017B3C(temp_s6, *var_s0, (s16) (arg1 + 0xC), (s32) (s16) (arg2 + (s16) ((var_s3 * 0xE) + 4)));
            var_s1 = &D_8012D608;
            temp_a0 = *(u8 *) ((u8 *) var_s4 + 0);
            if (temp_a0 == 0) {
                ovl_11_func_8011CEE0(func_8001A970(ovl_11_func_800D6014(*(u16 *) ((u8 *) var_s2 + 0)), &D_8012D608, var_s5));
            } else if (temp_a0 == 2) {
                ovl_11_func_8011CF10(func_8001A970(ovl_11_func_800D740C(*(u16 *) ((u8 *) var_s2 + 0)), &D_8012D608, var_s5));
            } else {
                if (temp_a0 == 3) {
                    temp_a1 = var_s5 + 1;
                    for (var_v0 = 0; var_v0 < temp_a1; var_v0++) {
                        (&D_8012D608)[var_v0] = 0x60;
                    }
                    {
                        s32 byteOffset;
                        byteOffset = var_v0 * 2;
                        *(u16 *) ((u8 *) &D_8012D608 + byteOffset) = 0xFFFF;
                    }
                } else {
                    var_s1 = NULL;
                }
            }
            if (var_s1 != NULL) {
                func_80017B3C(temp_s6, (s32) var_s1, sp1C, (s32) (s16) (arg2 + (s16) ((var_s3 * 0xE) + 4)));
            }
            var_s3++;
        }
        var_a0++;
        var_s0++;
        var_s2 = (s32 *) ((u8 *) var_s2 + 2);
        var_s4 = (s32 *) ((u8 *) var_s4 + 1);
    } while (var_a0 < 10);
    func_800136D4((u32 *) temp_s6, arg1, arg2, ((*(u8 *) ((u8 *) arg0 + 0x5B)) * 8) + 0xF, (s16) (s32) (s16) ((var_s3 * 0xE) + 6));
}
