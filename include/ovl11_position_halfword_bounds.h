#ifndef OVL11_POSITION_HALFWORD_BOUNDS_H
#define OVL11_POSITION_HALFWORD_BOUNDS_H

#include "common.h"

/* ovl_11:0x800E74AC. Return 1 iff both absolute distances are strictly below
 * their signed radii. Zero/negative radii never pass. No state changes.
 * Reads signed halfwords at work +0x52C8/+0x52D0, the LOW halves of the X/Z
 * words, rather than converting or testing the full position words. Targets
 * use these same raw units; no fixed-point conversion or wrapping is applied.
 * The caller must keep subtraction/absolute value within signed-word range.
 */
s32 Ovl11TestPositionHalfwordBounds(s16 xRadius, s16 zRadius,
                                   s32 targetX, s32 targetZ);

#endif
