/*
 * held_action_selection — reconstructed translation unit.
 *
 * Held-input action state machine with range-seeded delays, gated weighted
 * outcomes and a projected action sprite. The driver and its three local
 * helpers share the action-state protocol, not the preceding message-choice
 * state.
 *
 * Shared action state stores pulse ticks, delay ticks and a category. The
 * caller supplies the object and holds input bit 0x80 to keep the action
 * running. Completion can select an outcome and set persistent game flags.
 * See ovl11_held_action_selection.h and notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_held_action_selection.h"
#include "pad_actuator_sequences.h"
#include "sound_commands.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "game_flags.h"
#include "psyq/libcd.h"
#include "psyq/memory.h"
#include "sprite_render.h"
#include "point_projection.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_status_gauge_deltas.h"

s32 ovl_11_func_800CD3C4(s32 arg0);
void ovl_11_func_800CD08C(Recon_ovl_11_func_800CD08C_A0View *arg0);
s16 Ovl11SelectHeldActionDelay(Ovl11SelectArg *state);
s16 Ovl11SelectHeldActionOutcome(s32 *state);
void Ovl11DrawHeldActionSprite(s32 arg0);

s32 Ovl11UpdateHeldSelectionAction(Ovl11SelectionObject *obj) {
    s32 *state;
    u16 flags;
    s32 chance;
    s32 threshold;
    s16 item[3];
    struct Ovl11SpriteWork *work;

    state = gOvl11HeldActionState;
    flags = *(u16 *)&gOvl11PrimaryHeldButtons;
    switch (obj->phase) {
    case 0x45:
        if (obj->flags & 0x40) return 0;
        obj->phase = 0;
        state[2] = 0;
        if (!(obj->flags & 8)) {
            Ovl11ApplyStatusDelta(-1, (s8)ovl_11_func_800CD3C4(0), 0, 0);
        }
        work = (struct Ovl11SpriteWork *)&D_8007AFF0;
        state[2] = 4;
        if (work->header[1] == 0x2C) state[2] = 0;
        if (work->header[1] == 0xD) state[2] = 1;
        if (work->header[1] == 0xC) state[2] = 2;
        if (work->header[1] == 0x42) state[2] = 3;
        state[1] = Ovl11SelectHeldActionDelay((Ovl11SelectArg *)state);
        if (gOvl11HeldActionSignal == 0) {
            state[2] = -1;
            obj->phase = 3;
            SelectSpriteAnimation(&obj->animation, (u8)(obj->direction + 4));
            StopPadActuatorSequence(0);
            PlaySoundEffect(15);
        } else {
            gOvl11HeldActionSignal = 0;
            RequestPadActuatorSequence(0, 6);
        }
        break;
    case 0:
        if (obj->flags & 0x40) return 0;
        if (!(flags & 0x80)) {
            obj->phase = 2;
            SelectSpriteAnimation(&obj->animation, (u8)(obj->direction + 4));
            PlaySoundEffect(15);
        } else {
            if (state[1] == 0) {
                chance = 10;
                if (obj->id == 0x39) chance = 20;
                threshold = chance * 2;
                if ((s32)Rand(100) < threshold) {
                    obj->phase = 1;
                    SelectSpriteAnimation(&obj->animation, (u8)(obj->direction + 9));
                    RequestPadActuatorSequence(0, 0);
                    return 0;
                }
                state[1] = Ovl11SelectHeldActionDelay((Ovl11SelectArg *)state);
            } else if (state[2] != -1) {
                state[1]--;
            }
            SelectSpriteAnimation(&obj->animation, 8);
            obj->animation.field_5 = obj->direction;
            Ovl11DrawHeldActionSprite(0);
        }
        break;
    case 2:
        if (obj->animation.field_2 & 0x100) {
            obj->flags &= ~0x30;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *)obj);
        }
        break;
    case 3:
        if (obj->animation.field_2 & 0x100) {
            item[2] = 1;
            obj->flags &= ~0x30;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *)obj);
            if (state[2] != -1) {
                item[0] = Ovl11SelectHeldActionOutcome(state);
                if (item[0] != 0) Ovl11InstallPrimaryObjectRequest((u16 *)item);
            }
        }
        break;
    case 1:
        if (!(flags & 0x80)) {
            obj->phase = 3;
            SelectSpriteAnimation(&obj->animation, (u8)(obj->direction + 4));
            StopPadActuatorSequence(0);
            PlaySoundEffect(15);
        } else if (state[0] == 0) {
            obj->phase = 0;
            state[1] = Ovl11SelectHeldActionDelay((Ovl11SelectArg *)state);
            StopPadActuatorSequence(0);
        } else {
            state[0]--;
            Ovl11DrawHeldActionSprite(1);
        }
        break;
    }
    return 0;
}

/* First inclusive range match seeds the pulse timer. No matching range
 * returns zero without resetting it; category bounds are caller obligations. */
s16 Ovl11SelectHeldActionDelay(Ovl11SelectArg *state) {
    Ovl11HeldActionRange *entry;
    s16 rangeValue;
    u32 i;

    entry = gOvl11HeldActionRangeValues;
    rangeValue = D_80070D02;
    for (i = 0; i < 4; i++, entry++) {
        if (rangeValue < entry->min) {
            continue;
        }
        if (entry->max < rangeValue) {
            continue;
        }
        state->pulseTicks = Rand(gOvl11HeldActionDelayBounds[state->category]) + 5;
        return *(s16 *)(state->category * 2 + (u8 *)entry + 4);
    }
    return 0;
}

