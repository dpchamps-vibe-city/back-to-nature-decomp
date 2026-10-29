/*
 * Primary-object frame service: select live or substituted button masks,
 * dispatch object actions, advance timers/effects, and retain position history.
 * Input remapping, charge/release selection and the ground-contact probe feed
 * the frame service. The input dispatcher consumes that service's substituted
 * button snapshot, not a second poll of the pad.
 * A new dispatched action clears the cumulative footprint power; the two
 * footprint helpers add tier-scaled power and pass it to grid/effect services.
 * The cell request and action-script step share one non-reentrant lifecycle.
 * History A samples on odd ticks; history B and direction tags sample when
 * tick % 8 == 7. Early exits deliberately skip those samples.
 *
 * Public contracts: ovl11_primary_object_update.h (frame entry) and
 * ovl11_primary_object_grid_actions.h (transfer/animation/action entries).
 * Movement, collision and reusable state/animation setters remain external.
 * All original entries retain external linkage.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_update.h"
#include "ovl11_cell_actions.h"
#include "screen_fade.h"
#include "pad_actuator_sequences.h"
#include "master_volume.h"
#include "sound_commands.h"
#include "ovl11_clock_calendar.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_surface_height.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_held_item_replacement.h"
#include "ovl11_primary_object_collision_action.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_grouped_value_mapping.h"
#include "ovl11_position_transition.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_held_action_selection.h"
#include "ovl11_halfword_triplets.h"
#include "mode6_texture_refresh.h"
#include "map_cell_texture.h"
#include "ovl11_paired_item_ids.h"
#include "area_grid_triggers.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_primary_object_grid_actions.h"
#include "ovl11_record_placement.h"
#include "ovl11_moving_items.h"
#include "sprite_source.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_primary_object_action_phase.h"
#include "ovl11_scene_header_key.h"
#include "ovl11_cell_grid_selection.h"
#include "ovl11_primary_object_input_override.h"
#include "ovl11_recorded_input_playback.h"
#include "ovl11_status_gauge_deltas.h"
#include "ovl11_timed_effect_record_fill.h"
#include "ovl11_interaction_hit_action.h"
#include "ovl11_indexed_progress.h"
#include "ovl11_action_status_delta.h"
#include "ovl11_triplet_sprite_release.h"
#include "ovl11_held_item_flags.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "ovl11_item_record_lists.h"
#include "ovl11_cell_geometry.h"
#include "ovl11_interaction_record_actions.h"
#include "ovl11_spawn_effect_consume.h"
#include "ovl11_pending_stage_dispatch.h"
#include "ovl11_save_counters.h"
#include "psyq/memory.h"
#include "surface_collision.h"
#include "ovl11_object_area_sprite_reset.h"
#include "ovl11_object_record_state.h"
#include "ovl11_condition_evaluation.h"
#include "ovl11_action_result_request.h"
#include "ovl11_item_value_counters.h"

void Ovl11ProbePrimaryObjectSurface(Ovl11C498CObject *object);
void Ovl11RemapPrimaryDirectionButtons(void);
s32 Ovl11DispatchObjectActionInput(Ovl11C7010Object *object);

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
            Ovl11SyncHeldItemFlags((Ovl11C7010Object *) obj);
            obj->flags = (s32) (obj->flags & ~0x2000);
        }
        if (GetGameFlag(0xADU) == 1) {
            SetGameFlag(0xADU, 0U);
            RequestMessageWindow(0, 0x724);
        }
        Ovl11ProbePrimaryObjectSurface((Ovl11C498CObject *) obj);
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
        if (Ovl11StepRecordedInputPlayback() == 0) {
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
            gOvl11PrimaryDispatchResult = 0;
            if (!(obj->flags & 0x20000000)) {
                switch ((u32)obj->flags & 0x800010) {
                case 0x800000:
                    Ovl11ApplyPrimaryObjectInputOverride(obj);
                    if (Ovl11DispatchPrimaryObjectAction() != 0) {
                        gOvl11PrimaryFootprintPower = 0;
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
                    gOvl11PrimaryDispatchResult = Ovl11DispatchPrimaryObjectAction();
                    if (gOvl11PrimaryDispatchResult != 0) {
                        gOvl11PrimaryFootprintPower = 0;
                        gOvl11PrimaryNoActionTicks = 0;
                    } else if (gOvl11PrimaryNoActionTicks == 0x10) {
                        gOvl11PrimaryNoActionTicks = 0;
                        gOvl11PrimaryDirectionRemap = 1;
                        if ((obj->direction == 2) || (obj->direction == 0)) {
                            gOvl11PrimaryDirectionRemap = 0;
                        }
                    } else {
                        gOvl11PrimaryNoActionTicks += 1;
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

/* The frame updater's ground-contact step. The offset is applied only to
 * the copied probe position, not committed to the object's coordinates. */
void Ovl11ProbePrimaryObjectSurface(Ovl11C498CObject *object) {
    s32 probePosition[4];
    s16 *offset;

    Ovl11QueryPrimaryObjectCollisionAction((Ovl11CB114Object *)object, object->position, 0x18);
    probePosition[0] = object->position[0];
    probePosition[1] = object->position[1];
    probePosition[2] = object->position[2];
    probePosition[3] = object->position[3];
    offset = gOvl11FacingProbeOffsets[object->direction];
    probePosition[0] += offset[0];
    probePosition[2] += offset[1];
    Ovl11ResolveSurfaceHeight(probePosition, 0);
    gOvl11PrimarySurfaceFlags = *(s32 *)GetSurfaceHitPolygonAddress();
    /* Preserve the sentinel substitution when the polygon's upper half is 0. */
    if (!(gOvl11PrimarySurfaceFlags & 0xFFFF0000)) {
        gOvl11PrimarySurfaceFlags = 0x8000;
    }
}

/* Cell-action implementation. A request advances one footprint index per
 * call, then applies its score on the call after its inclusive limit. */

