#include "common.h"

void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_15_func_80134134(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5);

void ovl_15_func_801335F8(void) {
    s16 temp_v1;
    s16 var_a1;
    s32 var_s4;
    u8 *var_s2;
    s32 temp_v0;
    s32 temp_v1_2;
    s32 var_s1;
    s32 var_s3;
    s32 var_s5;
    s16 var_s7;
    s32 var_v1;
    s32 next;
    u8 *var_a2;
    Ovl15Slot *var_s0;

    var_s7 = 0x28;
    var_s5 = 0x22;
    func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x28), 1, 0x24, 0x10, 0x70, 0x8D);
    func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32) ((void *) ((u8 *) &D_80051CAE + *D_80054BBC)), 0x3C, 0x14);
    func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x28), 1, 0xAC, 0x10, 0x70, 0x8D);
    func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32) ((void *) ((u8 *) ((void *) ((u8 *) &D_80051CAE + 0xE)) + *D_80054BBC)), 0xC4, 0x14);
    var_v1 = 0;
    do {
        if (var_v1 != 0) {
            var_s2 = D_801376D0;
            var_s4 = var_s7 + 0x88;
        } else {
            var_s2 = (u8 *)&D_800742EC;
            var_s4 = var_s7 + 0x88;
        }
        next = var_v1 + 1;
        var_s0 = Ovl15Slots[var_v1];
        var_s1 = (var_s5 << 16) + 0x180000;
        var_s3 = 4;
        do {
            temp_v1 = var_s0->unk0;
            if (temp_v1 != -1) {
                if (temp_v1 == 0) {
                    var_a2 = var_s2 + var_s0->unk2 * 0xB4;
                    var_a1 = *(s16 *)(var_a2 + 0xAE);
                } else {
                    temp_v1_2 = var_s0->unk2 * 0xB8;
                    var_a2 = var_s2 + (temp_v1_2 + 0x708);
                    var_a1 = *(s16 *)(var_s2 + temp_v1_2 + 0x7B6);
                }
                ovl_15_func_80134134(*(s16 *)var_a2, var_a1, (s32)(var_a2 + 4), *(s16 *)(var_a2 + 0x16), var_s7, var_s5);
                temp_v0 = var_s1;
                var_s1 += 0x180000;
                var_s5 = temp_v0 >> 16;
            }
            var_s3--;
            var_s0++;
        } while (var_s3 >= 0);
        var_s7 = (s16)var_s4;
        var_v1 = next;
        var_s5 = 0x22;
    } while (var_v1 < 2);
}