/* A rejected gated outcome does not reroll or stop: later cumulative
 * thresholds can still accept the SAME roll. One-shot outcomes set flags;
 * ordinary outcomes participate in the shared twelve-result fallback. */
s16 Ovl11SelectHeldActionOutcome(s32 *state) {
    s16 *table;
    s32 count;
    s32 total;
    s32 roll;
    s32 i;

    table = gOvl11HeldActionDefaultWeightTables[((struct Ovl11WeightedSelectionState *)D_8006C838)->mode];
    switch (state[2]) {
    case 1: table = gOvl11HeldActionCategory1Weights; break;
    case 2: table = gOvl11HeldActionCategory2Weights; break;
    case 3: table = gOvl11HeldActionCategory3Weights; break;
    case 4: table = gOvl11HeldActionCategory4Weights; break;
    }
    count = *table++;
    total = 0;
    roll = Rand(100);
    for (i = 0; i < count; i++, table += 2) {
        total += table[0];
        if (roll < total) {
            switch (table[1]) {
            case 0xEA:
                if (GetGameFlag(0x5A) == 1) break;
                SetGameFlag(0x5A, 1);
                return table[1];
            case 0x67:
                if (GetGameFlag(0x53) == 1) break;
                SetGameFlag(0x53, 1);
                return table[1];
            case 0xED:
                if (GetGameFlag(0x54) != 1 &&
                    GetGameFlag(0x55) == 1 &&
                    GetGameFlag(0x56) == 1 &&
                    GetGameFlag(0x57) == 1 &&
                    GetGameFlag(0x58) == 1 &&
                    GetGameFlag(0x59) == 1) {
                    SetGameFlag(0x54, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 4;
                    return table[1];
                }
                break;
            case 0xF0:
                if (GetGameFlag(0x57) != 1 && GetGameFlag(0x5E) == 1) {
                    SetGameFlag(0x57, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x20;
                    return table[1];
                }
                break;
            case 0xF1:
                if (GetGameFlag(0x58) != 1 &&
                    (((struct Ovl11WeightedSelectionState *)D_8006C838)->time < 0x1E0 ||
                     ((struct Ovl11WeightedSelectionState *)D_8006C838)->time > 0x528)) {
                    SetGameFlag(0x58, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x40;
                    return table[1];
                }
                break;
            case 0xF2:
                if (GetGameFlag(0x59) != 1 && GetGameFlag(0x5F) == 1) {
                    SetGameFlag(0x59, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x80;
                    return table[1];
                }
                break;
            case 0xEF:
                if (GetGameFlag(0x56) == 0) {
                    SetGameFlag(0x56, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x10;
                    return table[1];
                }
                break;
            case 0xEE:
                if (GetGameFlag(0x55) != 1 &&
                    GetGameFlag(0x5B) == 1 &&
                    GetGameFlag(0x5C) == 1 &&
                    GetGameFlag(0x5D) == 1) {
                    SetGameFlag(0x55, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 8;
                    return table[1];
                }
                break;
            default:
                if (table[1] >= 0xA1 && table[1] <= 0xA3) {
                    gOvl11HeldActionFallbackCounter = 0;
                } else if (gOvl11HeldActionFallbackCounter < 11) {
                    gOvl11HeldActionFallbackCounter++;
                } else {
                    gOvl11HeldActionFallbackCounter = 0;
                    return gOvl11HeldActionFallbackValues[state[2]];
                }
                return table[1];
            }
        }
    }
    return 0;
}

/* Mode zero bobs at 22/4096 turns per update; nonzero mode advances a
 * four-pixel vertical cycle, but only after successful projection. */
void Ovl11DrawHeldActionSprite(s32 mode) {
    s32 nextPhase;
    s32 signedPhase;
    s32 depth;
    s32 bobPixels;
    s32 sine;

    if (mode == 0) {
        sine = csin(gOvl11HeldActionSpritePhase);
        if (sine < 0) {
            sine += 0x3FF;
        }
        bobPixels = (sine << 6) >> 0x10;
        nextPhase = (u16) gOvl11HeldActionSpritePhase + 0x16;
        signedPhase = (s16) nextPhase;
        gOvl11HeldActionSpritePhase = nextPhase;
        if (signedPhase < 0) {
            signedPhase += 0xFFF;
        }
        gOvl11HeldActionSpritePhase = nextPhase - ((signedPhase >> 0xC) << 0xC);
    } else {
        bobPixels = 0;
    }
    gOvl11HeldActionProjectionInput[0] = gOvl11HeldActionSpritePosition.x + 0x96;
    gOvl11HeldActionProjectionInput[1] = gOvl11HeldActionSpritePosition.y - 0x64;
    gOvl11HeldActionProjectionInput[2] = gOvl11HeldActionSpritePosition.z - 0x96;
    depth = ProjectPointToScreen(&gOvl11HeldActionProjectionOutput[0], (SVECTOR *)gOvl11HeldActionProjectionInput);
    if (depth != 0) {
        if (mode != 0) {
            gOvl11HeldActionSpriteStep += 1;
        }
        DrawSpriteFrameRects(gActiveRenderContext->field_120 + ((depth >> 2) * 4), (s32) &gSharedSpriteSource, (s32) (u8) gOvl11HeldActionSpriteHeader, 0, (s16) (*(s16 *) ((u8 *) gOvl11HeldActionProjectionOutput + 0)), (s16) (bobPixels + ((*(u16 *) ((u8 *) gOvl11HeldActionProjectionOutput + 4)) + ((u16) gOvl11HeldActionSpriteStep & 3))));
    }
}
