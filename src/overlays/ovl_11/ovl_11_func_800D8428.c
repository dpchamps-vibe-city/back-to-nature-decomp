#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

Ovl11D124Entry *ovl_11_func_800DAF60(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D5868(s16 arg0);
s32 ovl_11_func_800D5F44(u16 a, u16 b);
s32 ovl_11_func_800D812C(s16 arg0, s16 arg1, Vec3 *arg2, s16 arg3);
void ovl_11_func_800CF95C(Ovl11Func800CF95CArg arg0, s32 arg1);
void ovl_11_func_800DA638(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);

s32 ovl_11_func_800D8428(s16 arg0, s16 arg1, s16 arg2) {
    Ovl11Func800CF95CArg sp18;
    Ovl11D124Entry *temp_v0;
    s16 var_s1;
    u16 temp_s1;
    u8 var_s2;

    temp_v0 = ovl_11_func_800DAF60((s32) arg0, arg1, arg2);
    if (temp_v0 == NULL) {
        return 0;
    }
    if ((((struct Ovl11B264StateView *)D_8006C838)->field_44BA == 3) && (arg0 == 0)) {
        return 0;
    }
    {
            temp_s1 = (u16) temp_v0->unk0;
            var_s2 = 0;
            if (ovl_11_func_800D5868(temp_v0->unk0) == 1) {
                var_s2 = temp_v0->unk4;
            }
            if (ovl_11_func_800D5F44(temp_s1, (u16) var_s2) == 0) {
                return 0;
            }
            if (((u16) temp_v0->unk0 == 0x169) || (var_s1 = 1, ((u16) temp_v0->unk2 == 0x16B))) {
                var_s1 = 2;
                temp_v0->unk0 = 0x169;
                temp_v0->unk2 = 0x169;
            } else {
                temp_v0->unk0 = 0x168;
                temp_v0->unk2 = 0x168;
            }
            if ((arg0 == 0) && (((struct Ovl11B264StateView *)D_8006C838)->field_E4C8 == 1)) {
                var_s1 = 2;
                temp_v0->unk0 = 0x169;
                temp_v0->unk2 = 0x169;
            }
            temp_v0->unk4 = 0;
            temp_v0->unk5 = 0;
            temp_v0->unk0 = (s16) (u16) temp_v0->unk2;
            temp_v0->unk6 = (u16) temp_v0->unk6 & 0x7FFF;
            ovl_11_func_800D812C(arg1, arg2, (Vec3 *)&sp18, arg0);
            sp18.unk4 = 0;
            ovl_11_func_800CF95C(sp18, 0);
            D_80123DFC[arg0](arg1, arg2, var_s1);
            ovl_11_func_800DA638(arg2, arg1, arg0, 1, 0);
            return 1;
    }
}
