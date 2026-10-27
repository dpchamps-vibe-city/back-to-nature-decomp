#include "common.h"

#include "game_types.h"
#include "ovl11_keyframe_step.h"

s32 ovl_11_func_8010A254(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &D_80127AAC, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}
