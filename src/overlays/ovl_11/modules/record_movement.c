/*
 * record_movement — reconstructed translation unit.
 *
 * Record movement, collision probing, weighted direction choice and stage
 * transitions, with their interposed record-update, placement and rendering
 * services. Focused public headers retain the externally used subinterfaces.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "ovl11_record_pair_order.h"
#include "vblank_timer.h"
#include "sprite_rotation.h"
#include "ovl11_region_exit_query.h"
#include "ovl11_direction_vector.h"
#include "ovl11_movement_cell_rank.h"
#include "ovl11_movement_step.h"
#include "ovl11_row_placement.h"
#include "random.h"
#include "ovl11_area_record_updates.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_tagged_record_presentation.h"
#include "ovl11_pending_stage_dispatch.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_small_record_handlers.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_object_record_lifecycle.h"
#include "ovl11_directional_tile_probe.h"
#include "ovl11_object_effect_offset.h"
#include "ovl11_item_id_flags.h"
#include "ovl11_interaction_volume_query.h"
#include "ovl11_surface_height.h"
#include "ovl11_record_movement.h"
#include "ovl11_primary_object_flag29.h"
#include "ovl11_stage_mode2_record.h"
#include "ovl11_action_result_request.h"

void Ovl11UpdateRecordProbeFlag (Recon_ovl_11_func_800D2594_A0View *arg0);
s32 Ovl11ProbeMovementPosition (Ovl11Func800BDCE8Arg1 *arg0);
s32 Ovl11ChooseRecordDirection (Recon_ovl_11_func_800D2594_A0View *arg0, u16 arg1, s32 arg2);
void Ovl11TryRecordRegionExit (Recon_ovl_11_func_800D2594_A0View *arg0);

void Ovl11StepRecordMovement(Recon_ovl_11_func_800D2594_A0View *record,
                             s32 stepSize, s32 queryFlags) {
    VECTOR position;
    void *hitRecord;
    s32 hitType;
    s16 *stageWork;
    s32 stepValue;
    s16 probeResult;
    s32 recordFlags;
    s32 finalFlags;
    s32 activeQueryFlags;
    u16 itemHeight;
    u16 itemWidth;
    u16 itemId;
    u16 blockedDirections;

    activeQueryFlags = queryFlags;
    if (HasVSyncIntervalElapsed(0x96, &gOvl11MovementSpeedTimer, 1) == 1) {
        gOvl11MovementSpeedGear = gOvl11MovementSpeedGear != 1;
    }
    stepValue = (gOvl11MovementSpeedGear == 1 ? 0x438 : 0x2D0) / stepSize;
    Ovl11UpdateRecordProbeFlag(record);
    if (record->unk34 & 0x200000) {
        activeQueryFlags &= ~0x30;
    }
    if ((*(s16 *) ((u8 *) record + 0x18)) != 0) {
        --(*(s16 *) ((u8 *) record + 0x18));
        if ((*(s16 *) ((u8 *) record + 0x18)) < 0) {
            (*(s16 *) ((u8 *) record + 0x18)) = 0;
        }
        activeQueryFlags &= ~1;
    }
    if ((*(u16 *) ((u8 *) record + 0)) == 0x15B) {
        gOvl11MovementVolumeProbeEnabled = 0;
    } else {
        gOvl11MovementVolumeProbeEnabled = 1;
    }
    stageWork = (s16 *) &D_8007AFF0;
    if (((*(s16 *) ((u8 *) record + 0x30)) != stageWork[0x12A3B]) && (record->unk34 & 0x200000)) {
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) record + 0x48), 0);
        return;
    }
    recordFlags = record->unk34;
    if (recordFlags & 0x800000) {
        activeQueryFlags &= ~0x20;
    }
    itemId = *(u16 *) record;
    itemWidth = *(u16 *) ((u8 *) &gItemDataTable[itemId] + 0x24);
    itemHeight = *(u16 *) ((u8 *) &gItemDataTable[itemId] + 0x26);
    ++(*(s16 *) ((u8 *) record + 0x2E));
    if (((*(s16 *) ((u8 *) record + 0x2E)) >= stepValue) && !(recordFlags & 0x01000000)) {
        /* Bits 1/4/2 describe current/previous/next direction probes. */
        blockedDirections = 0;
        (*(u16 *) ((u8 *) record + 0x2E)) = 0U;
        Ovl11SetDirectionVector(*(s16 *) ((u8 *) record + 0x22), (Recon800D0408A1View *) &position, stepSize);
        position.vx += (*(s32 *) ((u8 *) record + 0x38));
        position.vz += *(s32 *) ((u8 *) record + 0x40);
        if (record->unk34 & 0x200000) {
            blockedDirections = Ovl11ProbeMovementPosition((Ovl11Func800BDCE8Arg1 *) &position) == 0;
        }
        if (record->unk34 & 0x800000) {
            if (Ovl11ProbeRecordStep(record, position, (s16) (s32) (*(s16 *) ((u8 *) record + 0x22)), 0) != 1) {
                blockedDirections |= 1;
            }
        }
        if (Ovl11QueryCollisionBudgeted(&position.vx, *(Ovl11CollisionTag *) ((u8 *) record + 0x2E), itemWidth, itemHeight, &hitRecord, &hitType, (s32) record, activeQueryFlags) == 0) {
            blockedDirections |= 1;
        }
        stepValue = *(s16 *) ((u8 *) record + 0x22);
        stepValue = (s16) (stepValue - 1);
        probeResult = 3;
        if (stepValue >= 0) {
            probeResult = stepValue;
        }
        Ovl11SetDirectionVector(probeResult, (Recon800D0408A1View *) &position, 0x190);
        position.vx += (*(s32 *) ((u8 *) record + 0x38));
        position.vz += *(s32 *) ((u8 *) record + 0x40);
        if ((record->unk34 & 0x200000) && (Ovl11ProbeMovementPosition((Ovl11Func800BDCE8Arg1 *) &position) == 0)) {
            blockedDirections |= 4;
        }
        if (record->unk34 & 0x800000) {
            if (Ovl11ProbeRecordStep(record, position, (s16) (s32) probeResult, 0) != 1) {
                blockedDirections |= 4;
            }
        }
        if (Ovl11QueryCollisionBudgeted((s32 *) &position, *(Ovl11CollisionTag *) ((u8 *) record + 0x2E), itemWidth, itemHeight, &hitRecord, &hitType, (s32) record, activeQueryFlags) == 0) {
            blockedDirections |= 4;
        }
        stepValue = *(s16 *) ((u8 *) record + 0x22);
        stepValue = (s16) (stepValue + 1);
        probeResult = 0;
        if (stepValue < 4) {
            probeResult = stepValue;
        }
        Ovl11SetDirectionVector(probeResult, (Recon800D0408A1View *) &position, 0x190);
        position.vx += *(s32 *) ((u8 *) record + 0x38);
        position.vz += *(s32 *) ((u8 *) record + 0x40);
        if ((record->unk34 & 0x200000) && (Ovl11ProbeMovementPosition((Ovl11Func800BDCE8Arg1 *) &position) == 0)) {
            blockedDirections |= 2;
        }
        if (record->unk34 & 0x800000) {
            if (Ovl11ProbeRecordStep(record, position, (s16) (s32) probeResult, 0) != 1) {
                blockedDirections |= 2;
            }
        }
        if (Ovl11QueryCollisionBudgeted((s32 *) &position, *(Ovl11CollisionTag *) ((u8 *) record + 0x2E), itemWidth, itemHeight, &hitRecord, &hitType, (s32) record, activeQueryFlags) == 0) {
            blockedDirections |= 2;
        }
        stepValue = (s16) Ovl11ChooseRecordDirection(record, blockedDirections, activeQueryFlags);
        Ovl11SetDirectionVector(stepValue, (Recon800D0408A1View *) ((u8 *) record + 0x68), 0x190);
        if (stepValue != 4) {
            (*(s16 *) ((u8 *) record + 0x22)) = stepValue;
        }
        (*(s32 *) ((u8 *) record + 0x68)) = (s32) ((*(s32 *) ((u8 *) record + 0x68)) + (*(s32 *) ((u8 *) record + 0x38)));
        (*(s32 *) ((u8 *) record + 0x70)) = (s32) ((*(s32 *) ((u8 *) record + 0x70)) + (*(s32 *) ((u8 *) record + 0x40)));
        record->unk34 |= 0x4000;
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) record + 0x48), 0);
    }
    if ((record->unk34 & 0x2000) && (Ovl11RankMovementCell(record) == 0)) {
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) record + 0x48), 0);
        Ovl11SelectRecordPairCode(record);
        return;
    }
    Ovl11SetDirectionVector(*(s16 *) ((u8 *) record + 0x22), (Recon800D0408A1View *) ((u8 *) record + 0x48), stepSize);
    position.vx = (*(s32 *) ((u8 *) record + 0x38)) + (*(s32 *) ((u8 *) record + 0x48));
    position.vz = (*(s32 *) ((u8 *) record + 0x40)) + (*(s32 *) ((u8 *) record + 0x50));
    position.vy = record->unk3C;
    finalFlags = record->unk34;
    probeResult = 0;
    if (!(finalFlags & 0x01000000)) {
        if (finalFlags & 0x200000) {
            probeResult = Ovl11ProbeMovementPosition((Ovl11Func800BDCE8Arg1 *) &position) != 0;
        }
        if (Ovl11QueryCollisionBudgeted((s32 *) &position, *(Ovl11CollisionTag *) ((u8 *) record + 0x2E), itemWidth, itemHeight, &hitRecord, &hitType, (s32) record, activeQueryFlags) != 0) {
            probeResult = 1;
        }
        if (record->unk34 & 0x800000) {
            if (Ovl11ProbeRecordStep(record, position, (s16) (s32) (*(s16 *) ((u8 *) record + 0x22)), 1) == 1) {
                probeResult = 1;
            }
        }
        if (probeResult == 1) {
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) record + 0x48), 0);
            (*(u16 *) ((u8 *) record + 0x2E)) = (u16) ((*(u16 *) ((u8 *) record + 0x2E)) + 2);
        }
        record->unk3C = position.vy;
        Ovl11TryRecordRegionExit(record);
    } else {
        Ovl11StepRecordVerticalOffset(record, stepSize);
    }
    gOvl11MovementRankThreshold = 0x258;
}

