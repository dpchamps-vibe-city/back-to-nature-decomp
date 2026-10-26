/*
 * primary_object_action_phase — reconstructed translation unit.
 *
 * Prioritize primary-object action input, its forced-state entry and
 * fade-gated idle transition.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_primary_object_action_phase.h"
#include "game_types.h"
#include "sprite_rotation.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "area_grid_triggers.h"
#include "ovl11_counter_choice.h"
#include "ovl11_primary_object_direction.h"
#include "screen_fade.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_held_item_actions.h"
#include "ovl11_interaction_volume_actions.h"
#include "ovl11_item_list_exchange_actions.h"
#include "ovl11_interaction_record_actions.h"
#include "ovl11_item_request_actions.h"

/* Consume the dispatcher's forced-entry flag with a fixed position/velocity
 * setup. The constants are state-specific, not generic movement defaults. */
void Ovl11BeginPrimaryObjectState3B(Ovl11C7010Object *object) {
    object->unk6C |= 0x1800;
    Ovl11ChangePrimaryObjectState(object, 0x3BU, 0, 0x14);
    *(s32 *)((u8 *)object + 0x100) = -0x2286;
    SelectSpriteAnimation((ObjectState *)&object->sprite, 4);
    object->unk38 = 2;
    *(s32 *)((u8 *)object + 0x114) = -0x8C;
    object->sprite.field_5 = object->unk38;
    object->unk6C |= 0x40000000;
    PlaySoundEffect(6);
}

void Ovl11BeginPrimaryObjectState3B (Ovl11C7010Object *arg0);
s32 ovl_11_func_800CD734 (Ovl11C7010Object *arg0, s32 arg1);
/* This caller supplies a direction; the matched helpers do not use it. */
s32 ovl_11_func_800C7838 (u8 *arg0);
void ovl_11_func_800CF308 (s16 arg0);
s32 ovl_11_func_800C77D8 (Ovl11C7010Object *arg0, u16 arg1);
s32 Ovl11TryBeginIdleState54 (Recon_ovl_11_func_800C97D0_A0View *arg0);

