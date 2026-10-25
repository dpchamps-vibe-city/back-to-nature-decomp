#include "common.h"
#include "game_types.h"

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ char pad_02[0x14];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ char pad_18[0x2];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ char pad_20[0x14];
    /* 0x34 */ u32 unk34;
    /* 0x38 */ char pad_38[0x76];
    /* 0xAE */ s16 unkAE;
} Ovl15Rec;

extern s16 D_8013759E;
extern s16 D_80137690[];

void ovl_15_func_80134134(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
s32 ovl_15_func_80134444(Ovl15Func80134444Arg *arg0);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80024A10(s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void ovl_15_func_801342A0(u8 *arg0, s16 arg1, s16 arg2) {
    Ovl15Rec *var_s2;
    u8 *var_v1;
    s32 temp_s5;
    s16 var_s4;
    s16 var_s1;
    s16 var_a0;
    s16 var_a1;
    void *var_a2;
    u32 i;

    temp_s5 = gActiveRenderContext->field_D8 + 0x14;
    var_s4 = 0x58;
    if (D_8013759E == 0) {
        var_s2 = (Ovl15Rec *) (arg0 + arg1 * 0xB4);
        var_a1 = var_s2->unkAE;
        var_a2 = (void *) ((u8 *) var_s2 + 4);
    } else {
        var_s2 = (Ovl15Rec *) (arg0 + (arg1 * 0xB8 + 0x708));
        var_v1 = (u8 *) arg0 + arg1 * 0xB8;
        var_a1 = *(s16 *) (var_v1 + 0x7B6);
        var_a2 = (void *) ((u8 *) var_s2 + 4);
    }
    var_s1 = var_s4 + 0x58;
    ovl_15_func_80134134(var_s2->unk0, var_a1, (s32) var_a2, var_s2->unk16, 0x40, arg2);
    for (i = 0; i < 0xF; i++) {
        D_80137690[i] = 0xFFD;
    }
    if (var_s2->unk34 & 0x40) {
        D_80137690[0] = 0x90;
    }
    D_80137690[4] = 0x71;
    func_8001A970(var_s2->unk1E + 1, &D_80137690[5], 2);
    D_80137690[8] = 0;
    D_80137690[9] = 0x26;
    D_80137690[10] = 0x24;
    *((u16 *) func_8001A970(ovl_15_func_80134444((Ovl15Func80134444Arg *) ((u8 *) var_s2 + 0x1A)), &D_80137690[11], 2)) = 0xFFFF;
    var_a0 = var_s4 + 0x58;
    func_80017B3C(temp_s5, (s32) D_80137690, (s32) var_a0, (s32) arg2);
    func_80024A10(temp_s5, (s16) (var_a0 + 0x10), arg2, var_s2->unk1C);
}