void Ovl11SetMovementRankThreshold(s16 threshold) {
    gOvl11MovementRankThreshold = threshold;
}

s32 Ovl11ChooseWeightedDirection(s32 arg0, s32 arg1, s32 arg2);
s32 Ovl11ProbeMovementPosition(Ovl11Func800BDCE8Arg1 *arg0);

s32 Ovl11ChooseRecordDirection(Recon_ovl_11_func_800D2594_A0View *arg0, u16 arg1, s32 arg2) {
    VECTOR position;
    void *hit;
    s32 hitType;
    s32 result;
    s32 extra;
    s16 direction;
    u16 width;
    u16 height;

    result = 4;
    extra = 0;
    if (arg0->unk34 & 0x2000) {
        extra = Ovl11RankMovementCell(arg0);
    }
    direction = *(s16 *)((u8 *)arg0 + 0x22);
    if (arg1 == 7) {
        result = Ovl11ChooseWeightedDirection((u16)direction, 0, extra);
    } else if (arg1 == 5) {
        result = Ovl11ChooseWeightedDirection((u16)direction, 1, extra);
    } else if (arg1 == 3) {
        result = Ovl11ChooseWeightedDirection((u16)direction, 2, extra);
    } else if (arg1 == 5) {
        result = Ovl11ChooseWeightedDirection((u16)direction, 1, extra);
    } else if (arg1 == 1) {
        result = *(u16 *)((u8 *)arg0 + 0x22);
    } else if (arg1 == 6) {
        result = Ovl11ChooseWeightedDirection((u16)direction, 3, extra);
    } else if (arg1 == 4) {
        s32 turn;
        direction--;
        turn = 3;
        if (direction >= 0) {
            turn = (u16)direction;
        }
        result = turn;
    } else if (arg1 == 2) {
        s32 turn;
        direction++;
        turn = 0;
        if (direction < 4) {
            turn = (u16)direction;
        }
        result = turn;
    } else {
        direction = direction < 2 ? direction + 2 : direction - 2;
        Ovl11SetDirectionVector(direction, (Recon800D0408A1View *)&position, 400);
        position.vx += *(s32 *)((u8 *)arg0 + 0x38);
        position.vz += *(s32 *)((u8 *)arg0 + 0x40);
        width = *(u16 *)((u8 *)&gItemDataTable[*(u16 *)arg0] + 0x24);
        height = *(u16 *)((u8 *)&gItemDataTable[*(u16 *)arg0] + 0x26);
        if (arg0->unk34 & 0x800000) {
            arg2 &= ~0x20;
        }
        if (arg0->unk34 & 0x200000) {
            if (Ovl11ProbeMovementPosition((Ovl11Func800BDCE8Arg1 *)&position)) {
                result = 4;
            } else {
                s32 blocked;
                blocked = Ovl11QueryCollisionBudgeted((s32 *)&position, ((Ovl11PlacedRecordView *)arg0)->collision, width, height, &hit, &hitType, (s32)arg0, arg2);
                result = (u16)direction;
                if (blocked) {
                    result = 4;
                }
            }
        } else {
            s32 original;
            s32 blocked;
            blocked = Ovl11QueryCollisionBudgeted((s32 *)&position, ((Ovl11PlacedRecordView *)arg0)->collision, width, height, &hit, &hitType, (s32)arg0, arg2);
            result = (u16)direction;
            original = result;
            if (blocked) {
                result = 4;
            }
            if (arg0->unk34 & 0x800000) {
                if (Ovl11ProbeRecordStep(arg0, position, direction, 0) != 1) {
                    result = original;
                }
            }
        }
    }
    return result;
}

