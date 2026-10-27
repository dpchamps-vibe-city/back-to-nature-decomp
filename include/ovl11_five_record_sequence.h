#ifndef OVL11_FIVE_RECORD_SEQUENCE_H
#define OVL11_FIVE_RECORD_SEQUENCE_H

#include "common.h"

/* ovl_11:800EB85C. Step a fixed five-record position/animation choreography.
 * phaseSlot is an unchecked signed index into the shared value bank. The
 * signed LOW halfword selects phases 0..16; the updated phase is written as
 * a full signed word. Always returns 1, even while the stored phase waits.
 * The caller, not this function, determines when the next step occurs.
 *
 * Phase 0 places the five table-selected records at Y=0 and their X/Z
 * coordinates, forces animation 5/frame 0, then advances to 1. Later phases
 * wait for animation queries or configure/clamp raw-word Z motion through
 * the state-record motion service. There is no position integration here.
 * Movement limits are raw record units; no fixed-point conversion is done.
 * Each loop phase advances from its LAST record's result only, not from an
 * all-record completion test. Fixed-record phases visit table indices in
 * order 1,3,0,4,2. Phase 16 loops back to 1, not the initialization phase.
 *
 * A legacy record predicate is sampled on every call. Unless it returns
 * exactly 1 for the first ID, loops start at index 1 instead of 0, and a
 * successful phase 7 skips phase 8. Fixed-record reset operations still run
 * as written. Unknown phases do nothing except the signed-word writeback.
 * Records, sprite sources and shared value storage must already be valid.
 * All table/state storage remains external; no private linkage is implied. */
s32 Ovl11StepFiveRecordSequence(s16 phaseSlot);

#endif