void Ovl11InstallObjectCellItems(Ovl11C4A2CObject *arg0) {
    Ovl11CE034Position sp18;
    u16 sp28;
    u16 sp2A;
    s16 sp2C;
    s16 sp2E;
    s32 temp_s3;
    u32 var_s1;

    temp_s3 = Ovl11GetCellGridSelection();
    sp18.unk0 = arg0->unk100;
    sp18.unk4 = arg0->unk104;
    sp18.unk8 = arg0->unk108;
    if (temp_s3 != 2) {
        gOvl11AreaCellConverters[temp_s3](&sp18, &sp28, &sp2A);
        var_s1 = 0;
        do {
            if (Ovl11FindCellGridTag(&arg0->unk84, var_s1, arg0->unk38, &sp2C, &sp2E, 0) != 0) {
                sp2C += sp28;
                sp2E += sp2A;
                if (Ovl11IsCellUnusable(sp2C, sp2E, 1, (s16) temp_s3) == 0) {
                    Ovl11TryInstallCellItem((s16) temp_s3, sp2C, sp2E, arg0->unk84);
                }
            }
            var_s1 += 1;
        } while (var_s1 < 9U);
    }
}

void Ovl11BeginCellActionRequest(s16 operation, s16 mode, s16 column,
                               s16 row, s16 filter, s32 limit);

s32 Ovl11BeginCellResetAction(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s32 tier;
    s16 value;
    s16 *entry;
    s32 mode;
    s32 result;

    mode = Ovl11GetCellGridSelection();
    if (mode != 2) {
        pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
        tier = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
        entry = &gOvl11CellResetTierCounts[tier];
        tier -= *(s16 *)((u8 *)arg0 + 0x86);
        value = *entry;
        gOvl11AreaCellConverters[mode](&pos, &col, &row);
        Ovl11BeginCellActionRequest(0, (s16)mode, col, row, tier, value);
        Ovl11ChangePrimaryObjectState(arg0, 0x68, 0, -2);
        result = 1;
    } else {
        Ovl11AddIndexedProgress((Ovl11CD5BCObject *)arg0, 0x28);
        result = 0;
    }
    return result;
}

s32 Ovl11BeginCellPromotionAction(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s16 temp_a0;
    s32 temp_s1;
    s32 temp_v1;
    s32 var_s0;
    s32 temp_v0;

    temp_v0 = Ovl11GetCellGridSelection();
    if (temp_v0 == 2) {
        Ovl11AddIndexedProgress((Ovl11CD5BCObject *)arg0, 0x28);
        return 0;
    }
    pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
    temp_s1 = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
    var_s0 = gOvl11CellPromotionTierCounts[temp_s1];
    temp_a0 = *(s16 *)((u8 *)arg0 + 0x88);
    temp_s1 -= *(s16 *)((u8 *)arg0 + 0x86);
    temp_v1 = temp_a0 - var_s0;
    if (temp_a0 < var_s0) {
        var_s0 = temp_a0 - 1;
    }
    *(s16 *)((u8 *)arg0 + 0x88) = temp_v1;
    if ((s16)temp_v1 < 0) {
        *(s16 *)((u8 *)arg0 + 0x88) = 0;
    }
    gOvl11AreaCellConverters[temp_v0](&pos, &col, &row);
    Ovl11BeginCellActionRequest(1, (s16)temp_v0, col, row, temp_s1, var_s0);
    Ovl11ChangePrimaryObjectState(arg0, 0x68, 0, -2);
    return 1;
}

s32 Ovl11BeginCellEffectAction(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s32 tier;
    s16 *entry;
    s16 value;
    s32 mode;
    s32 result;

    mode = Ovl11GetCellGridSelection();
    if (mode == 2) {
        if (gOvl11InteractionHitFlags & 0x8000) {
            if (Ovl11ConsumeSpawnEffect(gOvl11InteractionHitRecordView) != 0) {
                Ovl11AddIndexedProgress((Ovl11CD5BCObject *)arg0, 0x96);
            }
        } else {
            Ovl11AddIndexedProgress((Ovl11CD5BCObject *)arg0, 0x28);
        }
        result = 0;
    } else {
        pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
        tier = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
        entry = &gOvl11CellEffectTierCounts[tier];
        tier -= *(s16 *)((u8 *)arg0 + 0x86);
        value = *entry;
        gOvl11AreaCellConverters[mode](&pos, &col, &row);
        Ovl11BeginCellActionRequest(2, (s16)mode, col, row, tier, value);
        Ovl11ChangePrimaryObjectState(arg0, 0x68, 0, -2);
        result = 1;
    }
    return result;
}

void Ovl11BeginSelectedRecordTransfer(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Recon_ovl_11_func_800C7270_A0View *var_s0;

    if (arg1 == 0) {
        var_s0 = &arg0->unk84;
    } else {
        var_s0 = (Recon_ovl_11_func_800C7270_A0View *) ((u8 *) arg0 + ((arg2 * 6) + 0x90));
    }
    if ((*(s16 *) ((u8 *) var_s0 + 0)) != 0) {
        arg0->unk6C &= 0xE7FFFFFF;
        Ovl11ChangeObjectStateDirect(arg0, 0x15, 0x47, arg3);
        Ovl11CopyHalfwordTriplet((u16 *) &arg0->unkF0, (u16 *) var_s0);
        arg0->unk6C |= 0x8000;
        PlaySoundEffect(7);
    }
}

void Ovl11SelectHeldPairChangeAnimation(Ovl11C7010Object *arg0) {
    if ((u32)(*(u16 *)((u8 *)arg0 + 0x8A) - 0x6D) < 2U) {
        PlaySoundEffect(8);
        arg0->unk6C &= 0xEFFF7FFF;
        Ovl11ChangePrimaryObjectState(arg0, 0x34, arg0->unk38 + 0x1D, -2);
    }
}

