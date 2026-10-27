#ifndef OVL11_KEYFRAME_STEP_H
#define OVL11_KEYFRAME_STEP_H

#include "common.h"
#include "game_types.h"

/* Read {s16 code, s16 duration} rows at table + 4*(index + low16(bias)).
 * Write code to object+24, increment the u16 tick counter, and compare its
 * signed low halfword to duration. On expiry increment index and clear ticks;
 * use the following row's code immediately, or return 1 if that code is -1.
 * A terminal step leaves the current code at +24. Otherwise return 0.
 * If object+30 equals the shared mode at D_8007AFF0+25476, expiry also
 * requires an object+7A flag in mask 300; ticks still advance while gated.
 * The table must contain the accessed row and following code. No bounds or
 * negative-index checks are made. Counters wrap at their original widths. */
s32 Ovl11StepPackedKeyframes(Ovl11D04D4Obj *object, u8 *table, s16 *index,
    s32 bias, u16 *ticks);

#endif