/* Four candidate weights per cell rank and blocked-direction group.
 * A zero-weight candidate is skipped even when the roll is zero. The
 * comparison is inclusive; insufficient cumulative weight returns 4. */
s32 Ovl11ChooseWeightedDirection(s32 direction, s32 blockedGroup, s32 rank) {
    u16 *weights;
    u8 *base;
    s32 roll;
    s32 cumulative;
    s32 candidate;
    s32 chosen;
    s32 groupOffset;
    s32 directionOffset;

    direction &= 0xFFFF;
    blockedGroup &= 0xFFFF;
    rank &= 0xFFFF;

    roll = Rand(0x65);

    chosen = 4;
    candidate = 0;
    cumulative = 0;
    base = gOvl11DirectionChoiceWeights;
    groupOffset = blockedGroup * 9;
    directionOffset = direction * 288;
    weights = (u16 *)(((rank + groupOffset) * 8) + directionOffset + base);
    do {
        u16 weight = *weights;

        if (weight != 0) {
            cumulative += weight;
            if (cumulative >= roll) {
                chosen = candidate & 0xFFFF;
                break;
            }
        }
        candidate++;
        weights++;
    } while (candidate < 4);
    return chosen;
}

/*
 * Classify the X/Z displacement between two recorded object positions.
 * The 3x3 table maps signed axis ranks to a movement-cell code; a sufficiently
 * short move instead returns zero and clears the object's 0x2000 flag.
 * Public contract: ovl11_movement_cell_rank.h.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
s32 Ovl11RankMovementCell(Recon_ovl_11_func_800D2594_A0View *object) {
    /* This is a real GCC nested definition in the original: its entry saves
     * the incoming static chain, and the caller supplies that chain. Keeping
     * it nested also preserves the parent's table-address formation. */
    s32 Ovl11CompareMovementAxis(s32 from, s32 to) {
        if (from == to) {
            return 0;
        }
        if (from < to) {
            return 1;
        }
        return 2;
    }
    s32 x0;
    s32 x1;
    s32 z0;
    s32 z1;
    s32 dx;
    s32 dz;
    s32 xRank;
    s32 zRank;
    u16 result;

    z0 = *(s32 *) ((u8 *) object + 0x40);
    z1 = *(s32 *) ((u8 *) object + 0x60);
    x0 = *(s32 *) ((u8 *) object + 0x38);
    x1 = *(s32 *) ((u8 *) object + 0x58);
    dx = __builtin_abs(x1 - x0);
    dz = __builtin_abs(z1 - z0);
    xRank = Ovl11CompareMovementAxis(x0, x1);
    zRank = 0;
    if (dz * 3 >= dx) {
        zRank = Ovl11CompareMovementAxis(z0, z1);
    }
    if (dx < gOvl11MovementRankThreshold && dz < gOvl11MovementRankThreshold && (dx < 50 || dz < 50)) {
        result = 0;
        object->unk34 &= ~0x2000;
    } else {
        result = gOvl11MovementCellRanks[xRank * 3 + zRank];
    }
    return result;
}

