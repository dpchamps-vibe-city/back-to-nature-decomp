#ifndef OVL11_CURRENT_RECORD_BOUNDS_H
#define OVL11_CURRENT_RECORD_BOUNDS_H

#include "common.h"
#include "game_types.h"

/* Return the installed current record when its signed selector equals the
 * active selector and position is strictly within +/-200 in X/Z, +/-20 in Y.
 * Coordinates use the caller's world-coordinate units, not screen pixels.
 * On a hit set gOvl11RecordSpriteYOffsetIndex to 3; on a miss return NULL
 * without changing it. This operation does not enable flag 0x100 itself.
 * Both pointers must be valid; coordinate +/- bounds must not overflow s32.
 * The returned partial view aliases the spawn record used by moving_items.
 */
Ovl11CurrentRecordBoundsEntry *Ovl11ProbeCurrentRecordBounds(Vec3 *position);

#endif
