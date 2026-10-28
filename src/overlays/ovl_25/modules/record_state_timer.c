/*
 * record_state_timer — reconstructed translation unit.
 *
 * Record state transitions: initialize the signed timer only when the state
 * changes.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl25_record_state_timer.h"

void Ovl25SetRecordState(s16 *record, s16 newState, s16 timerTicks) {
    s32 previousState = record[1];

    /* Repeated state requests must not restart an in-progress countdown. */
    if (previousState != newState) {
        record[2] = timerTicks;
    }
    record[1] = newState;
}
