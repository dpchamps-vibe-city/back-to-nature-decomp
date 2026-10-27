/*
 * primary_object_state_change — reconstructed translation unit.
 *
 * Primary-object state transitions, previous-state capture and flag-selected
 * sprite animation variants. The selectors below are exclusive helpers of
 * the full transition path; the direct path implements the same state/flags
 * protocol without variant selection. See the public header for differences.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_primary_object_state_change.h"
#include "game_types.h"
#include "pad_actuator_sequences.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_primary_object_draw.h"
#include "ovl11_primary_object_item_sprite_source.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_stage_mode2_record.h"

/* Each selector updates an overlaid word/byte view of the caller's animation
 * parameter. Unlisted states retain the input animation. Flag-selected calls
 * are sequential, not mutually exclusive; a later selector may override it. */
void Ovl11SelectPrimaryAnimationForFlag10000000(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1) {
    switch (arg0->unk3C) {
    case 1:
        arg1->unk0.unk0_w = 0x1B;
        break;
    case 2:
        arg1->unk0.unk0_w = arg0->unk38 + 0xF;
        break;
    case 4:
        arg1->unk0.unk0_w = arg0->unk38 + 0x13;
        break;
    }
    SelectSpriteAnimation((ObjectState *)((u8 *)arg0 + 0x260), arg1->unk0.unk0_b);
}

void Ovl11SelectPrimaryAnimationForFlag08000000(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1) {
    switch (arg0->unk3C) {
    case 1:
        arg1->unk0.unk0_w = 0x29;
        break;
    case 2:
        arg1->unk0.unk0_w = arg0->unk38 + 0x2A;
        break;
    case 4:
        arg1->unk0.unk0_w = arg0->unk38 + 0x13;
        break;
    }
    SelectSpriteAnimation((ObjectState *)((u8 *)arg0 + 0x260), arg1->unk0.unk0_b);
}

void Ovl11SelectPrimaryState2Animation(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1) {
    switch (arg0->unk3C) {
    case 2:
        arg1->unk0.unk0_w = arg0->unk38 + 0xAC;
        break;
    case 4:
        arg0->unk3C = 2;
        arg1->unk0.unk0_w = arg0->unk38 + 0xAC;
        break;
    }
    SelectSpriteAnimation((ObjectState *)((u8 *)arg0 + 0x260), arg1->unk0.unk0_b);
}

void Ovl11SelectPrimaryAnimationForFlag100(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1) {
    switch (arg0->unk3C) {
    case 1:
        arg1->unk0.unk0_w = 0x36;
        break;
    case 2:
        arg1->unk0.unk0_w = arg0->unk38 + 0x3B;
        break;
    case 4:
        arg1->unk0.unk0_w = arg0->unk38 + 0x3B;
        RequestPadActuatorSequence(0, 0);
        break;
    }
    SelectSpriteAnimation((ObjectState *)((u8 *)arg0 + 0x260), arg1->unk0.unk0_b);
}

void Ovl11SelectPrimaryFlaggedAnimation(Ovl11C7010Object *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1) {
    SpriteSourceData *sprite;
    s32 flags;

    flags = arg0->unk6C;
    if (flags & 0x100) {
        if (arg0->unk3C != 0x39) {
            arg1->unk0.unk0_w = arg0->unk38 + 0x3F;
            SelectSpriteAnimation((ObjectState *)&arg0->sprite, arg1->unk0.unk0_b);
            Ovl11SelectStageMode2AnimationFromObject(arg0);
        }
    } else {
        if (flags & 0x18000000) {
            arg1->unk0.unk0_w = 0x1C;
        } else {
            arg1->unk0.unk0_w = 0xA;
        }
        sprite = &arg0->sprite;
        SelectSpriteAnimation((ObjectState *)sprite, arg1->unk0.unk0_b);
        if (GetSpriteFrameControlHighBit(sprite) != 0) {
            arg0->sprite.field_5 = arg0->unk38;
        }
    }
}

/* This state-switch family passes unsigned animation bytes; the callee
 * stores only the low byte (as in 800C7010 and 800C7270). */
void Ovl11SelectPrimaryFlaggedAnimation(Ovl11C7010Object *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag10000000(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag08000000(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryState2Animation(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag100(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);

void Ovl11ChangePrimaryObjectState(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u16 previousState;

    arg0->unk6C = (arg0->unk6C | 0x20000000) & ~0x04000000;
    previousState = arg0->unk3C;
    arg0->unk3C = arg1;
    *(s16 *)((u8 *)arg0 + 0x1C) = (s16)arg3;
    *(u16 *)((u8 *)arg0 + 0x3E) = previousState;
    if (arg0->unk6C & 0x40000000) {
        SelectSpriteAnimation((ObjectState *)&arg0->sprite, (u8)arg2);
        Ovl11SelectPrimaryFlaggedAnimation(arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
        return;
    }
    if (arg0->unk6C & 0x800) {
        Ovl11InitPrimaryObjectItemSpriteSource((Recon_ovl_11_func_800CCFF8_A0View *)arg0, 1);
        SelectSpriteAnimation((ObjectState *)&arg0->sprite, (u8)arg2);
        arg0->unk6C &= ~0x800;
        return;
    }
    if (!(arg0->unk6C & 0x1000)) {
        SelectSpriteAnimation((ObjectState *)&arg0->sprite, (u8)arg2);
        if (arg0->unk6C & 0x10000000) {
            Ovl11SelectPrimaryAnimationForFlag10000000((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
        }
        if (arg0->unk6C & 0x08000000) {
            Ovl11SelectPrimaryAnimationForFlag08000000((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
        }
        if (!(arg0->unk6C & 0x18000000)) {
            if (arg0->unk6C & 0x200000) {
                Ovl11SelectPrimaryState2Animation((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
            }
        }
        if ((arg0->unk6C & 0x100) && (arg0->unk3C != 0x39)) {
            Ovl11SelectPrimaryAnimationForFlag100((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
            if (!(arg0->unk6C & 0x80)) {
                Ovl11SelectStageMode2AnimationFromObject(arg0);
            } else {
                Ovl11SyncSavedObjectAnimation(arg0);
            }
        }
        if (GetSpriteFrameControlHighBit(&arg0->sprite) != 0) {
            arg0->sprite.field_5 = *(u8 *)((u8 *)arg0 + 0x38);
        }
    }
}

s32 Ovl11ChangeObjectStateDirect(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 ret;

    arg0->unk6C = (arg0->unk6C | 0x20000000) & -0x4000001;
    arg0->unk3E = arg0->unk3C;
    arg0->unk3C = arg1;
    SelectSpriteAnimation((ObjectState *)((u8 *)arg0 + 0x260), arg2 & 0xFF);
    ret = GetSpriteFrameControlHighBit((SpriteSourceData *)((u8 *)arg0 + 0x260));
    if (ret != 0) {
        arg0->unk1C = arg3;
        ret = arg0->unk38;
        arg0->unk265 = ret;
    }
    return ret;
}
