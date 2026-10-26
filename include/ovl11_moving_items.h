#ifndef OVL11_MOVING_ITEMS_H
#define OVL11_MOVING_ITEMS_H

#include "common.h"
#include "game_types.h"

/* Reset the three 0x34-byte slots. Clear occupancy, timers/state, fall step,
 * velocity and position, but preserve direction, ground height and reserved
 * words at +0x20/+0x30. This does not perform the release helper's shared
 * primary-object/item side effects. Requires resident ovl_11 storage. */
void Ovl11ResetMovingItemRecords(void);

/* First-free allocation (item ID at +2 is zero). Copy position's three signed
 * world-coordinate words, lowering y by 300, and use direction 0..3 to index
 * the private X/Z velocity table (Q12 world units per update). Ground height
 * comes from shared work +0x52CC. IDs 0x65/0x186 select special initial motion.
 * Returns NULL when full. Only *itemId is read; ID zero leaves the returned
 * slot free. No pointer/index validation. Existing slots are not displaced. */
Ovl11MovingItemRecord *Ovl11SpawnMovingItemRecord(Vec3 *position,
                                               u16 *itemId, s16 direction);

/* Update/draw each occupied slot once. X/Z motion divides Q12 velocity by
 * 4096; y uses the signed fall step, increased by 9 and capped at 45. Fade
 * and primary-work flag 0x08000000 gate simulation, not all rendering.
 * Collision/landing may release a slot, award/queue items, alter flags,
 * request messages/sounds or install a primary-object request. Projected
 * sprites advance shared render packet cursors and use private scratch and
 * one shared animation source; do not call reentrantly. */
void Ovl11UpdateMovingItemRecords(void);

#endif
