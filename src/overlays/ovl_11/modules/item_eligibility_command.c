/*
 * item_eligibility_command — reconstructed translation unit.
 *
 * Test a literal or value-slot item against the A4 special case and the
 * packed-property eligibility rule.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_item_eligibility_command.h"

s32 Ovl11TestItemEligibilityCommand(s16 itemOrSlot, s16 fromSlot) {
    ItemData *item;
    u32 flags;
    s16 itemId;

    if (fromSlot != 0) {
        itemId = *(s16 *)&gOvl11ValueSlots[itemOrSlot];
    } else {
        itemId = itemOrSlot;
    }
    if (itemId == 0xA4) {
        return 1;
    }
    item = gItemDataTable;
    item += itemId;
    flags = item->u0.flags;
    flags &= 0x1C00;
    if (flags) {
        flags = Ovl11GetItemPackedProperty(itemId, 3);
        if ((u16)flags < 3) {
            return 1;
        }
    }
    return 0;
}