/* Replaces the one pending request. Limit is inclusive; the step after the
 * final footprint index applies the score (40 initially, 150 after success).
 * Mode selects a grid; operation 0/1/2 resets/promotes/applies an effect. */
void Ovl11BeginCellActionRequest(s16 operation, s16 mode, s16 column,
                               s16 row, s16 filter, s32 limit) {
    gOvl11CellActionProgress = 0;
    gOvl11CellActionScore = 0x28;
    gOvl11CellActionMode = mode;
    gOvl11CellActionOperation = operation;
    gOvl11CellActionColumn = column;
    gOvl11CellActionRow = row;
    gOvl11CellActionLimit = limit;
    gOvl11CellActionFilter = filter;
}

s32 Ovl11StepCellActionRequest(Ovl11CD5BCObject *arg0) {
    s16 sp18;
    s16 sp1A;

    if (gOvl11CellActionProgress <= gOvl11CellActionLimit) {
        if (Ovl11FindCellGridTag(&arg0->value, (u32) gOvl11CellActionProgress, *(s16 *) ((u8 *) arg0 + 0x38), &sp18, &sp1A, gOvl11CellActionFilter) != 0) {
            sp18 += gOvl11CellActionColumn;
            sp1A += gOvl11CellActionRow;
            if (Ovl11IsCellUnusable(sp18, sp1A, 0, gOvl11CellActionMode) != 1) {
                switch (gOvl11CellActionOperation) {
                case 0:
                    if (Ovl11TryResetCellAndSpawnEffect(gOvl11CellActionMode, sp18, sp1A) == 1) {
                        gOvl11CellActionScore = 0x96;
                    } else {
                        gOvl11CellActionProgress = gOvl11CellActionLimit + 1;
                    }
                    break;
                case 1:
                    if (Ovl11TryPromoteCellPendingKind(gOvl11CellActionMode, sp18, sp1A, 0) == 1) {
                        gOvl11CellActionScore = 0x96;
                    }
                    break;
                case 2:
                    if (Ovl11TryApplyCellActionEffect(gOvl11CellActionMode, sp18, sp1A) == 1) {
                        gOvl11CellActionScore = 0x96;
                    }
                    break;
                }
            }
        }
    } else {
        Ovl11AddIndexedProgress(arg0, gOvl11CellActionScore);
        return 1;
    }
    gOvl11CellActionProgress += 1;
    return 0;
}

/* Power accumulates only for usable grid cells or eligible effect hits.
 * Its u16 wrap and the callee's signed-halfword view are intentional. */
void Ovl11ApplyScoredCellFootprint(Ovl11CD5BCObject *object) {
    Ovl11CE034Position position;
    u16 baseColumn;
    u16 baseRow;
    s16 column;
    s16 row;
    s16 tier;
    s32 layer;
    s32 found;
    s32 score;

    score = 0x28;
    layer = Ovl11GetCellGridSelection();
    position.unk0 = object->x;
    position.unk4 = object->y;
    position.unk8 = object->z;
    tier = object->power / 20;
    if (layer != 2) {
        gOvl11AreaCellConverters[layer](&position, &baseColumn, &baseRow);
        found = Ovl11FindCellGridTag(&object->value, 0U, object->direction, &column, &row, 0);
        if (found == 1) {
            column += baseColumn;
            row += baseRow;
            if (Ovl11IsCellUnusable(column, row, 0, (s16) layer) == 0) {
                gOvl11PrimaryFootprintPower += found << tier;
                if (Ovl11TryClearCellFootprintScored((s16) layer, column, row, gOvl11PrimaryFootprintPower, object->amount) == found) {
                    score = 0x96;
                }
            }
        }
    } else if (gOvl11InteractionHitFlags & 0x8000) {
        gOvl11PrimaryFootprintPower += 1 << tier;
        if (Ovl11ConsumeTieredSpawnEffect(gOvl11InteractionHitRecordView, gOvl11PrimaryFootprintPower, object->amount) == 1) {
            score = 0x96;
        }
    }
    Ovl11AddIndexedProgress(object, score);
}

void Ovl11ApplyCellFootprint(Ovl11CD5BCObject *object) {
    Ovl11CE034Position position;
    u16 baseColumn;
    u16 baseRow;
    s16 column;
    s16 row;
    s16 tier;
    s32 layer;
    s32 found;
    s32 score;

    score = 0x28;
    layer = Ovl11GetCellGridSelection();
    position.unk0 = object->x;
    position.unk4 = object->y;
    position.unk8 = object->z;
    tier = object->power / 20;
    if (layer != 2) {
        gOvl11AreaCellConverters[layer](&position, &baseColumn, &baseRow);
        found = Ovl11FindCellGridTag(&object->value, 0U, object->direction, &column, &row, 0);
        if (found == 1) {
            column += baseColumn;
            row += baseRow;
            if (Ovl11IsCellUnusable(column, row, 0, (s16) layer) == 0) {
                gOvl11PrimaryFootprintPower += found << tier;
                if (Ovl11TryClearCellFootprint((s16) layer, column, row, gOvl11PrimaryFootprintPower, object->amount) == found) {
                    score = 0x96;
                }
            }
        }
    } else if (gOvl11InteractionHitFlags & 0x8000) {
        gOvl11PrimaryFootprintPower += 1 << tier;
        if (Ovl11ConsumeSpawnEffectWithBudgets(gOvl11InteractionHitRecordView, gOvl11PrimaryFootprintPower, object->amount) == 1) {
            score = 0x96;
        }
    }
    Ovl11AddIndexedProgress(object, score);
}