void Ovl11UpdateRecordProbeFlag(Recon_ovl_11_func_800D2594_A0View *record) {
    VECTOR position;
    void *collision;
    s32 result;

    position.vx = *(s32 *) ((u8 *) record + 0x38);
    position.vy = record->unk3C;
    position.vz = *(s32 *) ((u8 *) record + 0x40);
    if (Ovl11QueryCollisionBudgeted((s32 *) &position,
                           *(Ovl11CollisionTag *) ((u8 *) record + 0x2E),
                           0, 0, &collision, &result, (s32) record, 0x10) != 0) {
        record->unk34 |= 0x200000;
        return;
    }
    record->unk34 &= 0xFFDFFFFF;
}

/*
 * area_record_updates — reconstructed translation unit.
 *
 * Four ordered area-frame sweeps over tagged, small, state-object and paired
 * stage-object pools. Dispatch record handlers, advance eligible
 * positions/timers, and draw or reset effects according to stage and work
 * flags.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

void Ovl11UpdateTaggedRecordPools(void) {
    Recon_ovl_11_func_8010CE80_A0View *entry;
    s32 *entryB4;
    s32 flags;
    s32 update_mask;
    s32 i;
    u8 *base;
    u8 *far_base;
    u8 *base2;
    u8 *far_base2;

    for (i = 0; i < 20; i++) {
        entry = (Recon_ovl_11_func_8010CE80_A0View *)((u8 *)&gTaggedRecordBase + i * 0xB8);
        base = (u8 *)D_8006C838;
        far_base = D_8009AFF0;
        if (*(u16 *)entry != 0 && (flags = entry->unk34, !(flags & 0x400))) {
            if (flags & 0x10000) {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            Ovl11StepTaggedRecordHandler(entry);
                            Ovl11ApplyTaggedRecordDisplacement((s32)entry);
                        }
                    } else {
                        Ovl11StepTaggedRecordHandler(entry);
                        Ovl11ApplyTaggedRecordDisplacement((s32)entry);
                    }
                    Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                    Ovl11SelectAndDrawTaggedRecordSprite((u8 *)entry);
                } else {
                    Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)entry + 0xA8));
                }
            } else {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            Ovl11TickCode160Record((UnkStruct800DF4F0 *)entry);
                            Ovl11AdvanceTaggedRecordPosition((s32)entry);
                        }
                    } else {
                        Ovl11TickCode160Record((UnkStruct800DF4F0 *)entry);
                        Ovl11AdvanceTaggedRecordPosition((s32)entry);
                    }
                    Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (!(entry->unk34 & 0x20000)) {
                    if (*(s16 *)(far_base + 0x5476) != *(s16 *)((u8 *)entry + 0x30)) {
                        Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)entry + 0xA8));
                    } else {
                        Ovl11DrawTaggedRecordSprite((u8 *)entry);
                    }
                }
            }
        }
    }
    for (i = 0; i < 10; i++) {
        entryB4 = (s32 *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4);
        far_base2 = D_8009AFF0;
        /* The existing s32 anchor gives resident work: 742EC - 7AB4 = 6C838. */
        base2 = (u8 *)(&gSmallTaggedRecordBase - 0x1EAD);
        if (*(u16 *)entryB4 != 0) {
            Ovl11StepSmallRecordHandler((s16 *)entryB4);
            update_mask = 0x20000;
            if (*(s16 *)(far_base2 + 0x5476) != *(s16 *)((u8 *)entryB4 + 0x30)) {
                Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4 + 0xA8));
            } else {
                if (!(update_mask & *(s32 *)((u8 *)entryB4 + 0x34))) {
                    if (!(*(s32 *)(base2 + 0xC) & 0x08000000)) {
                        Ovl11ApplySmallRecordDisplacement((s32)entryB4);
                    }
                    if (!(*(s32 *)((u8 *)entryB4 + 0x34) & 0x02000000)) {
                        Ovl11SelectAndDrawSmallRecordSprite((u8 *)entryB4);
                        Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4 + 0xA8));
                    }
                }
            }
        }
    }
}

