#ifndef OVL11_HELD_ITEM_SLOT_COMMAND_H
#define OVL11_HELD_ITEM_SLOT_COMMAND_H

#include "common.h"

/* ovl_11 primary held-item adapter. With fromSlot != 0, read the full s32
 * item ID from the unchecked signed itemOrSlot value-bank index; otherwise
 * use itemOrSlot literally. Forward it and presentation to the shared held-
 * item replacement service (snapshot, sprite and object-state effects are
 * documented in ovl11_held_item_replacement.h). Always returns 1, irrespective
 * of whether presentation installs the staged item into the held slot.
 * Original identity: ovl_11:800E932C. */
s32 Ovl11ReplaceHeldItemCommand(s16 itemOrSlot, s16 fromSlot, s32 presentation);

#endif
