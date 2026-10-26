#ifndef OVL11_GRID_POSITION_H
#define OVL11_GRID_POSITION_H

#include "common.h"

/* Shared address-taken position-to-cell interface: three signed XYZ words,
 * two halfword outputs. The outputs are written even on an out-of-range
 * result. Negative biased coordinates return 2 (decrementing that axis's
 * truncated cell); a high column/row returns 3. Otherwise return 1 when the
 * active matching mode's pointer-grid entry lacks bit 3, or 0.
 * Requires initialized, non-null grid entries. Coordinate bias wraps through
 * signed halfword precision before division; these are not full-range world
 * conversions. The internal dead four-word copy is not an output operation.
 * Mode 1: 45x25, X bias 4400, Z mirrored about 1600, cell divisor 399.
 * Mode 6: 7x7, X bias 1620, Z mirrored about 1220, cell divisor 400. */
s32 Ovl11LookupMode1GridPosition(s32 *position, u16 *column, u16 *row);
s32 Ovl11LookupMode6GridPosition(s32 *position, u16 *column, u16 *row);

#endif
