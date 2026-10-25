#ifndef PAD_ACTUATOR_SEQUENCES_H
#define PAD_ACTUATOR_SEQUENCES_H

#include "common.h"

/*
 * Two unchecked sequence slots (0/1), NOT a per-controller-port API. Both
 * slots write the first PadInputState's actuator request bytes; slot 1 runs
 * after slot 0 and may overwrite them. Pad polling/output consumes those bytes.
 * This is software request generation, not a direct PadSetAct call.
 */

/* exe:0x8001B258. Dispatch each active slot once per call. A three-byte step
 * contains flags (bit 0: request0=1; bit 1: request1=magnitude), magnitude,
 * and a duration byte. Increment the slot counter and advance when it exceeds
 * duration >> 1, then emit the selected step. A 0xFF flags byte resets the
 * slot and output. Timing is in calls, not milliseconds or video fields. */
void UpdatePadActuatorSequences(void);

/* exe:0x8001B2CC. Select an unchecked preset index (the table has 11 entries).
 * Ignore requests while the external-input flag 0x10000 is set or actuator
 * output is disabled. An idle slot accepts any preset; an active slot accepts
 * only a different sequence whose priority is >= its current priority.
 * Acceptance does NOT reset the step counter or immediately write output. */
void RequestPadActuatorSequence(s32 slot, s32 preset);

/* exe:0x8001B3CC. Immediately clear this slot's sequence, counter, mode and
 * priority, AND both first-pad output bytes, even if the other slot is active.
 * Unlike Request, this operation is not gated by actuator enable state. */
void StopPadActuatorSequence(s32 slot);

#endif
