#ifndef PAD_INPUT_SETUP_H
#define PAD_INPUT_SETUP_H

#include "common.h"

/* Reset the two 0x38-byte input records, both SDK read buffers, and both
 * eight-byte actuator tables; then set both repeat timing pairs to (20, 7).
 * Boot must first bind gPad0Input/gPad1Input to valid records. This clears
 * all record bytes, including the actuator-request bytes, but neither starts
 * nor stops controller communication and does not change the actuator-enable
 * flag. Like boot, coordinate this reset with controller polling/communication;
 * it supplies no synchronization or immediate PadSetAct command. */
void ResetPadInputState(void);

/* Set the independent repeat timings for both pads. The first pair controls
 * the repeat output at record +0x00, the secondary pair the output at +0x24.
 * Both records must be bound. No counters, button state, or outputs are reset.
 *
 * delay/interval are signed base update counts, not milliseconds or VSyncs.
 * The processor divides them by GetUpdateTimingMode() when it is 2 or 3,
 * otherwise by 1, and clamps only the effective interval to at least 1.
 * A changed held-button mask restarts both streams and reports new presses.
 * While unchanged, the counter must exceed the effective delay to enter
 * repeat mode; subsequent polls emit the held mask on interval multiples.
 * No validation is done here: use nonnegative delay and positive interval;
 * negative delay has unsigned-comparison behavior in the existing processor.
 * Neither function configures graphics or changes controller hardware. */
void SetPadRepeatTiming(s32 delay, s32 interval);
void SetPadSecondaryRepeatTiming(s32 delay, s32 interval);

#endif
