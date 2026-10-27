/*
 * Primary-object frame service: select live or substituted button masks,
 * dispatch object actions, advance timers/effects, and retain position history.
 * The adjacent direction remapper consumes a latch maintained only here.
 * History A samples on odd ticks; history B and direction tags sample when
 * tick % 8 == 7. Early exits deliberately skip those samples.
 *
 * Public contract: ovl11_primary_object_update.h. Object-specific movement,
 * collision and action services remain separately linked; co-location does
 * not give either entry private linkage.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_update.h"
#include "screen_fade.h"
#include "sound_commands.h"
#include "ovl11_clock_calendar.h"
#include "message_window.h"
#include "game_flags.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_held_action_selection.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_primary_object_grid_actions.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_object_action_input.h"
#include "ovl11_primary_object_action_phase.h"
#include "ovl11_scene_header_key.h"
#include "ovl11_primary_object_input_override.h"

void ovl_11_func_800CCF58 (Ovl11C7010Object *arg0);
void ovl_11_func_800C498C (Ovl11C498CObject *arg0);
s32 ovl_11_func_800CE654(void);
void Ovl11RemapPrimaryDirectionButtons(void);

void Ovl11UpdatePrimaryObject(void) {
    Ovl11PlayerUpdate *obj;
    PadInputState *input;
    s32 advanceAction;
    u32 cachedValue;
    s32 flags;

    obj = &((Ovl11UpdateState *)D_8006C838)->player;

    if ((*(s32 *) ((u8 *) obj + -0x51BC)) & 0x08000000) {
        if (gOvl11ActiveItemActionSound != -1) {
            StopSoundEffectVoice((s32) gOvl11ActiveItemActionSound);
            gOvl11ActiveItemActionSound = -1;
        }
    } else if ((Ovl11IsClockAdvanceActive() != 1) && ((obj->flags & 0x800002) || (IsScreenFadeComplete() != 0)) && !(obj->flags & 0x400) && (Ovl11SceneHeaderKeyDiffers(obj->collision) == 0)) {
        if (obj->flags & 0x2000) {
            ovl_11_func_800CCF58((Ovl11C7010Object *) obj);
            obj->flags = (s32) (obj->flags & ~0x2000);
        }
        if (GetGameFlag(0xADU) == 1) {
            SetGameFlag(0xADU, 0U);
            RequestMessageWindow(0, 0x724);
        }
        ovl_11_func_800C498C((Ovl11C498CObject *) obj);
        cachedValue = obj->flags;
        if ((cachedValue & 0x20000) && !(obj->flags & 0x20000010)) {
            Ovl11ResetPrimaryObjectRequest();
            gOvl11InteractionDispatchCount = 0;
            obj->flags = (s32) (obj->flags & 0xFFFDFFFF);
        }
        obj->flags36 = (u16) (obj->flags36 & 0xFFFB);
        if (obj->flags & 0x800000) {
            gOvl11PrimaryHeldButtons = ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C;
            gOvl11PrimaryPressedButtons = ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230;
            goto block_22;
        }
        input = gPad0Input;
        gOvl11PrimaryHeldButtons = input->heldButtons;
        gOvl11PrimaryPressedButtons = input->pressedButtons;
        if (ovl_11_func_800CE654() == 0) {
            Ovl11RemapPrimaryDirectionButtons();
            if (obj->flags36 & 0x40) {
                if (gOvl11PrimaryHeldButtons & 0x20) {
                    gOvl11PrimaryHeldButtons &= ~0x20;
                    gOvl11PrimaryPressedButtons &= ~0x20;
                } else {
                    obj->flags36 = (u16) (obj->flags36 & 0xFFBF);
                }
            }
block_22:
            Ovl11TickTimedSpriteEffect((Ov11Timer *)&obj->effectTimer);
            D_80128CB0 = 0;
            if (!(obj->flags & 0x20000000)) {
                switch ((u32)obj->flags & 0x800010) {
                case 0x800000:
                    Ovl11ApplyPrimaryObjectInputOverride(obj);
                    if (Ovl11DispatchPrimaryObjectAction() != 0) {
                        D_80128C64 = 0;
                    }
                    flags = obj->flags;
                    if (flags & 8) {
                        cachedValue = (u16)obj->timer;
                        if (obj->timer != 0) {
                            obj->timer = cachedValue - 1;
                        } else {
                            goto clear_flags;
                        }
                    }
                    break;
                case 0x10:
                    Ovl11UpdateHeldSelectionAction((Ovl11SelectionObject *)obj);
                    return;
                case 0:
                    Ovl11ApplyPrimaryObjectInputOverride(obj);
                    D_80128CB0 = Ovl11DispatchPrimaryObjectAction();
                    if (D_80128CB0 != 0) {
                        D_80128C64 = 0;
                        D_80128D30 = 0;
                    } else if (D_80128D30 == 0x10) {
                        D_80128D30 = 0;
                        gOvl11PrimaryDirectionRemap = 1;
                        if ((obj->direction == 2) || (obj->direction == 0)) {
                            gOvl11PrimaryDirectionRemap = 0;
                        }
                    } else {
                        D_80128D30 += 1;
                    }
                    flags = obj->flags;
                    if (flags & 8) {
                        if (obj->timer == 0) {
clear_flags:
                            obj->flags = flags & ~0xA;
                        } else {
                            obj->timer--;
                        }
                    }
                    break;
                }
            } else {
                Ovl11DispatchObjectActionInput((Ovl11C7010Object *)obj);
            }
            advanceAction = 0;
            if (obj->countdown != -2) {
                if (obj->countdown != -3) {
                    obj->countdown = (s16) ((u16) obj->countdown - 1);
                }
                if (obj->countdown == -1) {
                    advanceAction = 1;
                }
            } else if ((obj->flags36 & 0x10) || (obj->flags262 & 0x100)) {
                advanceAction = 1;
            }
            if (advanceAction != 0) {
                if (Ovl11StepPrimaryObjectAction((Ovl11C580CObject *)obj) != 0) {
                    Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) obj, 1U, 0, -2);
                    obj->flags = (s32) ((obj->flags & 0xDFFFFFFF) | 0x04000000);
                }
            }
            if (((Ovl11UpdateState *)D_8006C838)->tick & 1) {
                obj->historyA[9] = obj->historyA[8];
                obj->historyA[8] = obj->historyA[7];
                obj->historyA[7] = obj->historyA[6];
                obj->historyA[6] = obj->historyA[5];
                obj->historyA[5] = obj->historyA[4];
                obj->historyA[4] = obj->historyA[3];
                obj->historyA[3] = obj->historyA[2];
                obj->historyA[2] = obj->historyA[1];
                obj->historyA[1] = obj->historyA[0];
                obj->historyA[0] = obj->position;
            }
            if ((((Ovl11UpdateState *)D_8006C838)->tick & 7) == 7) {
                obj->historyB[9] = obj->historyB[8];
                obj->historyB[8] = obj->historyB[7];
                obj->historyB[7] = obj->historyB[6];
                obj->historyB[6] = obj->historyB[5];
                obj->historyB[5] = obj->historyB[4];
                obj->historyB[4] = obj->historyB[3];
                obj->historyB[3] = obj->historyB[2];
                obj->historyB[2] = obj->historyB[1];
                obj->historyB[1] = obj->historyB[0];
                obj->tags[9] = obj->tags[8];
                obj->tags[8] = obj->tags[7];
                obj->tags[7] = obj->tags[6];
                obj->tags[6] = obj->tags[5];
                obj->tags[5] = obj->tags[4];
                obj->tags[4] = obj->tags[3];
                obj->tags[3] = obj->tags[2];
                obj->tags[2] = obj->tags[1];
                obj->tags[1] = obj->tags[0];
                if (!(obj->flags & 0x100)) {
                    obj->historyB[0] = obj->position;
                    obj->tags[0].phase = (s16) ((u32) (((Ovl11UpdateState *)D_8006C838)->tick & 8) >> 3);
                    obj->tags[0].direction = (u16) obj->direction;
                    return;
                }
                obj->tags[0].direction = 5U;
            }
        }
    }
}

/* Keep snapshots of both masks: all four output bits must come from the
 * original nibble, not from earlier writes to the rotated result. */
