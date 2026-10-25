#ifndef OVL15_ROW_PLACEMENT_H
#define OVL15_ROW_PLACEMENT_H

#include "common.h"
#include "game_types.h"

/* Overlay-15 coordinate-row writers; separate entry points and tables from
 * overlay 11. Coordinates use integer object/world units. Only position at
 * +0x38/+0x3C/+0x40, direction at +0x22 and mode at +0x30 are changed.
 * No input validation, collision checking, allocation or initialization of
 * other object fields occurs. Both tables have five slots per row.
 */

/* slot 0..9: row=slot/5, column=slot%5. Set X=row.X+column*400,
 * copy Y/Z, direction=Rand(4), mode=4. Advances the shared EXE RNG.
 * Negative slots are not sentinels and can index outside the table.
 * Original identity: ovl_15:0x80132A8C. */
void Ovl15PlaceRowXSlot(RowXPlacement *output, s16 slot);

/* slot 0..29: row=slot/5, column=slot%5; -1/-2 select row 4/5,
 * column 0. Copy X/Y, set Z=row.Z+column*600, direction=1 for odd rows
 * or 3 for even rows, mode=5, return 5. Does not use RNG.
 * Original identity: ovl_15:0x80132B44. */
s32 Ovl15PlaceRowZSlot(RowZPlacement *output, s16 slot);

#endif
