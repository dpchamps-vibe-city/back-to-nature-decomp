#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

s32 ovl_11_func_800D04D4(Ovl11A47CObject *arg0, u8 *arg1, s16 *arg2, s32 arg3, u16 *arg4);
s32 ovl_11_func_8010AE64(Recon_ovl_11_func_800D2594_A0View *arg0);
s32 ovl_11_func_800DF010(Ovl11ActionDispatchView *arg0, s32 arg1);
s32 ovl_11_func_800E0C24(Ovl11ActionDispatchView *arg0, s32 arg1);
void ovl_11_func_800D0698(Recon_ovl_11_func_800D2594_A0View *arg0);
s32 ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);

s32 ovl_11_func_8010A47C(Ovl11A47CObject *arg0) {
    s16 temp_v1;
    s32 var_s2;

    temp_v1 = arg0->unk28;
    var_s2 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        D_8012D070 = 0;
        break;
    case -1:
        var_s2 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        arg0->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (ovl_11_func_800D04D4(arg0, D_80127AD0, &arg0->unk28, -1, &D_8012D070) != 0) {
            arg0->unk28 = -1;
        }
        if (arg0->unk34 & 0x2000) {
            arg0->unk58 = D_8012D078[0x38 / 4];
            arg0->unk5C = D_8012D078[0x3C / 4];
            arg0->unk60 = D_8012D078[0x40 / 4];
            arg0->unk64 = D_8012D078[0x44 / 4];
            ovl_11_func_8010AE64((Recon_ovl_11_func_800D2594_A0View *) arg0);
            if (arg0->unk28 != 1) {
                arg0->unk28 = -1;
            }
        } else {
            if (D_8012D07C != 0) {
                ovl_11_func_800DF010((Ovl11ActionDispatchView *) D_8012D078, 0xC);
            } else {
                ovl_11_func_800E0C24((Ovl11ActionDispatchView *) D_8012D078, 0xC);
            }
            if (arg0->unk28 == 1) {
                D_8012D070 = 0;
                arg0->unk28++;
                if (((struct Ovl11UpdateWork *) &D_8007AFF0)->field_25476 == arg0->unk30) {
                    PlaySoundEffect(0x19);
                }
            }
            ovl_11_func_800D0698((Recon_ovl_11_func_800D2594_A0View *) arg0);
            ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        }
        arg0->unk34 |= 0x800;
        break;
    }
    return var_s2;
}
