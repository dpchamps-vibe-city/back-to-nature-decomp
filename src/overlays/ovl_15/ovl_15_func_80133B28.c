#include "common.h"
#include "sound_commands.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
/* Both return paths in 801344E8 produce sign-extended halfwords. */
s16 ovl_15_func_801344E8(u8 *arg0, s16 arg1);
void func_80024770(s32 arg0, s16 arg1, s16 arg2);
void func_800247C0(s32 arg0, s16 arg1, s16 arg2);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void func_8001AAF4(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);
void ovl_15_func_801342A0(u8 *arg0, s16 arg1, s16 arg2);
s32 ovl_15_func_801340B8(s32 arg0, s16 arg1);
void func_80024924(s32 arg0, s16 arg1, s16 arg2);
void ovl_15_func_80134000(s32 arg0, s16 arg1);
void ovl_15_func_80133F4C(s32 arg0, s16 arg1);

s32 ovl_15_func_80133B28(s16 arg0) {
    s32 sp18;
    s16 temp_v0;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_s4;
    s32 temp_s7;
    s32 temp_v1;
    s32 var_a2;
    s32 var_v0;
    s32 temp_v1_2;
    u8 *var_s3;

    sp18 = 0;
    if ((arg0 << 0x10) != 0) {
        var_s3 = D_801376D0;
    } else {
        var_s3 = (u8 *) &D_800742EC;
    }
    temp_a1 = gActiveRenderContext->field_D8;
    temp_s7 = temp_a1 + 0x14;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_s7, (s32) &D_800520A0[*gTextBankOffsets]);
    temp_s4 = ovl_15_func_801344E8(var_s3, -1);
    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 0);
    if (temp_v1 & 0x1000) {
        PlaySoundEffect(5);
        D_801375A2 -= 1;
    } else if (temp_v1 & 0x4000) {
        PlaySoundEffect(5);
        D_801375A2 += 1;
    }
    temp_v1_2 = temp_s4 - 1;
    if ((s16) D_801375A2 < 0) {
        D_801375A2 = temp_v1_2;
    }
    if ((s32) temp_v1_2 < (s16) D_801375A2) {
        D_801375A2 = 0;
    }
    if (temp_s4 >= 5) {
        if (((s16) D_801375A0 + 3) < (s16) D_801375A2) {
            D_801375A0 = D_801375A2 - 3;
        }
        if ((s16) D_801375A2 < (s16) D_801375A0) {
            D_801375A0 = (s16) D_801375A2;
        }
    }
    if (temp_s4 == 0) {
        D_801375A2 = 0;
        D_801375A0 = 0;
    }
    if (temp_s4 < (s16) D_801375A0) {
        func_80024770(temp_s7, 0x120, 0x30);
    }
    if (((s16) D_801375A0 + 4) < temp_s4) {
        func_800247C0(temp_s7, 0x120, 0x80);
    }
    func_800248B0(temp_s7, 0x20, (s16) ((((s16) D_801375A2 - (s16) D_801375A0) * 0x18) + 0x3C));
    for (var_a2 = 0; var_a2 < 4 && (var_v0 = var_a2 + 1, temp_s4 >= (s16) D_801375A0 + var_v0); var_a2++) {
        temp_s1 = var_a2 + 1;
        temp_s0 = var_a2 * 0x18;
        temp_v0 = temp_s0 + 0x36;
        func_8001AAF4((s16) ((u16) D_801375A0 + temp_s1), 2, gActiveRenderContext->field_D8 + 0x14, 0x20, (s16) (s32) temp_v0);
        ovl_15_func_801342A0(var_s3, ovl_15_func_801344E8(var_s3, (s16) ((u16) D_801375A0 + temp_s1)), (s16) (temp_s0 + 0x30));
        if (ovl_15_func_801340B8((s32) var_s3, ovl_15_func_801344E8(var_s3, (s16) ((u16) D_801375A0 + temp_s1))) == 1) {
            func_80024924(temp_s7, 0x20, temp_v0);
        }
    }
    temp_a0 = *(s32 *) ((u8 *) gPad0Input + 8);
    if (temp_a0 & 0x20) {
        PlaySoundEffect(1);
        sp18 = -1;
    } else if (temp_a0 & 0x40) {
        if (temp_s4 == 0) {
            PlaySoundEffect(2);
        } else if (ovl_15_func_801340B8((s32) var_s3, ovl_15_func_801344E8(var_s3, (s16) (D_801375A2 + 1))) == 1) {
            ovl_15_func_80134000((s32) var_s3, ovl_15_func_801344E8(var_s3, (s16) (D_801375A2 + 1)));
            PlaySoundEffect(1);
        } else if ((s16) D_801375A4 >= 5) {
            PlaySoundEffect(2);
        } else {
            ovl_15_func_80133F4C((s32) var_s3, ovl_15_func_801344E8(var_s3, (s16) (D_801375A2 + 1)));
            PlaySoundEffect(0);
        }
    } else if (temp_a0 & 0x800) {
        PlaySoundEffect(0);
        sp18 = 1;
    }
    return sp18;
}
