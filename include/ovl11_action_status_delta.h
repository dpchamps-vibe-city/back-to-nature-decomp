#ifndef OVL11_ACTION_STATUS_DELTA_H
#define OVL11_ACTION_STATUS_DELTA_H

#include "common.h"

/* Return baseDelta +1 for nonzero day condition, +1 at hours 22/23,
 * +2 before hour 6, and +2 when the primary-object +12 gauge is zero.
 * Then halve (signed division toward zero) if +6C bit 0200 is set;
 * finally return zero if +36 bit 0008 is set. No state is changed.
 * Callers pass the result to the secondary status-gauge delta service.
 * Additions must fit s32; there is no final range clamp. */
s32 Ovl11AdjustActionStatusDelta(s32 baseDelta);

#endif
