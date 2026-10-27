#ifndef OVL11_SEASON_MASK_H
#define OVL11_SEASON_MASK_H

#include "common.h"

/* Read the resident calendar's signed season index without changing it.
 * Returns 1, 2, 4 or 8 for seasons 0..3; every other value returns 1.
 * Placement/record rules AND this result with their enabled-season bits.
 * The switch's default is intentional: this is not an unchecked shift. */
s32 Ovl11GetCalendarSeasonMask(void);

#endif
