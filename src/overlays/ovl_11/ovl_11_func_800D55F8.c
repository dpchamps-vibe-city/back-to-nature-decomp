#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_keyframe_step.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800D55F8(Ovl11D55F8Object *arg0) {
    s32 result;
    u8 *far_base;

    result = 0;
    switch (arg0->unk28) {
    case 0:
        Ovl11StartTimedSpriteEffect((Ovl11C35ECStatePair *)((u8 *)arg0 + 0xA8), 0x24, 0x2D);
        arg0->unk28 = 1;
        arg0->unk2C = 0;
        arg0->unk2A = 0;
        arg0->unk22 = (u16)D_80128D7A;
        arg0->unk34 = (arg0->unk34 & ~0x2000) | 0x800;
        ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x18, 0x9010);
        far_base = (u8 *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
            PlaySoundEffect(0x1A);
        }
        break;
    case -1:
        result = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (arg0->unk2C != 0) {
            arg0->unk2C--;
        }
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, D_80123C94, &arg0->unk28, -1, (u16 *)&arg0->unk2C) != 0) {
            arg0->unk28 = -1;
        } else {
            ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x18, 0x9010);
        }
        break;
    }
    return result;
}