void Ovl11PlaceObjectCellTexture(Ovl11CE034Object *arg0) {
    Ovl11CE034Position sp10;
    s16 sp20;
    s16 sp22;
    s32 temp_s0;
    Ovl11D124Entry *var_s0;

    temp_s0 = Ovl11GetCellGridSelection();
    Ovl11ComputeGridOffsetPosition(arg0, &sp10, 1);
    if (temp_s0 != 2) {
        gOvl11AreaCellConverters[temp_s0](&sp10, &sp20, &sp22);
        if (temp_s0 != 0) {
            var_s0 = &gAlternateAreaGrid[sp22][sp20];
            Ovl11SetMode6CellTexture(sp20, sp22, 0);
        } else {
            var_s0 = &gMainAreaGrid[sp22][sp20];
            Ovl11SetMapCellTexture(sp20, sp22, 0);
        }
        var_s0->unk0 = *(u16 *) ((u8 *) arg0 + 0x8A);
        if ((u16)var_s0->unk0 == 0x13B) {
            var_s0->unk0 = 0x3AU;
        }
        if ((u16)var_s0->unk0 == 0x13C) {
            var_s0->unk0 = 0x3EU;
        }
        if ((u16)var_s0->unk0 == 0x13D) {
            var_s0->unk0 = 0x3FU;
        }
        var_s0->unk4 = 0;
        var_s0->unk5 = 0;
        var_s0->unk6 = 0;
        var_s0->unk2 = 0x167;
    }
}

void Ovl11MoveToFreeDirection(Ovl11C498CObject *arg0) {
    VECTOR position;
    s32 result;
    u16 direction;
    u32 i;

    direction = arg0->direction;
    for (i = 0; i < 4U; i++) {
        arg0->direction = i;
        position = *(VECTOR *)arg0->position;
        Ovl11ComputeGridOffsetPosition((Ovl11CE034Object *)arg0,
                           (Ovl11CE034Position *)&position, 2);
        result = Ovl11ResolveSurfaceHeight((s32 *)&position, 0xC080);
        if ((result | Ovl11QueryPrimaryObjectCollisionAction((Ovl11CB114Object *)arg0,
                                        (s32 *)&position, 0x18)) == 0) {
            *(VECTOR *)arg0->position = position;
            return;
        }
    }
    arg0->direction = direction;
}

void Ovl11FinalizePrimaryItemAction(Ovl11C6770Object *object);
s32 Ovl11CommitPendingItemRecord(Ovl11C7010Object *object);

/* Action scripts use six-byte {state, animation, attributes} entries.
 * 0x270F terminates the script. Preserve the shared finish/advance tails:
 * request reset, gauge changes and script advancement have distinct gates. */
s32 Ovl11StepPrimaryObjectAction(Ovl11C580CObject *arg0) {
    s16 column;
    VECTOR position;
    Ovl11CC7CCRequest request1;
    Ovl11CC7CCRequest request2;
    Ovl11CC7CCRequest request3;
    Ovl11TripletTailView *temp_s0;
    Ovl11TripletTailView *temp_s0_2;
    Ovl11TripletTailView *temp_s0_3;
    s16 temp_s0_4;
    s16 temp_s0_5;
    s16 temp_v1_4;
    s16 var_a1_3;
    s16 var_a2_2;
    s32 var_s0;
    s32 applyCost;
    s32 waitForAction;
    s32 var_v0;
    u16 *temp_s0_6;
    u16 *var_a0_3;
    u16 temp_v0;
    s32 temp_v0_2;
    s32 status;
    u8 temp_v1;
    void *temp_a0;
    void *temp_v0_3;
    void *temp_v1_3;

    waitForAction = 0;
    if (arg0->unk6C & 0x1000) {
        temp_v0 = arg0->state;
        switch (temp_v0) {
        case 0x3D:
        case 0x3E:
        case 0x3F:
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
            arg0->unk6C &= ~0x4000;
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
reset_done:
            return 0;
        case 0x52:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0x15);
            arg0->timer = 0x64;
            goto negative_action;
        case 0x55:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0x11);
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 8);
            temp_s0 = (Ovl11TripletTailView *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                Ovl11InvokeItemAction(temp_s0);
            }
            Ovl11StepTripletSpriteRelease(temp_s0);
            arg0->unk6C &= 0xFFFF7FFB;
            Ovl11ResetPrimaryObjectRequest();
            Ovl11ApplyStatusDelta(0, Ovl11AdjustActionStatusDelta(0), 0, 0);
            return 0;
        case 0x50:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            Ovl11SetPendingStageMode(0);
            if (arg0->attributes & 8) {
                Ovl11InvokeItemAction((void *) ((u8 *) arg0 + 0x84));
            }
            Ovl11ResetPrimaryObjectRequest();
            Ovl11ApplyStatusDelta(0, Ovl11AdjustActionStatusDelta(0), 0, 0);
            return 0;
        case 0x51:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0x11);
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 4);
            temp_s0_2 = (Ovl11TripletTailView *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                Ovl11InvokeItemAction(temp_s0_2);
            }
            Ovl11StepTripletSpriteRelease(temp_s0_2);
            arg0->unk6C &= 0xFFFF7FFB;
            Ovl11ResetPrimaryObjectRequest();
            Ovl11ApplyStatusDelta(0, Ovl11AdjustActionStatusDelta(0), 0, 0);
            return 0;
        case 0x4E:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 6);
            if (arg0->attributes & 8) {
                Ovl11InvokeItemAction((void *) ((u8 *) arg0 + 0x84));
            }
            arg0->unk6C &= ~4;
            Ovl11ApplyStatusDelta(0, Ovl11AdjustActionStatusDelta(0), 0, 0);
            return 0;
        case 0x4F:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 2);
            if (arg0->attributes & 8) {
                Ovl11InvokeItemAction((void *) ((u8 *) arg0 + 0x84));
            }
            Ovl11ResetPrimaryObjectRequest();
            Ovl11ApplyStatusDelta(0, Ovl11AdjustActionStatusDelta(0), 0, 0);
            return 0;
        case 0x4D:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 5);
            if (arg0->attributes & 8) {
                Ovl11InvokeItemAction((void *) ((u8 *) arg0 + 0x84));
            }
            arg0->unk6C &= ~4;
            Ovl11ApplyStatusDelta(0, Ovl11AdjustActionStatusDelta(0), 0, 0);
            return 0;

        case 0x53:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0xE);
            Ovl11InstallObjectCellItems((Ovl11C4A2CObject *) arg0);
            status = arg0->unk6C;
            status &= 0xEFFFFFFF;
            arg0->unk6C = status;
            Ovl11StepTripletSpriteRelease((Ovl11TripletTailView *) ((u8 *) arg0 + 0x84));
            Ovl11ResetPrimaryObjectRequest();