void Ovl11UpdateSmallRecordTriplet(void) {
    s16 *entry;
    s32 i;
    u8 *far_base;
    u8 *base;

    for (i = 0; i < 3; i++) {
        entry = (s16 *)((u8 *)&gSmallRecordTripletBase + i * 0xB4);
        far_base = D_8009AFF0;
        base = (u8 *)D_8006C838;
        if (*(u16 *)entry != 0) {
            Ovl11StepSmallRecordHandler(entry);
            if (*(s16 *)(far_base + 0x5476) != *(s16 *)((u8 *)entry + 0x30)) {
                Ovl11ResetTimedSpriteEffect(entry + 0x54);
            } else {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    Ovl11ApplySmallRecordDisplacement((s32)entry);
                }
                Ovl11SelectAndDrawSmallRecordSprite((u8 *)entry);
                Ovl11TickTimedSpriteEffect((Ov11Timer *)(entry + 0x54));
            }
        }
    }
}

void Ovl11UpdateStateObjectPool(void) {
    s32 recordIndex;
    u16 *record;
    u8 *stateWork;
    u8 *stageWork;

    for (recordIndex = 0; recordIndex < 6; recordIndex++) {
        record = (u16 *)(gStateObjectPoolBytes + recordIndex * 0xB0);
        stateWork = (u8 *)D_8006C838;
        stageWork = D_8009AFF0;
        if ((*record != 0) && !((*(s32 *) ((u8 *) record + 0x34)) & 0x20000)) {
            Ovl11UpdateObjectState(record);
            if (!((*(s32 *) (stateWork + 0xC)) & 0x08000000)) {
                Ovl11AdvanceStateObjectPosition((s32) record);
                Ovl11TickTimedSpriteEffect((Ov11Timer *) ((record + 0x54)));
            }
            if ((*(s16 *) (stageWork + 0x5476)) != (*(s16 *) ((u8 *) record + 0x30))) {
                Ovl11ResetTimedSpriteEffect((s16 *) ((record + 0x54)));
            } else {
                Ovl11DrawStateObjectRecord((u8 *) record);
            }
        }
    }
}

s32 ovl_11_func_801090A4(Ovl11HandlerObjectView *arg0);
void Ovl11AdvanceRecordModeCounter(Ov11D266CFields *arg0, u16 *arg1, s32 arg2);
void ovl_11_func_80108CA4(s32 arg0);
void ovl_11_func_80108BE8(u8 *arg0);

/* The two paths share presentation, but only slot 0 has the date-row-8
 * exception. Retain the shared tail rather than conflating their guards. */
