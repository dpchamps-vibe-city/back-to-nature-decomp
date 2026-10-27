#ifndef OVL11_RECORDED_INPUT_PLAYBACK_H
#define OVL11_RECORDED_INPUT_PLAYBACK_H
#include "common.h"

/* Step the shared 0x738-sample input stream once per call. Active flag 10000
 * copies one sample and advances the cursor while in range; exhaustion requests
 * mode 1A only if the current mode is zero, retaining the last sample. Start
 * flag 8000 switches to active, clears held/pressed buttons and consumes the
 * first sample WITHOUT a bounds check. Caller must bind the buffer and set a
 * valid cursor before starting. Active takes precedence if both flags are set.
 * Returns 1 only for the start transition, otherwise 0. Does not reset cursor,
 * clear the active flag or impose a sample rate. Not reentrant. */
s32 Ovl11StepRecordedInputPlayback(void);
#endif
