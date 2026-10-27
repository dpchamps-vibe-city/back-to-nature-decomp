#include "common.h"
#include "game_types.h"
#include "ovl11_keyframe_step.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800E3200(Ovl11D04D4Obj *arg0) {
    s16 temp_s0;
    s16 temp_v1;
    s32 var_s2;

    temp_v1 = arg0->unk28;
    var_s2 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s2 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        arg0->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, D_801240BC, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        } else {
            temp_s0 = arg0->unk2A;
            ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *) arg0, 0x12, 0x31);
            arg0->unk2A = temp_s0;
        }
        break;
    }
    return var_s2;
}