void Ovl11UpdatePairedStageObjects(void) {
    s32 slotIndex;
    struct_800759E4 *record;
    u8 *stateWork;
    u8 *stageWork;
    u8 *stateWork2;
    s32 skipPresentationMask;

    for (slotIndex = 0; slotIndex < 2; slotIndex++) {
        stateWork = (u8 *)D_8006C838;
        stageWork = D_8009AFF0;
        if (slotIndex == 0) {
            record = &gStageMode1Record;
            if ((gStageMode1Record.f00 != 0) && (!(gStageMode1Record.f34 & 0x400) || (Ovl11FindCalendarDate(*(s16 *) (stateWork + 0x44BA), *(s16 *) (stateWork + 0x44BC)) != 8))) {
                ovl_11_func_801090A4((Ovl11HandlerObjectView *) &gStageMode1Record);
                goto present_record;
            }
            goto next_record;
        }
        record = (struct_800759E4 *) &gStageMode2Record;
        if ((gStageMode2Record != 0) && !(((struct_800759E4 *) &gStageMode2Record)->f34 & 0x400)) {
            Ovl11UpdateStageMode2State(&((struct_800759E4 *) &gStageMode2Record)->f00);
present_record:
            skipPresentationMask = 0x20000;
            if ((*(s16 *) (stageWork + 0x5476)) != (s16) record->f30) {
                Ovl11ResetTimedSpriteEffect((s16 *) ((u8 *) record + 0xA8));
            } else if (!(record->f34 & skipPresentationMask)) {
                if (slotIndex == 0) {
                    if (!((*(s32 *) (stateWork + 0xC)) & 0x08000000)) {
                        Ovl11AdvanceRecordModeCounter((Ov11D266CFields *) record, (u16 *) &gOvl11Mode1DispatchCounter, 0x11);
                        ovl_11_func_80108CA4((s32) record);
                    }
                    ovl_11_func_80108BE8((u8 *) record);
                    Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) record + 0xA8));
                } else {
                    stateWork2 = (u8 *)D_8006C838;
                    if (!((*(s32 *) (stateWork2 + 0x5234)) & 0x100)) {
                        if (!((*(s32 *) (stateWork2 + 0xC)) & 0x08000000)) {
                            Ovl11AdvanceRecordModeCounter((Ov11D266CFields *) record, (u16 *) &gOvl11Mode2DispatchCounter, 0x11);
                            Ovl11AccumulateStageMode2Direction((s32) record);
                        }
                        Ovl11DrawStageMode2Record((u8 *) record);
                        Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) record + 0xA8));
                    }
                    goto next_record;
                }
            } else {
                goto next_record;
            }
        } else {
next_record:
            ;
        }
    }
}

/*
 * movement_step — reconstructed translation unit.
 *
 * Movement-record tile-probe latch and vertical-offset progression. The probe
 * sets bit 0x01000000; reaching zero in the vertical step clears that bit and
 * the active-step bit 0x00400000.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

s32 Ovl11ProbeRecordStep(Recon_ovl_11_func_800D2594_A0View *record,
                        VECTOR position, s16 direction, s32 latchResultTwo) {
    s32 result;

    result = Ovl11ProbeDirectionalTile(position,
                                *(Ovl11CollisionTag *) ((u8 *) record + 0x2E),
                                direction, 0);
    if (result == 2 && latchResultTwo == 1) {
        record->unk34 |= 0x01000000;
    }
    return result;
}

s32 Ovl11StepRecordVerticalOffset(Recon_ovl_11_func_800D2594_A0View *record,
                                 s32 movePeriod) {
    s32 verticalStep;
    s32 active;

    active = 1;
    /* Preserve the two signed divisions and the remainder correction: this
     * is integer arithmetic, not a continuous speed/period conversion. */
    verticalStep = 0x190 / movePeriod / 2;
    if (0x190 % movePeriod != 0) {
        verticalStep += 2;
    }
    verticalStep = 0xC8 / verticalStep;
    if ((record->unk34 & 0x400000) == 0) {
        if (record->unk3C < -0xC8) {
            record->unk4C = verticalStep;
            record->unk34 = record->unk34 | 0x400000;
        } else {
            record->unk4C = -verticalStep;
        }
    } else {
        record->unk4C = verticalStep;
        if (record->unk3C + verticalStep >= 0) {
            record->unk4C = 0;
            record->unk3C = 0;
            record->unk34 &= 0xFEFFFFFF;
            record->unk34 &= 0xFFBFFFFF;
            active = 0;
        }
    }
    return active;
}

/* The matched writer takes a four-word aggregate, including its unused pad. */
void ovl_11_func_800D05D0(Ovl11SetFieldsView *object, Ovl11PaddedVec3 position);

void Ovl11AdvanceRecordModeCounter(Ov11D266CFields *record,
                                   u16 *counter, s32 mode) {
    char *work;
    s32 maskedMode;

    maskedMode = mode & 0xFFFF;
    if (*counter < 300) {
        if (record->field_34 & 0x2000) {
            work = (char *)&D_8006C838;
            ovl_11_func_800D05D0((Ovl11SetFieldsView *)record,
                *(Ovl11PaddedVec3 *)(work + 0x52C8));
        }
        if (record->field_26 == maskedMode) {
            *counter = *counter + 1;
        }
    } else {
        *counter = 300;
    }
}

