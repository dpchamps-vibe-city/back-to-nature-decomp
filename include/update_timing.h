#ifndef UPDATE_TIMING_H
#define UPDATE_TIMING_H

#include "common.h"

/*
 * Raw, unitless timing selector, not elapsed time or a hardware refresh rate.
 * Initial data is 2; the scene update loop also sets 2 on entry. Consumers
 * interpret the same stored byte differently:
 *
 * - Input repeat (exe:80013CD0) divides its delay/interval settings by 2 or 3
 *   for those modes, otherwise by 1; repeat intervals are clamped to >= 1.
 * - CD-volume decay (exe:8002194C) subtracts 2 or 3 per eligible update for
 *   those modes, otherwise 1, with the remaining volume clamped to zero.
 * - Feedback fade (exe:80012D30) uses alternating 0/1 counter steps for mode
 *   0 and a step of 1 for every nonzero mode. Other fade modes ignore it.
 *
 * No common duration unit, boolean interpretation or closed enum is implied.
 */

/* Store the byte verbatim, without validation or immediate timing/SDK work.
 * Takes effect when consumers next query it; does not wait, schedule updates,
 * or reset their counters. Original exe:80011F2C (SetFlag8005E274). */
void SetUpdateTimingMode(u8 mode);

/* Return the stored byte without changing it. Original exe:80011F38
 * (GetFlag8005E274). Values outside 0..3 are preserved, not normalized. */
u8 GetUpdateTimingMode(void);

#endif
