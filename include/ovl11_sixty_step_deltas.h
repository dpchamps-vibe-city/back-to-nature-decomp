#ifndef OVL11_SIXTY_STEP_DELTAS_H
#define OVL11_SIXTY_STEP_DELTAS_H

#include "common.h"

/* Pure calculations used together by the primary-object reset path.
 * steps = value / 60, with C signed division (toward zero).
 * Decrease returns max(-2*steps - 5, -128).
 * Increase returns min( 7*steps + 50, 127).
 * Only the stated side is clamped: negative inputs need not produce a
 * signed-byte result. Products must fit s32. Neither routine changes state;
 * the caller applies the deltas to the primary-object counters. */
s32 Ovl11ComputeSixtyStepDecrease(s32 value);
s32 Ovl11ComputeSixtyStepIncrease(s32 value);

#endif