negative_action:
            Ovl11ApplyStatusDelta(-1, Ovl11AdjustActionStatusDelta(0), 0, 0);
            break;
        case 0x54:
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x2D, 1, -2);
            return 0;
        case 0x4C:
            arg0->state = 0x31U;
            Ovl11FinalizePrimaryItemAction((Ovl11C6770Object *) arg0);
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            arg0->unk6C &= ~0x4000;
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x3B:
            PlaySoundEffect(0x10);
            arg0->timer114 = 0;
            arg0->direction = 0U;
            arg0->unk6C |= 1;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x56U, 0, -2);
state_done:
            return 0;
        case 0x56:
            arg0->unk6C |= 0x1800;
            arg0->attributes |= 0x100;
            arg0->unk6C &= ~0x40000000;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x3CU, 0, gOvl11ClockMinutePeriodTwice * 0xA);
            arg0->unk6C |= 0x40000000;
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x58, 0, gOvl11ClockMinutePeriodTwice * 0xA);
            RequestPadActuatorSequence(0, 3);
            return 0;
        case 0x58:
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x57, 1, gOvl11ClockMinutePeriodTwice * 0xA);
            return 0;
        case 0x45:
            arg0->unk6C = (arg0->unk6C | 0x10) & 0xDFFFFFFF;
            goto state_done;
        case 0x49:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            temp_s0_3 = (Ovl11TripletTailView *) ((u8 *) arg0 + 0x8A);
            arg0->unk6C &= 0xEFFF3FFF;
            Ovl11InvokeItemAction(temp_s0_3);
            Ovl11StepTripletSpriteRelease(temp_s0_3);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x46:
            if ((gOvl11PrimarySurfaceFlags & 0x80) && (Ovl11CommitPendingItemCounts() != 0)) {
                PlaySoundEffect(0x13);
                SetGameFlag(7U, 1U);
            }
            arg0->unk6C &= ~0x4000;
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x4B:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            Ovl11ResolveActionResultOrRotate(0x2F, 0U, NULL, 1);
            arg0->direction = (u16) ((arg0->direction + 2) & 3);
            Ovl11BeginPrimaryObjectInputOverride(0x14);
            return 0;
        case 0x4A:
            temp_v1 = arg0->phase;
            if (temp_v1 == 3) {
                ((struct Ovl11C580CWork *)&D_8007AFF0)->mode = 0xC;
                BeginScreenFadeIncrease(0x1E, 0, 2);
                BeginMasterVolumeFadeOut(0x1E);
                arg0->unk6C |= 0x400;
                SetGameFlag(0x47U, 1U);
                return 0;
            }
            if (temp_v1 == 1) {
                SetGameFlag(0xAU, 1U);
            }
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x4A, arg0->phase + 1, *((s32 *) ((u8 *) &gOvl11ActionPhaseDelays + (arg0->phase * 4))));
            return 0;

        default:
            Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            goto reset_done;
        }
        return 0;
    } else {
        status = arg0->state;
        applyCost = 0;
        switch (status) {
        case 0x6F:
            PlaySoundEffect(9);
            ((struct Ovl11C580CState *)D_8006C838)->record->flags |= 0x100;
            gOvl11RecordSpriteYOffsetIndex = 3;
            if (Ovl11StageItemValue(arg0->item) != 0) {
                RequestMessageWindow(3, 0x383);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            Ovl11ReleaseHeldItem((Ovl11C7010Object *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x3A:
            if (arg0->unk6C & 0x80) {
                Ovl11RefreshRecordById(*(s16 *)Ovl11SavedInteractionObject, (Ovl25SetupStatePair *) ((u8 *) arg0 + 0xFC), (VECTOR *) ((u8 *) arg0 + 0x100));
            } else {
                Ovl11FillTimedEffectRecord((u8 *)&gStageMode2Record, (Ovl11EffectRecordPair *)((u8 *)arg0 + 0xFC), (Vec3 *)((u8 *)arg0 + 0x100));
            }
            arg0->unk6C &= ~0x180;
            Ovl11InitSharedSpriteSlots((u8 *) arg0);
            Ovl11MoveToFreeDirection((Ovl11C498CObject *) arg0);
            return 1;
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
        case 0x61:
        case 0x62:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6E:
            temp_s0_4 = arg0->item;
            if (arg0->state == 0x61) {
                PlaySoundEffect(0x13);
            }
            if (arg0->state == 0x62) {
                Ovl11ComputeGridOffsetPosition((Ovl11CE034Object *)arg0, (Ovl11CE034Position *)&position, 4);
                Ovl11SetSpawnRecord(0x64, position, arg0->collisionId, 0);
                PlaySoundEffect(7);
            }
            if (arg0->state == 0x6E) {
                PlaySoundEffect(0x10);
                ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) arg0->item;
                if (arg0->item == 0xA4) {
                    ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) ((struct Ovl11C580CState *)D_8006C838)->fallback;
                }
                Ovl11SetConditionState(4);
            }
            arg0->unk6C &= 0xEFFF3FFF;
            Ovl11RemoveSlotSprite(arg0->item);
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0x8A));
            if (arg0->state == 0x6A) {
                SetGameFlag(0x45U, 1U);
                temp_v0_2 = ((struct Ovl11C580CState *)D_8006C838)->count + 1;
                ((struct Ovl11C580CState *)D_8006C838)->count = temp_v0_2;
                if ((s16) temp_v0_2 < 2) {
                    Ovl11ApplyStatusDelta(0, -5, 0, 0);
                }
            }
            if (arg0->state == 0x59) {
                request1.id = 0x8E;
                if (temp_s0_4 == 0x8C) {
                    request1.id = 0x8F;
                }
                if (temp_s0_4 == 0x8D) {
                    request1.id = 0x90;
                }
                if (temp_s0_4 == 0xE3) {
                    request1.id = 0xE4;
                }
                request1.count = 1;
                PlaySoundEffect(7);
                Ovl11InstallPrimaryObjectRequest(&request1.id);
                return 0;
            } else if (arg0->state == 0x5A) {
                request2.id = 0x94;
                if (temp_s0_4 == 0x92) {
                    request2.id = 0x95;
                }
                if (temp_s0_4 == 0x93) {
                    request2.id = 0x96;
                }
                if (temp_s0_4 == 0xE7) {
                    request2.id = 0xE6;
                }
                request2.count = 1;
                PlaySoundEffect(7);
                Ovl11InstallPrimaryObjectRequest(&request2.id);
                return 0;
            } else if (arg0->state == 0x5B) {
                request3.id = 0x88;
                if (temp_s0_4 == 0x123) {
                    request3.id = 0x89;
                }
                if (temp_s0_4 == 0x124) {
                    request3.id = 0x8A;
                }
                if (temp_s0_4 == 0xE8) {
                    request3.id = 0xE5;
                }
                if (temp_s0_4 == 0x125) {
                    request3.id = 0x8A;
                }
                request3.count = 1;
                PlaySoundEffect(7);
                Ovl11InstallPrimaryObjectRequest(&request3.id);
                return 0;
            } else if (arg0->state == 0x5C) {
                Ovl11AddSaveCounter20(1U, 0U);
            }
            Ovl11ResetPrimaryObjectRequest();
            break;
        case 0x33:
            Ovl11AdjustSpendableBalance((s32) (*(s16 *) ((u8 *) gItemDataTable + 0x1002)), 0);
            Ovl11RemoveSlotSprite(arg0->item);
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0x8A));
            arg0->unk6C &= 0xEFFFFFFF;
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x34:
            arg0->kind = 1;
            if (arg0->item == 0x6E) {
                arg0->kind = 2;
            }
            arg0->slot = (u16) *((u16 *) ((u8 *) &gOvl11ItemKindSlots + (arg0->kind * 2)));
            Ovl11RemoveSlotSprite(arg0->item);
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0x8A));
            arg0->unk6C &= 0xEFFFFFFF;
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x45:
            arg0->unk6C |= 0x10;
        default:
finish_action:
            goto advance_action;
        case 0x32:
            if ((Ovl11HandleInteractionRequest80((Ovl11C8FA8Object *) arg0) == 0) && (gOvl11InteractionHitFlags & 0x400)) {
                Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) gOvl11InteractionHitRecordView, 0xD);
                Ovl11ResetPrimaryObjectRequest();
            }
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0xF0));
            arg0->unk6C &= 0xFFFF7FFF;
            goto finish_action;
        case 0xA:
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x15:
            if (arg0->state == 0x15) {
                arg0->unk6C &= 0xFFFF7FFF;
            }
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0xF0));
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x2F:
            PlaySoundEffect(9);
            Ovl11SpawnMovingItemRecord((Vec3 *) ((u8 *) arg0 + 0x100), (u16 *) ((u8 *) arg0 + 0x8A), (s16) arg0->direction);
            arg0->unk6C |= 0x40;
            /* fallthrough */
        case 0x9:
            if (arg0->state == 9) {
                PlaySoundEffect(9);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            if (arg0->state == 9) {
                Ovl11ReleaseHeldItem((Ovl11C7010Object *) arg0);
            }
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x38:
            PlaySoundEffect(9);
            Ovl11PlaceObjectCellTexture((Ovl11CE034Object *) arg0);
            arg0->unk6C &= 0xE7FF3FFF;
            Ovl11ReleaseHeldItem((Ovl11C7010Object *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0xC:
            if (Ovl11IsLowerPairedItemId(arg0->item) == 0) {
                temp_s0_6 = (u16 *) ((u8 *) arg0 + 0x8A);
                Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + (((s16) arg0->slot * 6) + 0xBA)), temp_s0_6);
                Ovl11CompactSecondaryItemList();
                var_a0_3 = temp_s0_6;
            } else {
                Ovl11RemoveSlotSprite(arg0->item);
                var_a0_3 = (u16 *) ((u8 *) arg0 + 0x8A);
            }
            Ovl11ClearHalfwordTriplet((s16 *) var_a0_3);
            arg0->unk6C &= 0xEFFFFFFF;
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0xD:
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x6:
        case 0xE:
        case 0x30:
            Ovl11CommitPendingItemRecord((Ovl11C7010Object *) arg0);
            if (Ovl11IsUpperPairedItemId(arg0->item) == 1) {
                Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x2FU, arg0->direction + 0x17, -2);
            }
            if (Ovl11IsLowerPairedItemId(arg0->item) != 1) {
                goto tool_done;
            }
            arg0->unk6C |= 0x10000000;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) &gPrimaryObjectBase, 0x67U, 0x1B, 0x1E);
