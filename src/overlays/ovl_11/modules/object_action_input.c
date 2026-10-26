/*
 * object_action_input — reconstructed translation unit.
 *
 * Current-action button dispatch, held charge progression and charge-release
 * animation/sound selection. The release helper consumes the charge tier
 * built by the dispatcher; it is not an independent timer. See the public
 * header for update units and return semantics.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_object_action_input.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl11_grouped_value_mapping.h"
#include "ovl11_position_transition.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_primary_object_state_change.h"

void Ovl11ReleaseChargedItemAction(Ovl11C7010Object *arg0) {
    s16 temp_s0;
    s32 soundBase;
    s32 soundOffset;

    temp_s0 = (s16) (*(u16 *) ((u8 *) arg0 + 0x18)) / 20;
    soundOffset = ((arg0->unk3C - 0x16) / 2) * 5;
    soundBase = temp_s0 + 0x1D;
    gOvl11ActiveItemActionSound = PlaySoundEffect((s16) (soundOffset + soundBase));
    Ovl11ChangePrimaryObjectState(arg0, arg0->unk3C + 1, Ovl11MapGroupedValueCode((s16) ((u16) arg0->unk84 - ((*(u16 *) ((u8 *) arg0 + 0x86)) - temp_s0))) + (arg0->unk38 * 5), -2);
    arg0->sprite.field_5 = temp_s0 + 1;
}

/* 800C6C64 explicitly supplies an unused fourth argument. */
s32 ovl_11_func_800D622C();
void Ovl11ReleaseChargedItemAction(Ovl11C7010Object *arg0);

s32 Ovl11DispatchObjectActionInput(Ovl11C7010Object *arg0) {
    s32 var_v0;
    u16 temp_v0;
    u8 temp_s1;
    u16 held;
    u16 pressed;

    if (!(arg0->unk6C & 0x40)) {
        held = *(u16 *)&gOvl11PrimaryHeldButtons;
        pressed = *(u16 *)&gOvl11PrimaryPressedButtons;
        temp_v0 = arg0->unk3C;
        switch (temp_v0) {
        case 0x4F:
            if (((*(s32 *) ((u8 *) arg0 + 0x264)) & ~0xFF) == 0x400) {
                PlaySoundEffect(0xC);
                return 0;
            }
            return 0;
        case 0x58:
            if (pressed & 0x40) {
                arg0->unk6C = (arg0->unk6C | 0x1800) & 0xBFFFFFFF;
                Ovl11ChangePrimaryObjectState(arg0, 0x4BU, 0, 0x2D);
                arg0->unk38 = 0;
                SelectSpriteAnimation((ObjectState *) &arg0->sprite, 4);
                (*(s32 *) ((u8 *) arg0 + 0x114)) = -0xFA;
                arg0->sprite.field_5 = (u8) (*(u16 *) ((u8 *) arg0 + 0x38));
                arg0->unk6C = (arg0->unk6C | 0x40000000) & ~1;
                (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xFEFF);
                PlaySoundEffect(6);
            }
            goto block_9;
        case 0x57:
            if (pressed & 0x40) {
                Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x58, 0, gOvl11ClockMinutePeriodTwice * 0xA);
            }
block_9:
            if (D_8006C844 & 0x100000) {
                ovl_11_func_800D622C(2, -1, 0, 0);
            }
            return 0;
        default:
block_30:
            return 0;
        case 0x3:
        case 0x31:
            if ((u8) arg0->sprite.field_5 >= 2U) {
                arg0->unk6C &= 0xFFFF7FFF;
            }
            return 0;
        case 0x2D:
            if (!(held & 0xF9FF)) {
                if (arg0->unk6C & 0x800000) {
                    if (!(D_80070C88 & 0x1000)) {
                        goto block_18;
                    }
                            return 0;
                }
                goto block_30;
            }
block_18:
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x2E, 2, -2);
            arg0->unk6C &= 0xFFBFFFFF;
            return 1;
        case 0x16:
        case 0x18:
        case 0x1A:
        case 0x1C:
        case 0x1E:
            if (held & 0x80) {
                if ((*(s16 *) ((u8 *) arg0 + 0x18)) < ((*(s16 *) ((u8 *) arg0 + 0x86)) * 0x14)) {
                    (*(s16 *) ((u8 *) arg0 + 0x18)) = (s16) ((u16) (*(s16 *) ((u8 *) arg0 + 0x18)) + 1);
                } else {
                    gOvl11ItemActionChargeFrames += 1;
                }
                arg0->sprite.field_5 = (*(s16 *) ((u8 *) arg0 + 0x18)) / 20;
                if ((u32) gOvl11ItemActionChargeFrames >= 0x3DU) {
                    arg0->unk6C |= 0x1800;
                    Ovl11ChangePrimaryObjectState(arg0, 0x3EU, 0, -2);
                    gOvl11ItemActionChargeFrames = 0;
                    return 0;
                }
                    return 0;
            }
            Ovl11ReleaseChargedItemAction(arg0);
            return 0;
        case 0x4A:
            temp_s1 = arg0->sprite.field_4;
            if (temp_s1 == 1) {
                var_v0 = Ovl11StepPositionTransition((Ovl11PositionTransitionObject *) arg0);
                if (var_v0 == temp_s1) {
                    var_v0 = -2;
                } else {
                    var_v0 = 0x84;
                }
                (*(s16 *) ((u8 *) arg0 + 0x1C)) = var_v0;
            }
            return 0;
        }
    } else {
        return 0;
    }
}
