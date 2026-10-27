/*
 * Read-only rectangular-distance command over the primary work position.
 * This tests signed LOW HALFWORDS of the X/Z words, not full coordinates.
 * See ovl11_position_halfword_bounds.h for the deliberately strict bounds.
 */

#include "common.h"
#include "ovl11_position_halfword_bounds.h"

s32 Ovl11TestPositionHalfwordBounds(s16 xRadius, s16 zRadius,
                                   s32 targetX, s32 targetZ) {
    s32 result;
    s16 *workHalfwords;
    s32 dx;
    s32 x;
    s32 z;
    s32 dz;
    s32 xLimit;
    s32 zLimit;

    result = 0;
    xLimit = (s16)xRadius;
    zLimit = (s16)zRadius;
    workHalfwords = (s16 *)&D_8006C838;
    x = workHalfwords[0x2964];
    z = workHalfwords[0x2968];
    dx = __builtin_abs(x - targetX);
    if (dx < xLimit) {
        dz = __builtin_abs(z - targetZ);
        result = dz < zLimit;
    }
    return result;
}
