#ifndef OVL11_ITEM_SLOT_COMMANDS_H
#define OVL11_ITEM_SLOT_COMMANDS_H

#include "common.h"

/* Four-operand command ABI; unused trailing operands are retained for the
 * overlay's dispatch table. Slot indices, item IDs and capacity exponent are
 * unchecked. The grid capacity is 2^(work.exponent+1); callers must keep it
 * within the backing grid. Return values are boolean s32, not match counts.
 * All storage and sprite/request services must already be initialized.
 */

/* Match id 0 (an empty slot), without removal. which selects the primary (0)
 * or secondary (1) slot set. lastGroup selects the last searched group:
 * 0 = single slot, 1 = grid, 2 = overflow, 3 = additional storage.
 * -1 searches only the grid, retaining the backend's skip of grid index 0.
 */
s32 Ovl11HasEmptyItemSlot(s16 which, s16 lastGroup, s16 unused2, s16 unused3);

/* flags bit 1 reads the signed low halfword of valueSlots[itemOrSlot]; bit 0
 * requests removal of one match through the shared inventory backend (including
 * its sprite/request cleanup). Other flag bits are ignored. which/lastGroup
 * use that backend's group protocol as described above.
 */
s32 Ovl11MatchItemSlotsCommand(s16 itemOrSlot, s16 which, s32 lastGroup, s32 flags);

/* Install {id,0,1} in the secondary request (useGrid==0) or its first empty
 * grid slot. Nonzero fromValueSlot reads the unsigned low halfword of the
 * selected value slot. Return 0 when the preliminary empty-slot check fails,
 * otherwise 1. The grid path also installs the slot sprite.
 */
s32 Ovl11InsertSecondaryItemCommand(s16 itemOrSlot, s16 fromValueSlot,
                                   s32 useGrid, s32 unused);

/* Choose the first empty held, grid, then 64-entry overflow slot. Nonzero
 * fromValueSlot reads a SIGNED low halfword. excludeOverflowCheck changes only
 * the preliminary capacity query: the later overflow scan is still present.
 * Store count 1; IDs 1..25 get variant (id-1)%5, all others variant 0.
 * Non-overflow insertion installs a sprite. feedbackMode 1 requests a 10-tick
 * transfer, other nonzero modes 60 ticks; zero suppresses feedback. Overflow
 * insertion does neither. Return 0 on rejection/no slot, 1 on insertion.
 */
s32 Ovl11InsertHeldGridOrOverflowItem(s16 itemOrSlot, s16 fromValueSlot,
                                     s32 excludeOverflowCheck, s32 feedbackMode);

#endif
