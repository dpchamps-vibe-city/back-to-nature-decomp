/*
 * pending_item_command — reconstructed translation unit.
 *
 * Test and destructively snapshot the pending-item latch, or query its shared
 * status bit through the command interface.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_pending_item_command.h"

s32 Ovl11ConsumePendingItemCommand(s16 mode, s16 propertyIndex, s32 resultSlot, s32 unused) {
    s32 result;

    result = 0;
    switch (mode) {
    case 0:
        if (((struct Ovl11E9914State *)D_8006C838)->pending != 0) {
            if (Ovl11GetItemPackedProperty(((struct Ovl11E9914State *)D_8006C838)->pending, propertyIndex) == 0) {
                result = 1;
            }
        }
        if (resultSlot != -1) {
            gOvl11ValueSlots[resultSlot] = ((struct Ovl11E9914State *)D_8006C838)->pending;
        }
        ((struct Ovl11E9914State *)D_8006C838)->pending = 0;
        break;
    case 1:
        if (D_80070C88 & 0x20) {
            result = 1;
        }
        break;
    }
    return result;
}