tool_done:
            return 0;
        case 0x67:
            arg0->unk6C |= 0x10000000;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) &gPrimaryObjectBase, 0xCU, (*(u16 *) ((u8 *) &gPrimaryObjectBase + 0x38)) + 0x1D, -2);
            arg0->unk6C &= 0xFFFF7FFF;
            return 0;
        case 0x2C:
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x2D, 0xB1, -2);
            return 0;
        case 0x3:
        case 0x31:
            arg0->unk6C &= ~0x4000;
            Ovl11FinalizePrimaryItemAction((Ovl11C6770Object *) arg0);
            Ovl11SyncHeldItemFlags((Ovl11C7010Object *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x28:
            Ovl11StepTripletSpriteRelease((Ovl11TripletTailView *) ((u8 *) arg0 + 0x8A));
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x17:
            if (Ovl11BeginCellEffectAction((Ovl11C7010Object *) arg0) == 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            goto advance_action;
        case 0x19:
            temp_v1_4 = *((struct Ovl11C580CWork *)&D_8007AFF0)->header;
            if ((temp_v1_4 == 0x37) || (temp_v1_4 == 0x43)) {
                var_s0 = 0x28;
                waitForAction = Ovl11ClassifyAreaGridCell((Ovl11CE034Object *) arg0);
                if (waitForAction != 0) {
                    var_s0 = 0x96;
                }
                Ovl11AddIndexedProgress((Ovl11CD5BCObject *)arg0, var_s0);
                applyCost = 1;
            } else {
                if (Ovl11BeginCellResetAction((Ovl11C7010Object *) arg0) == 1) {
                    applyCost = 0;
                    waitForAction = 1;
                } else {
                    applyCost = 1;
                }
            }
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            RequestPadActuatorSequence(0, 2);
            goto advance_action;
        case 0x1F:
            if (Ovl11BeginCellPromotionAction((Ovl11C7010Object *) arg0) == 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            StopSoundEffectVoice((s32) gOvl11ActiveItemActionSound);
            gOvl11ActiveItemActionSound = -1;
            goto finish_action;
        case 0x68:
            if (Ovl11StepCellActionRequest((Ovl11CD5BCObject *) arg0) != 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            goto finish_action;
        case 0x1D:
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            Ovl11ApplyCellFootprint((Ovl11CD5BCObject *) arg0);
            RequestPadActuatorSequence(0, 2);
            applyCost = 1;
            goto finish_action;
        case 0x1B:
            Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            Ovl11ApplyScoredCellFootprint((Ovl11CD5BCObject *) arg0);
            RequestPadActuatorSequence(0, 2);
            applyCost = 1;
            goto finish_action;
        case 0x39:
            return 1;
        }

advance_action:
        if (applyCost == 1) {
            if (waitForAction == 0) {
                Ovl11ResetPrimaryObjectRequest();
            }
            column = (s16)arg0->power / 20;
            temp_s0_5 = gOvl11ItemActionTierValues[(s16)((s16)(arg0->value - 1) / 5)][column];
            Ovl11ApplyStatusDelta((s8) temp_s0_5, Ovl11AdjustActionStatusDelta(0), 0, 0);
        }
        var_v0 = 0;
        if (waitForAction == 1) {
            return var_v0;
        }
        {
            temp_v0_3 = *(void **) ((u8 *) arg0 + 0x4C);
            temp_a0 = (void *) ((u8 *) temp_v0_3 + 6);
            if (temp_v0_3 != NULL) {
                (*(void **) ((u8 *) arg0 + 0x4C)) = temp_a0;
                if ((*(s16 *)((u8 *)temp_v0_3 + 6)) == 0x270F) {
                    Ovl11ResetPrimaryObjectRequest();
                    (*(void **) ((u8 *) arg0 + 0x4C)) = NULL;
                    goto script_finished;
                }
                if ((*(s16 *)((u8 *)temp_a0 + 4)) != 0) {
                    arg0->unk6C |= 0x1800;
                }
                temp_v1_3 = *(void **) ((u8 *) arg0 + 0x4C);
                arg0->attributes = (u16) (arg0->attributes & 0xFFEF);
                var_a1_3 = (*(s16 *)temp_v1_3);
                var_a2_2 = (*(s16 *)((u8 *)temp_v1_3 + 2));
                Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, var_a1_3, (s32) var_a2_2, -2);
                return 0;
            }
script_finished:
            var_v0 = 1;
            return var_v0;
        }
    }
}

void Ovl11FinalizePrimaryItemAction(Ovl11C6770Object *arg0) {
    Ovl11ItemState *temp_s1;

    temp_s1 = &arg0->item;
    Ovl11InvokeItemAction(temp_s1);
    if (arg0->item.id == 0x126) {
        arg0->unk6C |= 0x200;
    }
    Ovl11StepTripletSpriteRelease((Ovl11TripletTailView *)temp_s1);
    if (arg0->unk3C == 0x31) {
        arg0->item = gOvl11SavedHeldItem;
        Ovl11InstallSlotSprite(arg0->item.id);
        arg0->unk6C &= ~0x4000;
        arg0->unk6C |= 0x8000;
        if (arg0->item.id == 0) {
            arg0->unk6C &= ~0x10000000;
            arg0->unk6C &= ~0x8000;
        }
    } else {
        arg0->unk6C &= ~0x10000000;
        arg0->unk6C &= ~0x8000;
    }
}

s32 Ovl11BeginFlaggedItemFollowup(Ovl11C7010Object *arg0);

s32 Ovl11CommitPendingItemRecord(Ovl11C7010Object *arg0) {
    u16 *item;
    s32 index;

    if (arg0->unk3C == 0xE) {
        arg0->unk6C |= 0x08000000;
    } else {
        arg0->unk6C |= 0x10000000;
    }
    arg0->unk6C &= 0xFFDFFFFF;
    if (arg0->unk3C == 0x30) {
        Ovl11ChangePrimaryObjectState(arg0, 0x31, 5, -2);
        PlaySoundEffect(0x4A);
        arg0->unk6C |= 0x8000;
        return 1;
    }
    index = Ovl11ClassifyItemFlags(*(s16 *) ((u8 *) arg0 + 0xF0));
    if (index == 2) {
        index = 1;
    }
    item = (u16 *) ((u8 *) arg0 + 0xF0);
    Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + (index * 6 + 0x84)), item);
    Ovl11ClearHalfwordTriplet((s16 *) item);
    arg0->unk6C |= 0x8000;
    if (arg0->itemIndex == 0x66) {
        PlaySoundEffect(8);
        arg0->unk6C &= 0xEFFF7FFF;
        Ovl11ChangePrimaryObjectState(arg0, 0x33, arg0->unk38 + 0x1D, -2);
        return 1;
    }
    if (Ovl11BeginFlaggedItemFollowup(arg0) == 0) {
        Ovl11ResetPrimaryObjectRequest();
        return 0;
    }
    return 1;
}