/*
 * pending_stage_dispatch — reconstructed translation unit.
 *
 * Consume a pending mode when the readiness predicate permits, then dispatch
 * stage-matched object handlers and clear per-mode counters when those
 * handlers return zero. Counter initialization and advancement live elsewhere;
 * this unit only consumes the pending request and performs the handler calls.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

void Ovl11DispatchStageMode(s32 mode);

void Ovl11ProcessPendingStageMode(void) {
    if ((gOvl11PendingStageMode != -1) && (Ovl11IsPrimaryObjectFlag29Clear() != 0)) {
        Ovl11DispatchStageMode((s32) gOvl11PendingStageMode);
        gOvl11PendingStageMode = -1;
    }
}

s32 ovl_11_func_80109188(Ovl11PrimaryStageHandlerObj *arg0, s32 arg1);

#define CURRENT_STAGE (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476)
#define MAIN_STAGE (((struct Ovl11D2764StageView *)D_8006C838)->field_91DC)
#define OTHER_STAGE (((struct Ovl11D2764StageView *)D_8006C838)->field_92CC)

/* Internal helper by use, not static linkage: preserve the original symbol.
 * Mode 0 scans all 20 slots even after a handler clears the shared counter. */
void Ovl11DispatchStageMode(s32 mode) {
    Ovl11StageHandlerObj *object;
    s32 slot;

    switch (mode) {
    case 0:
        if (CURRENT_STAGE == MAIN_STAGE) {
            ovl_11_func_80109188((Ovl11PrimaryStageHandlerObj *)&gStageMode1Record, 5);
        }
        for (slot = 0; slot < 20; slot++) {
            object = (Ovl11StageHandlerObj *)((u8 *)&gTaggedRecordBase + slot * 0xB8);
            if (*(u16 *)object != 0) {
                if (object->unk34 & 0x10000) {
                    if (CURRENT_STAGE == *(s16 *)((u8 *)object + 0x30)) {
                        if (Ovl11DispatchCode160Handler(object, 0xB) == 0) {
                            gOvl11Mode0DispatchCounter = 0;
                        }
                    }
                } else if (CURRENT_STAGE == *(s16 *)((u8 *)object + 0x30)) {
                    if (Ovl11DispatchTaggedRecordHandler(object, 0xB) == 0) {
                        gOvl11Mode0DispatchCounter = 0;
                    }
                }
            }
        }
        return;
    case 1:
        if ((CURRENT_STAGE == MAIN_STAGE) &&
            (ovl_11_func_80109188((Ovl11PrimaryStageHandlerObj *)&gStageMode1Record, 4) == 0)) {
            gOvl11Mode1DispatchCounter = 0;
            return;
        }
        return;
    case 2:
        if ((CURRENT_STAGE == OTHER_STAGE) &&
            (Ovl11ChangeStageMode2State(&gStageMode2Record, 0x13) == 0)) {
            gOvl11Mode2DispatchCounter = 0;
        }
        break;
    }
}

/*
 * object_effect_offset — reconstructed translation unit.
 *
 * Pure object-ID lookup for the screen-space vertical offset of the timed
 * effect drawn above an object. The renderer consumes the signed result in
 * pixels; this service changes no state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

s32 Ovl11GetObjectEffectYOffset(s32 objectId) {
    s32 relativeId;

    /* Keep the signed halfword normalization of the ID relative to 0xA1. */
    relativeId = (s32) ((objectId << 0x10) + 0xFF5F0000) >> 0x10;
    switch (relativeId) {
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x65:
    case 0x66:
    case 0x67:
    case 0x68:
    case 0x6A:
    case 0x6B:
    case 0x6C:
    case 0x6D:
    case 0x6E:
    case 0x6F:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBF:
    case 0xC3:
        return -0x14;
    case 0xC0:
    case 0xC4:
        return -0x1E;
    case 0xBD:
    case 0xBE:
    case 0xC1:
        return -0x28;
    case 0x69:
        return -0xA;
    default:
        return 0;
    }
}

/* Five-slot coordinate rows for overlay 11. The two entry points share the
 * placement protocol but retain different spacing, direction and mode rules.
 * Table provenance and supported unit boundaries: notes/file-groupings.md. */
s32 Ovl11PlaceRowZSlot(RowZPlacement *output, s16 slot) {
    s16 selected = slot;
    s16 row;
    s16 column;
    s32 mode;

    if (selected == -1) {
        row = 4;
        column = 0;
    } else if (selected == -2) {
        row = 5;
        column = 0;
    } else {
        row = selected / 5;
        column = selected % 5;
    }
    output->x = gOvl11RowZPlacementTable[row][0];
    output->y = gOvl11RowZPlacementTable[row][1];
    output->z = gOvl11RowZPlacementTable[row][2] + column * 600;
    output->direction = (row & 1) ? 1 : 3;
    mode = 5;
    output->mode = mode;
    return mode;
}

