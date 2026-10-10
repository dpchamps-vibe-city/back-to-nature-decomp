#include "common.h"
#include "game_types.h"

void func_8001F278(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4);

s32 ovl_11_func_800CE37C(Ovl11CE37CArg0 *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a1;
    s32 var_v1;
    s32 var_a2;
    Ovl11CE37CSample sp18;
    char *p;

    func_8001F278(D_80128CEC, 0x11, (s32 *)&D_80128CE0, (s32 *)&D_80128CD0, &sp18.unk0);
    p = (char *)&D_8007AFF0;
    var_a2 = D_80128CE0.unk4 - 0x28;
    (*(u16 *)(p + 0x253B4)) = (u16)sp18.unk0;
    (*(u16 *)(p + 0x253B8)) = (u16)sp18.unk8;
    if (var_a2 < sp18.unk4) {
        var_a2 = sp18.unk4;
    }
    (*(s16 *)(p + 0x253B6)) = (s16)var_a2;
    if (D_80128CEC < 9) {
        var_v1 = 8 - D_80128CEC;
    } else {
        var_v1 = D_80128CEC - 8;
    }
    temp_v1 = var_v1 * 0x6F;
    temp_v1 = temp_v1 / 8;
    if (D_80128CEC < 9) {
        var_a1 = arg0->unk104 - temp_v1;
    } else {
        var_a1 = arg0->unk104 + temp_v1;
    }
    temp_v0 = D_80128CE0.unk4 - 0x28;
    if (var_a1 >= temp_v0) {
        var_a1 = temp_v0;
    }
    if (D_80128CEC >= 0x11) {
        ((struct struct_8006C838_view *)D_8006C838)->field_0C = ((struct struct_8006C838_view *)D_8006C838)->field_0C & 0xFFBFFFFF;
        arg0->unk100 = sp18.unk0;
        arg0->unk104 = D_80128CE0.unk4 - 0x28;
        arg0->unk108 = sp18.unk8;
        return 1;
    }
    arg0->unk104 = var_a1;
    arg0->unk100 = sp18.unk0;
    arg0->unk108 = sp18.unk8;
    D_80128CEC += 1;
    return 0;
}
