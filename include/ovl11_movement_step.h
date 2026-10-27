#ifndef OVL11_MOVEMENT_STEP_H
#define OVL11_MOVEMENT_STEP_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* Probe a by-value world position using the record's collision tag at +0x2E.
 * Direction 0/1/2/3 means -Z/-X/+Z/+X. Returns the tile-probe code unchanged;
 * result 2 sets flag 0x01000000 only when latchResultTwo is EXACTLY 1.
 * Other results do not clear that latch. Requires an initialized tile map. */
s32 Ovl11ProbeRecordStep(Recon_ovl_11_func_800D2594_A0View *record,
                        VECTOR position, s16 direction, s32 latchResultTwo);

/* Set Y delta (+0x4C) for the record's vertical offset (+0x3C), in raw world
 * units per caller update. The caller integrates position separately.
 * Start with negative delta; below -200, set 0x00400000 and reverse it.
 * On a later active-step update where Y+delta >= 0, zero Y and delta, clear
 * both 0x00400000 and the probe latch, and return 0; otherwise return 1.
 * This progression expects a positive movePeriod from the movement-period
 * selector. It must be nonzero
 * and 400/movePeriod/2 + (400%movePeriod != 0 ? 2 : 0) must be nonzero.
 * No division, overflow or invalid-record guards are supplied. */
s32 Ovl11StepRecordVerticalOffset(Recon_ovl_11_func_800D2594_A0View *record,
                                 s32 movePeriod);

#endif
