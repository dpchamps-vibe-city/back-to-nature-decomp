#include "common.h"
#include "sound_commands.h"

#include "game_types.h"
#include "ovl11_keyframe_step.h"

extern u8 D_801240C8;

s32 ovl_11_func_800E32E4(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;
    u16 temp_a0;
    char *far_base;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        far_base = (char *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
            temp_a0 = arg0->unk0;
            if (temp_a0 == 0x109) {
                PlaySoundEffect(0x17);
            } else if (temp_a0 == 0x10A) {
                PlaySoundEffect(0x1B);
            }
        }
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &D_801240C8, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}