s32 Ovl11DispatchPrimaryObjectAction(void) {
    Ovl11C7010Object *obj;
    u16 pressed;
    u16 held;
    s32 direction;
    u32 delay;

    obj = (Ovl11C7010Object *)&gPrimaryObjectBase;
    pressed = *(u16 *)&gOvl11PrimaryPressedButtons;
    held = *(u16 *)&gOvl11PrimaryHeldButtons;
    if ((((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x37 ||
         ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x43) &&
        Ovl11CheckAreaGridContact(obj)) {
        return 1;
    }
    if (!(obj->unk6C & 0xA)) {
        if ((obj->unk6C & 0x100) && (pressed & 0x40)) {
            Ovl11ChangePrimaryObjectState(obj, 0x3A, 0xB0, -2);
            return 1;
        }
        if (obj->unk6C & 0x40) {
            return 1;
        }
        if (*(u16 *)((u8 *)obj + 0x36) & 0x80) {
            Ovl11BeginPrimaryObjectState3B(obj);
            *(u16 *)((u8 *)obj + 0x36) &= ~0x80;
            return 1;
        }
        if (*(u16 *)((u8 *)obj + 0x36) & 0x20) {
            if (Ovl11GetSpriteRotationState(0x20) == 1) {
                *(u16 *)((u8 *)obj + 0x36) &= ~0x20;
                obj->unk6C |= 0x400;
                Ovl11BeginCounterChoice();
            }
            return 1;
        }
        if (Ovl11DispatchPrimaryInteractionInput((Recon_ovl_11_func_800C7270_A0View *)obj, pressed)) {
            return 1;
        }
        if (!(obj->unk6C & 0x40000108)) {
            if ((((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x02000000) &&
                ((struct struct_8006C838_time *)D_8006C838)->field_44C0 == 0xC) {
                return 0;
            }
            if (ovl_11_func_800CD734(obj, 0)) {
                return 0;
            }
            if (Ovl11DispatchInteractionRecordInput((Ovl11C8FA8Object *)obj, pressed)) {
                return 1;
            }
            if (pressed & 8) {
                if (obj->itemIndex == 0) {
                    Ovl11TryBeginStackListItemAction((Recon_ovl_11_func_800C7270_A0View *)obj, 0);
                }
                return 2;
            }
            if (pressed & 2) {
                Ovl11TryBeginSecondaryListItemAction((Recon_ovl_11_func_800C7270_A0View *)obj, 0);
                return 2;
            }
            if (!(obj->unk6C & 0x18000000)) {
                if (held & 8) {
                    direction = 2;
                    if (pressed & 0x8000) {
                        direction = 1;
                    }
                    if (pressed & 0x2000) {
                        direction = 0;
                    }
                    if (direction != 2) {
                        Ovl11TryBeginStackListItemAction((Recon_ovl_11_func_800C7270_A0View *)obj, direction);
                        return 2;
                    }
                }
                if (held & 2) {
                    direction = 2;
                    if (pressed & 0x8000) {
                        direction = 1;
                    }
                    if (pressed & 0x2000) {
                        direction = 0;
                    }
                    if (direction != 2) {
                        Ovl11TryBeginSecondaryListItemAction((Recon_ovl_11_func_800C7270_A0View *)obj, direction);
                        return 2;
                    }
                }
                if (pressed & 4) {
                    Ovl11ChangePrimaryObjectState(obj, 0x14, obj->unk38 + 0x43, -2);
                    ovl_11_func_800C7838((u8 *)obj);
                    ovl_11_func_800CF308(1);
                    PlaySoundEffect(0x12);
                    return 1;
                }
                if (pressed & 1) {
                    Ovl11ChangePrimaryObjectState(obj, 0x14, obj->unk38 + 0x43, -2);
                    ovl_11_func_800C7838((u8 *)obj);
                    ovl_11_func_800CF308(2);
                    PlaySoundEffect(0x12);
                    return 1;
                }
            }
            if (Ovl11DispatchItemRequestInput(obj, pressed)) {
                return 1;
            }
            if (Ovl11UpdateHeldItemAction(obj)) {
                return 0;
            }
            if (ovl_11_func_800C77D8(obj, pressed)) {
                return 1;
            }
            if (Ovl11TryBeginIdleState54((Recon_ovl_11_func_800C97D0_A0View *)obj)) {
                return 1;
            }
        }
    }
    if (!(obj->unk6C & 0x40000000)) {
        if (Ovl11UpdatePrimaryObjectDirection()) {
            delay = 0xC;
            if (obj->unk3C == 4) {
                delay = 8;
            }
            if (delay < gOvl11FacingStepSoundDelay) {
                gOvl11ActiveItemActionSound = PlaySoundEffect(0x48);
                gOvl11FacingStepSoundDelay = 0;
            } else {
                gOvl11FacingStepSoundDelay++;
            }
            return 1;
        }
    }
    return 0;
}

s32 Ovl11TryBeginIdleState54(Recon_ovl_11_func_800C97D0_A0View *arg0) {
    s32 callRet3;
    if ((D_8006C844 & 0x8000000) == 0) {
        callRet3 = IsScreenFadeComplete();
        if (callRet3 == 0) {
            return 0;
        } else {
            if ((arg0->unk6C & 0x18000000) == 0) {
                if (arg0->unk1A < 0x65) {
                    return 0;
                } else {
                    if ((arg0->unk6C & 0x400000) == 0) {
                        arg0->unk6C = arg0->unk6C | 0x1800;
                        Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)arg0, 0x54, 0, -2);
                        arg0->unk6C = arg0->unk6C | 0x400000;
                        return 1;
                    } else {
                        return 0;
                    }
                }
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }
}
