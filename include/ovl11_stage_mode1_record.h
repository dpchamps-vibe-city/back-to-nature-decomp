#ifndef OVL11_STAGE_MODE1_RECORD_H
#define OVL11_STAGE_MODE1_RECORD_H

#include "common.h"
#include "game_types.h"

/* Mode-1 singleton record with paired transition-check/per-frame script banks.
 * Scripts consume the preceding probes' shared results; callers must
 * serialize this lifecycle rather than invoke callbacks independently.
 * Requires initialized record/spawn pools, sprite resources and render state.
 * Positions use the existing world-coordinate units; all timers count calls.
 */

/* Draw the embedded sprite using state/facing animation pairs and the current
 * render-context offsets. The global drawing-pause bit suppresses this work.
 * The record and state/facing indices must already be initialized and valid. */
void Ovl11DrawStageMode1Record(u8 *record);

/* Accumulate the direction vector at +48 into the position at +38.
 * recordAddress retains the original raw 32-bit address ABI. */
void Ovl11AccumulateStageMode1Direction(s32 recordAddress);

/* Acquire the sole F0-byte slot for IDs 106/107 only when mode==1 and empty.
 * Initializes the record and sprite, retaining metadata at +4..+14.
 * Returns NULL on rejection or when occupied; does not allocate another slot. */
struct_800759E4 *Ovl11AcquireStageMode1Record(s32 id, s32 mode);

/* Update signed-halfword counters, apply narrow clamps and consume flags.
 * Returns -1 for an empty record, otherwise 0 (including calendar-gated work).
 * May change ID/sprite resources, state, effects and the fixed spawn record. */
s32 Ovl11UpdateStageMode1Counters(s32 *record);

/* Run the selected callback and the 300-call automatic state-selection gate.
 * Counters measure eligible calls, not seconds; returns -1 for an empty slot. */
s32 Ovl11StepStageMode1Record(Ovl11HandlerObjectView *record);

/* Request an unchecked state-table index. Calendar/state guards may reject
 * it (-1), retain it (0), or return its callback's result. Accepted changes
 * clear keyframe/timer fields and reset the direction vector. Subsequent
 * frame steps run the paired script, which may consume probe results, change
 * another record or publish a snapshot to fixed spawn slot 1 (kind 0x65). */
s32 Ovl11SetStageMode1State(Ovl11D04D4Obj *record, s32 state);

/* Synchronously replace the record's copied sprite data and embedded source.
 * Waits for drawing and clears the current ordering table; not reentrant. */
s32 Ovl11RefreshStageMode1Sprite(s32 *record);

/* Set the singleton's selector and raw world position for preset 0 or 1.
 * Other values leave it unchanged. Does not initialize the sprite or state. */
void Ovl11SetStageMode1PositionPreset(s32 preset);

#endif
