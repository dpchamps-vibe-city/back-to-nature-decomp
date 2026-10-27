/*
 * held_item_actions — reconstructed translation unit.
 *
 * Button-triggered held-item action selection, collision dispatch cleanup and
 * charge-start state. The four code handlers form an ordered try-chain;
 * special codes retain their separate pending-record and camera effects.
 * See ovl11_held_item_actions.h for input gates and return semantics.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_held_item_actions.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl11_grouped_value_mapping.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_halfword_triplets.h"
#include "map_texture_camera.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_primary_object_item_sprite_source.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_held_item_effect.h"

s32 Ovl11TryBeginLowCodeItemAction(Ovl11C7010Object *arg0, u16 *arg1) {
    s32 temp_s2;
    s32 var_a2;
    s16 item;

    if ((u32) (arg1[0] - 1) < 0x19U) {
        *(s16 *) ((u8 *) arg0 + 0x18) = 1;
        item = (s16) arg1[0];
        gOvl11ItemActionChargeFrames = 0;
        temp_s2 = Ovl11GetGroupedValueIndex(item);
        var_a2 = Ovl11MapGroupedValueCode((s16) arg1[0]) + (arg0->unk38 * 5);
        if ((u32) (arg1[0] - 0x15) < 5U) {
            if (gOvl11PrimarySurfaceFlags & 3) {
                arg0->unk6C |= 0x1800;
                Ovl11ChangePrimaryObjectState(arg0, 0x52,
                    (arg0->unk38 * 5) + *(s16 *) (arg1 + 1), -2);
                return 1;
            }
            if (*(s16 *) (arg1 + 2) == 0) {
                return 0;
            }
        }
        Ovl11ChangePrimaryObjectState(arg0, (temp_s2 * 2) + 0x16, var_a2, -2);
        return 1;
    }
    return 0;
}

void Ovl11ResetItemActionCollisionDispatch(void);
/* 800C7464 passes the full addition; 800C70CC stores its low halfword. */

s32 Ovl11TryBeginGroupedItemAction(Ovl11C7010Object *arg0, u16 *arg1) {
    u16 temp_v1;

    temp_v1 = *arg1;
    if ((u32) (temp_v1 - 0x1A) < 4U) {
        if ((s16) temp_v1 != 0x1D) {
            Ovl11ResetItemActionCollisionDispatch();
        }
        arg0->unk6C |= 0x1800;
        Ovl11ChangePrimaryObjectState(arg0, Ovl11GetGroupedValueIndex((s16) *arg1) + 0x4D, (s32) arg0->unk38, -2);
        PlaySoundEffect((s16) (arg0->unk3C + 0xFFBD));
        return 1;
    }
    return 0;
}

s32 Ovl11TryBeginHighCodeItemAction(u8 *arg0, u16 *arg1) {
    if ((u32) (*arg1 - 0x22) < 0x15U) {
        *(s32 *)(arg0 + 0x6C) |= 0x1800;
        Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)arg0, 0x53,
            gItemDataTable[*(s16 *)(arg0 + 0x84)].action.field_6, -2);
        return 1;
    }
    return 0;
}

/* The callee stores only the low byte of the animation selector. */

s32 Ovl11TryBeginSpecialItemAction(Ovl11C7010Object *arg0, u16 *arg1) {
    switch ((s16) *arg1) {
    case 0x38:
    case 0x39:
        Ovl11BeginHeldItemEffect(arg0);
        Ovl11InitPrimaryObjectItemSpriteSource((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
        SelectSpriteAnimation((ObjectState *) &arg0->sprite, (u8) arg0->unk38);
        return 1;
    case 0x21:
        Ovl11ChangePrimaryObjectState(arg0, 0x32, 0x47, 0xA);
        Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + 0xF0), (u16 *) &arg0->unk84);
        arg0->unk6C |= 0x8000;
        return 1;
    case 0x1E:
        Ovl11ResetItemActionCollisionDispatch();
        arg0->unk6C |= 0x1800;
        Ovl11ChangePrimaryObjectState(arg0, 0x51, arg0->unk38, -2);
        Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + 0xF0), (u16 *) &arg0->unk84);
        arg0->unk6C |= 0x8000;
        return 1;
    case 0x1F:
    case 0x20:
        Ovl11ResetItemActionCollisionDispatch();
        arg0->unk6C |= 0x1800;
        Ovl11ChangePrimaryObjectState(arg0, 0x55, arg0->unk38, -2);
        Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + 0xF0), (u16 *) &arg0->unk84);
        arg0->unk6C |= 0x8000;
        return 1;
    case 0x12D:
        arg0->unk6C |= 0x400;
        Ovl11EnterMapCameraMode();
        return 1;
    default:
        return 0;
    }
}

typedef struct {
    /* 0x00 */ char pad_00[0x26];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ char pad_2E[0x34 - 0x2E];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ char pad_38[0xB6 - 0x38];
    /* 0xB6 */ s16 unkB6;
} Struct_800DF010;

s32 ovl_11_func_8010B57C(u16 *arg0, s32 arg1);

void Ovl11ResetItemActionCollisionDispatch(void) {
    u32 temp_v1;

    temp_v1 = gOvl11InteractionHitFlags & 0x206;
    switch (temp_v1) {                              /* irregular */
    case 0x4:
        /* fallthrough */
    case 0x2:
        if (gOvl11InteractionHitRecordView != NULL) {
            Ovl11DispatchCode160Handler((Struct_800DF010 *)gOvl11InteractionHitRecordView, 0);
            return;
        }
        return;
    case 0x200:
        if (gOvl11InteractionHitRecordView != NULL) {
            ovl_11_func_8010B57C(&gStageMode2Record, 0);
        }
        break;
    }
}

s32 Ovl11TryBeginLowCodeItemAction(Ovl11C7010Object *arg0, u16 *arg1);
s32 Ovl11TryBeginGroupedItemAction(Ovl11C7010Object *arg0, u16 *arg1);
s32 Ovl11TryBeginHighCodeItemAction(u8 *arg0, u16 *arg1);
s32 Ovl11TryBeginSpecialItemAction(Ovl11C7010Object *arg0, u16 *arg1);

s32 Ovl11UpdateHeldItemAction(Ovl11C7010Object *arg0) {
    s16 *temp_s0;
    s32 temp_a0;
    u16 pressed;
    u16 held;

    temp_a0 = arg0->unk6C;
    if (temp_a0 & 0x18000000) {
        return 0;
    }
    if (*(s16 *)((u8 *)arg0 + 0x16) >= 100) {
        return 1;
    }
    pressed = *(u16 *)&gOvl11PrimaryPressedButtons;
    held = *(u16 *)&gOvl11PrimaryHeldButtons;
    if (temp_a0 & 0x40) {
        return 0;
    }
    if (*(s16 *)((u8 *)arg0 + 0x18) != 0) {
        if (held & 0x80) {
            (*(s16 *)((u8 *)arg0 + 0x18))++;
            return 1;
        }
    } else if (pressed & 0x80) {
        temp_s0 = &arg0->unk84;
        if (Ovl11TryBeginLowCodeItemAction(arg0, (u16 *)temp_s0) ||
            Ovl11TryBeginGroupedItemAction(arg0, (u16 *)temp_s0) ||
            Ovl11TryBeginHighCodeItemAction((u8 *)arg0, (u16 *)temp_s0)) {
            arg0->sprite.field_5 = 0;
            return 1;
        }
        if (Ovl11TryBeginSpecialItemAction(arg0, (u16 *)temp_s0)) {
            return 1;
        }
    }
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    return 0;
}
