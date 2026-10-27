#ifndef OVL11_RECORD_APPROACH_SEQUENCE_H
#define OVL11_RECORD_APPROACH_SEQUENCE_H

#include "common.h"

/* ovl_11:800F07EC. Step recordId's X/Z and caller-owned phase (0..9).
 * Phases 0..3 wait on animation 5; 4 moves Z by +10 toward targetZ+600;
 * 5 moves Z by -10 toward targetZ; 6 moves X by +/-10 toward targetX+/-500
 * (positive for an ordinary counter slot, negative for a special slot);
 * 7 waits on animation 8; 8 returns X to targetX; 9 waits on animation 8/7
 * (ordinary/special). Y is untouched. Units are the record's raw coordinates.
 * Movement completes only on STRICT overshoot, then clamps. Animation/move
 * completion advances the phase but returns 0, except phase 9 completion
 * resets phase to 0 and returns 1. Unknown phases do nothing and return 0.
 * No position/phase/index validation; arithmetic must remain in s32 range.
 * Requires initialized record, sprite state and a writable phase word. */
s32 Ovl11StepRecordApproachSequence(s16 recordId, s32 targetX, s32 targetZ,
                                    u32 *phase);

#endif
