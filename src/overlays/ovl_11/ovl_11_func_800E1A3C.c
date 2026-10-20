#include "common.h"
#include "game_types.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800D04D4(Ovl11D04D4Obj *arg0, u8 *arg1, s16 *arg2, s32 arg3, u16 *arg4);

s32 ovl_11_func_800E1A3C(Ovl11D04D4Obj *arg0) {
    s32 result;

    result = 0;
    switch (arg0->unk28) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk2C = 0;
        arg0->unk34 |= 0x800;
        ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x12, 0x8031);
        break;
    case -1:
        result = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x4000;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (arg0->unk2C != 0) {
            arg0->unk2C--;
        }
        if (ovl_11_func_800D04D4(arg0, D_80123FDC, &arg0->unk28, -1, (u16 *)&arg0->unk2C) != 0) {
            arg0->unk28 = -1;
        } else {
            ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x12, 0x8031);
        }
        break;
    }
    return result;
}