void Ovl11RemapPrimaryDirectionButtons(void) {
    s32 held;
    s32 pressed;
    s32 heldUp;
    s32 heldWithoutDirections;
    s32 remap;

    remap = 0;
    if (((struct Ovl11DirectionRemapState *) D_8006C838)->mode == 3) {
        if (gPad0Input->stickMagnitude >= 0x56) {
            gOvl11PrimaryHeldButtons |= 0x20;
        }
    }
    if (gOvl11PrimaryDirectionRemap != 0) {
        remap = 1;
    }
    if (((struct Ovl11DirectionRemapState *) D_8006C838)->mode == 1) {
        remap = 1;
    }
    if (((struct Ovl11DirectionRemapState *) D_8006C838)->mode == 0) {
        remap = 0;
    }
    if (((struct Ovl11DirectionRemapState *) D_8006C838)->mode == 3) {
        remap = 0;
    }
    if (remap != 0) {
        held = gOvl11PrimaryHeldButtons;
        pressed = gOvl11PrimaryPressedButtons;
        heldWithoutDirections = held & 0xFFFF0FFF;
        heldUp = held & 0x1000;
        gOvl11PrimaryHeldButtons = heldWithoutDirections;
        gOvl11PrimaryPressedButtons = pressed & 0xFFFF0FFF;
        if (heldUp != 0) {
            gOvl11PrimaryHeldButtons = heldWithoutDirections | 0x8000;
        }
        if (pressed & 0x1000) {
            gOvl11PrimaryPressedButtons = (pressed & 0xFFFF0FFF) | 0x8000;
        }
        if (held & 0x4000) {
            gOvl11PrimaryHeldButtons |= 0x2000;
        }
        if (pressed & 0x4000) {
            gOvl11PrimaryPressedButtons |= 0x2000;
        }
        if (held & 0x8000) {
            gOvl11PrimaryHeldButtons |= 0x4000;
        }
        if (pressed & 0x8000) {
            gOvl11PrimaryPressedButtons |= 0x4000;
        }
        if (held & 0x2000) {
            gOvl11PrimaryHeldButtons |= 0x1000;
        }
        if (pressed & 0x2000) {
            gOvl11PrimaryPressedButtons |= 0x1000;
        }
    }
}