s32 Ovl11BeginFlaggedItemFollowup(Ovl11C7010Object *arg0) {
    if ((gItemDataTable[arg0->itemIndex].u0.flags & 0x1200) == 0x1200) {
        Ovl11ChangePrimaryObjectState(arg0, 3, 5, -2);
        PlaySoundEffect(0x4A);
        return 1;
    }
    return 0;
}

/* The tier consumes the dispatcher's +18 charge count; the saved voice is
 * cancelled by the frame/action services, not by this release operation. */
void Ovl11ReleaseChargedItemAction(Ovl11C7010Object *object) {
    s16 chargeTier;
    s32 soundBase;
    s32 soundOffset;

    chargeTier = (s16) (*(u16 *) ((u8 *) object + 0x18)) / 20;
    soundOffset = ((object->unk3C - 0x16) / 2) * 5;
    soundBase = chargeTier + 0x1D;
    gOvl11ActiveItemActionSound = PlaySoundEffect((s16) (soundOffset + soundBase));
    Ovl11ChangePrimaryObjectState(object, object->unk3C + 1, Ovl11MapGroupedValueCode((s16) ((u16) object->unk84 - ((*(u16 *) ((u8 *) object + 0x86)) - chargeTier))) + (object->unk38 * 5), -2);
    object->sprite.field_5 = chargeTier + 1;
}

/* Process the frame's held/pressed snapshot for the current action. Charge
 * states saturate +18 at 20*the +86 word, then count saturated updates; at
 * 61 they request state 0x3E and clear that count. Release selects the next
 * animation and saves its sound voice. Units are update calls, not time.
 * Return 1 only for the handled state-0x2D exit; zero can still have effects.
 * Object flag 0x40 suppresses processing. */
s32 Ovl11DispatchObjectActionInput(Ovl11C7010Object *object) {
    s32 transitionResult;
    u16 action;
    u8 animation;
    u16 held;
    u16 pressed;

    if (!(object->unk6C & 0x40)) {
        held = *(u16 *)&gOvl11PrimaryHeldButtons;
        pressed = *(u16 *)&gOvl11PrimaryPressedButtons;
        action = object->unk3C;
        switch (action) {
        case 0x4F:
            if (((*(s32 *) ((u8 *) object + 0x264)) & ~0xFF) == 0x400) {
                PlaySoundEffect(0xC);
                return 0;
            }
            return 0;
        case 0x58:
            if (pressed & 0x40) {
                object->unk6C = (object->unk6C | 0x1800) & 0xBFFFFFFF;
                Ovl11ChangePrimaryObjectState(object, 0x4BU, 0, 0x2D);
                object->unk38 = 0;
                SelectSpriteAnimation((ObjectState *) &object->sprite, 4);
                (*(s32 *) ((u8 *) object + 0x114)) = -0xFA;
                object->sprite.field_5 = (u8) (*(u16 *) ((u8 *) object + 0x38));
                object->unk6C = (object->unk6C | 0x40000000) & ~1;
                (*(u16 *) ((u8 *) object + 0x36)) = (u16) ((*(u16 *) ((u8 *) object + 0x36)) & 0xFEFF);
                PlaySoundEffect(6);
            }
            goto update_status;
        case 0x57:
            if (pressed & 0x40) {
                Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) object, 0x58, 0, gOvl11ClockMinutePeriodTwice * 0xA);
            }
update_status:
            if (D_8006C844 & 0x100000) {
                Ovl11ApplyStatusDelta(2, -1, 0, 0);
            }
            return 0;
        default:
unhandled:
            return 0;
        case 0x3:
        case 0x31:
            if ((u8) object->sprite.field_5 >= 2U) {
                object->unk6C &= 0xFFFF7FFF;
            }
            return 0;
        case 0x2D:
            if (!(held & 0xF9FF)) {
                if (object->unk6C & 0x800000) {
                    if (!(D_80070C88 & 0x1000)) {
                        goto finish_state_2d;
                    }
                    return 0;
                }
                goto unhandled;
            }
finish_state_2d:
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) object, 0x2E, 2, -2);
            object->unk6C &= 0xFFBFFFFF;
            return 1;
        case 0x16:
        case 0x18:
        case 0x1A:
        case 0x1C:
        case 0x1E:
            if (held & 0x80) {
                if ((*(s16 *) ((u8 *) object + 0x18)) < ((*(s16 *) ((u8 *) object + 0x86)) * 0x14)) {
                    (*(s16 *) ((u8 *) object + 0x18)) = (s16) ((u16) (*(s16 *) ((u8 *) object + 0x18)) + 1);
                } else {
                    gOvl11ItemActionChargeFrames += 1;
                }
                object->sprite.field_5 = (*(s16 *) ((u8 *) object + 0x18)) / 20;
                if ((u32) gOvl11ItemActionChargeFrames >= 0x3DU) {
                    object->unk6C |= 0x1800;
                    Ovl11ChangePrimaryObjectState(object, 0x3EU, 0, -2);
                    gOvl11ItemActionChargeFrames = 0;
                    return 0;
                }
                return 0;
            }
            Ovl11ReleaseChargedItemAction(object);
            return 0;
        case 0x4A:
            animation = object->sprite.field_4;
            if (animation == 1) {
                transitionResult = Ovl11StepPositionTransition((Ovl11PositionTransitionObject *) object);
                if (transitionResult == animation) {
                    transitionResult = -2;
                } else {
                    transitionResult = 0x84;
                }
                (*(s16 *) ((u8 *) object + 0x1C)) = transitionResult;
            }
            return 0;
        }
    } else {
        return 0;
    }
}
