#ifndef OVL11_ROW_PLACEMENT_H
#define OVL11_ROW_PLACEMENT_H

#include "common.h"
#include "game_types.h"

/* Overlay-11 table-driven position writers; coordinates are the same integer
 * world units as the object's +0x38/+0x3C/+0x40 triple. Each row has five slots.
 * Only that triple, direction (+0x22) and mode (+0x30) are changed. Inputs are
 * unchecked; no allocation, collision test or object initialization occurs.
 * The distinct signed/unsigned halfword views preserve the matched interfaces.
 */

/* slot 0..29 selects row=slot/5, column=slot%5; -1/-2 select row 4/5,
 * column 0. Copy row X/Y and set Z=row.Z+column*600. Set direction to 1
 * for odd rows, 3 for even rows; set mode=5 and return 5. No RNG use.
 * Original identity: ovl_11:0x800D29B0. */
s32 Ovl11PlaceRowZSlot(RowZPlacement *output, s16 slot);

/* slot 0..9 selects row=slot/5, column=slot%5. Set X=row.X+column*400,
 * copy row Y/Z, set direction=Rand(4), mode=4. Advances shared EXE RNG state.
 * Negative slots are not sentinels and can index outside the table.
 * Original identity: ovl_11:0x800D2A88. */
void Ovl11PlaceRowXSlot(RowXPlacement *output, s16 slot);

#endif