void Ovl11PlaceRowXSlot(RowXPlacement *output, s16 slot) {
    s16 row = slot / 5;
    s16 column = slot % 5;
    s32 direction;

    output->x = gOvl11RowXPlacementTable[row][0] + column * 0x190;
    output->y = gOvl11RowXPlacementTable[row][1];
    output->z = gOvl11RowXPlacementTable[row][2];
    direction = Rand(4);
    output->direction = direction;
    output->mode = 4;
}

void Ovl11TryRecordRegionExit(Recon_ovl_11_func_800D2594_A0View *arg0) {
    VECTOR sp20;
    u16 sp30;
    s16 temp_s0;
    s16 var_s2;
    s16 var_s4;
    s32 var_s0;
    s32 var_v1;
    u16 var_s3;
    u32 var_t0;
    u8 *work;

    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        work = (u8 *) &D_8007AFF0;
        var_s2 = 1;
        switch (var_s0) {
        case 0:
            var_v1 = 0x04000000;
            var_t0 = 0;
            var_s3 = 2;
            var_s4 = 2;
            break;
        case 1:
            var_v1 = 0x08000000;
            var_t0 = 1;
            var_s2 = 3;
            var_s3 = 2;
            var_s4 = 2;
            break;
        case 2:
            if (D_80070D0C == 0) {
                continue;
            }
            var_v1 = 0x10000000;
            var_t0 = 2;
            var_s2 = 4;
            var_s3 = 1;
            var_s4 = 1;
            break;
        case 3:
            var_v1 = 0x20000000;
            var_t0 = 3;
            var_s2 = 2;
            var_s3 = 1;
            var_s4 = 1;
            break;
        default:
            continue;
        }
        if ((arg0->unk34 & var_v1) && (Ovl11FindRegionExit(*(VECTOR *) ((u8 *) arg0 + 0x38), *(s16 *) ((u8 *) arg0 + 0x30), &sp20, &sp30, var_t0) != 0)) {
            temp_s0 = *(s16 *) (work + 0x25476);
            if (temp_s0 == 1) {
                Ovl11ResolveActionResultOrRotate(var_s2, var_s3, NULL, 1);
                (*(s16 *) ((u8 *) arg0 + 0x2C)) = 0;
                if ((*(s16 *) ((u8 *) arg0 + 0x30)) == temp_s0) {
                    (*(s16 *) ((u8 *) arg0 + 0x22)) = var_s4;
                } else {
                    Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
                }
                if (Ovl11GetSpriteRotationState((s32) var_s2) == 3) {
                    (*(s32 *) ((u8 *) arg0 + 0x38)) = sp20.vx;
                    (*(s32 *) ((u8 *) arg0 + 0x40)) = sp20.vz;
                    (*(s16 *) ((u8 *) arg0 + 0x30)) = (s16) sp30;
                    Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
                    arg0->unk34 &= ~0x2000;
                    (*(s16 *) ((u8 *) arg0 + 0x18)) = 0x5A;
                }
            } else {
                (*(s32 *) ((u8 *) arg0 + 0x38)) = sp20.vx;
                (*(s32 *) ((u8 *) arg0 + 0x40)) = sp20.vz;
                (*(s16 *) ((u8 *) arg0 + 0x30)) = (s16) sp30;
                Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
                arg0->unk34 &= ~0x2000;
            }
            break;
        }
    }
}

/*
 * item_id_flags — reconstructed translation unit.
 *
 * Add item-code-specific bits to a record flag word at +0x34, preserving all
 * existing bits. Shared by allocation and item transitions; it owns no state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

void Ovl11ApplyItemIdFlags(void *record) {
    u16 itemId = *(u16 *)record;
    u32 *flags = (u32 *)((char *)record + 0x34);

    switch (itemId) {
    case 0xA1:
    case 0x108:
    case 0x10B:
    case 0x10C:
    case 0x10D:
    case 0x10E:
    case 0x10F:
    case 0x110:
    case 0x112:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBD:
    case 0xBE:
    case 0xBF:
    case 0xC0:
    case 0xC1:
    case 0xC3:
    case 0xC4:
    case 0x159:
    case 0x15B:
    case 0x165:
        break;
    case 0x162:
    case 0x163:
        *flags |= 0x80000000;
        return;
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
    case 0x117:
    case 0x118:
    case 0x119:
    case 0x11A:
    case 0x153:
    case 0x154:
    case 0x155:
    case 0x156:
    case 0x157:
        *flags |= 0x1000;
        break;
    }
}

s32 Ovl11ProbeMovementPosition(Ovl11Func800BDCE8Arg1 *position) {
    if (gOvl11MovementVolumeProbeEnabled == 1) {
        if (Ovl11ProbeInteractionVolume((u8 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header, position, 0) != NULL) {
            return 1;
        }
    }
    return Ovl11ResolveSurfaceHeight(&position->unk0, 0xC000) != 0;
}
