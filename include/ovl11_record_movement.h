#ifndef OVL11_RECORD_MOVEMENT_H
#define OVL11_RECORD_MOVEMENT_H

#include "common.h"
#include "game_types.h"

/* Step a placed record's movement/collision state. stepSize is a nonzero
 * positive world-coordinate increment; it also divides the 720/1080 cadence
 * numerator. queryFlags are forwarded to the budgeted collision service with
 * record-dependent bits removed. The caller must supply the established
 * record view, valid item ID, direction 0..3 and initialized scene/pool state.
 * Updates direction, position/probe fields, countdowns and movement flags;
 * may select a weighted direction through Rand or cross a region exit.
 * A shared gear toggles after 150 SDK VSync counter units, not 150 calls.
 * Uses shared probe mode, timer and rank threshold: this is non-reentrant.
 * Normal completion resets the rank threshold to 600; early returns do not.
 * No elapsed-time integration or lower/upper position clamp is added. */
void Ovl11StepRecordMovement(Recon_ovl_11_func_800D2594_A0View *record,
                             s32 stepSize, s32 queryFlags);

/* Set the raw signed X/Z rank threshold used by the next movement query.
 * No clamping or record update; the movement step normally restores 600. */
void Ovl11SetMovementRankThreshold(s16 threshold);

/* The unit also publishes its independently used subinterfaces through
 * ovl11_movement_cell_rank.h, ovl11_movement_step.h,
 * ovl11_area_record_updates.h, ovl11_pending_stage_dispatch.h,
 * ovl11_row_placement.h, ovl11_item_id_flags.h and
 * ovl11_object_effect_offset.h. Co-location does not make their entries static. */

#endif
