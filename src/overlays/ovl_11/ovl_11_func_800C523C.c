#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C9D64(void);
s32 ovl_11_func_800D5960(s16 *arg0, u32 arg1, s16 arg2, s16 *arg3, s16 *arg4, s16 arg5);
s32 ovl_11_func_800D806C(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
s32 ovl_11_func_800D8C6C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
s32 ovl_11_func_800F482C(Ovl11SpawnEffectRecord *arg0, s16 arg1, s16 arg2);
void ovl_11_func_800CD5BC(Ovl11CD5BCObject *arg0, s32 arg1);

void ovl_11_func_800C523C(Ovl11CD5BCObject *arg0) {
    Ovl11CE034Position sp18;
    u16 sp28;
    u16 sp2A;
    s16 sp2C;
    s16 sp2E;
    s16 temp_s3;
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s4;

    var_s4 = 0x28;
    temp_s0 = ovl_11_func_800C9D64();
    sp18.unk0 = arg0->x;
    sp18.unk4 = arg0->y;
    sp18.unk8 = arg0->z;
    temp_s3 = arg0->power / 20;
    if (temp_s0 != 2) {
        D_80123234[temp_s0](&sp18, &sp28, &sp2A);
        temp_v0 = ovl_11_func_800D5960(&arg0->value, 0U, arg0->direction, &sp2C, &sp2E, 0);
        if (temp_v0 == 1) {
            sp2C += sp28;
            sp2E += sp2A;
            if (ovl_11_func_800D806C(sp2C, sp2E, 0, (s16) temp_s0) == 0) {
                D_80128C64 += temp_v0 << temp_s3;
                if (ovl_11_func_800D8C6C((s16) temp_s0, sp2C, sp2E, D_80128C64, arg0->amount) == temp_v0) {
                    var_s4 = 0x96;
                }
            }
        }
    } else if (D_80128C54 & 0x8000) {
        D_80128C64 += 1 << temp_s3;
        if (ovl_11_func_800F482C(D_80128C4C_dispatch, D_80128C64, arg0->amount) == 1) {
            var_s4 = 0x96;
        }
    }
    ovl_11_func_800CD5BC(arg0, var_s4);
}
