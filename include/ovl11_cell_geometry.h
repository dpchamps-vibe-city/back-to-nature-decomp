#ifndef OVL11_CELL_GEOMETRY_H
#define OVL11_CELL_GEOMETRY_H

#include "common.h"
#include "game_types.h"

/* layer must be 0 (45x25) or 1 (7x7). Return 1 if outside that layer's
 * bounds or requireFlag8 != 0 and the pointer-grid entry lacks bit 3;
 * otherwise return 0. In-range pointer-grid entries must be initialized
 * and non-null, even when requireFlag8 is zero. No state changes. */
s32 Ovl11IsCellUnusable(s16 column, s16 row, s16 requireFlag8, s16 layer);

/* Write X/Z only, preserving Y, and return Z. Cell spacing is 400 world
 * units. Layer 0: X=column*400-4200, Z=1400-row*400; any nonzero layer:
 * X=column*400-1420, Z=1020-row*400. No bounds checks. This is not the exact
 * inverse of the mode-1 position lookup, which divides by 399. */
s32 Ovl11GetCellCornerPosition(s16 column, s16 row, Vec3 *position, s16 layer);

#endif
