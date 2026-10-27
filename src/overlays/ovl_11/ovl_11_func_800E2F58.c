#include "common.h"

#include "game_types.h"
#include "ovl11_keyframe_step.h"

extern u8 D_80124080;
extern u8 D_80124090;

s32 ovl_11_func_800E2F58(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;
    u8 *var_a1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (arg0->unk0 == 0x10A) {
            var_a1 = &D_80124090;
        } else {
            var_a1 = &D_80124080;
        }
        if (Ovl11StepPackedKeyframes(arg0, var_a1, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}
