#ifndef OVL11_GRID_ITEM_REMOVAL_COMMAND_H
#define OVL11_GRID_ITEM_REMOVAL_COMMAND_H

#include "common.h"

/* ovl_11 must be resident and both map grids initialized.
 * mode 0: if suppressReset == 0, replace main-grid ID 0x40 with 0x167 throughout
 *         its 25 rows/45 columns, clearing bytes +4/+5/+6. Return 1.
 * mode 1: remove up to 50 random cells of IDs 0x31/0x32/0x178/0x34/0x35/0x179.
 *         A failed area/kind search disables that combination for this call.
 *         Work+44D4 == 0 excludes the alternate area; otherwise choose either
 *         area until all alternate-area combinations are exhausted. Return 0
 *         even after all fifty removals. This mode consumes random numbers.
 * mode 2: search alternate then main area, in the ID order above. Resolve the
 *         first found cell's item state into gOvl11ValueSlots[resultSlot], then
 *         remove it and return 1; return 0 on no match without writing the
 *         slot. The shared search still randomly chooses a matching cell.
 * Other modes return 1 without changes. suppressReset is ignored outside 0;
 * resultSlot is unchecked and used only in 2.
 *
 * Removal restores each cell's ID from +2 and clears +4/+5/+6; it is not a
 * zero-fill of the whole record. Partial removals are never rolled back.
 * Original identity: ovl_11:800E7D4C. */
s32 Ovl11ProcessGridItemRemovalCommand(s16 mode, s16 suppressReset, s32 resultSlot);

#endif
