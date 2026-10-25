#ifndef OVL11_ITEM_TRANSFER_MENU_H
#define OVL11_ITEM_TRANSFER_MENU_H

#include "common.h"

/* Resident ovl_11 item-transfer UI. Requires initialized message service,
 * pad 0, sprite assets and active render context. Entries are six bytes:
 * item ID, two opaque detail bytes, and count. Slot 0/9 are special slots;
 * regular slots 1..8 and 10..17 are limited by the work-area capacity exponent
 * (expected 0..2). Indices, item classes and counts are not validated here.
 * Uses shared text/sprite state; not reentrant. Does not advance game time. */

/* Reset menu state/cursors, clear the picked source to FF. Does not close
 * a message window or alter any item record. Original: ovl_11:800FAAAC. */
void Ovl11ResetItemTransferMenu(void);

/* Open once, playing sound 3; otherwise leave the menu unchanged. The legacy
 * s32 ABI is retained: only the already-active path returns a defined state.
 * Callers must discard the result. Original: ovl_11:800FAAD4. */
s32 Ovl11OpenItemTransferMenu(void);

/* Service once per rendered frame. Draw the two item groups and cursor,
 * consume pad 0, inspect entries through archive 3, and perform validated
 * transfers. Empty destination moves, unlike IDs swap, equal class-2 items
 * merge to a maximum destination count of 99, retaining any remainder.
 * Close waits for message status 2, then resets the message service, invokes
 * the surrounding UI helper and sets work flags +C:40000 and +5234:2000.
 * Restores text style, but message/portrait and cursor state are stateful.
 * Original: ovl_11:800FAB18. */
void Ovl11UpdateItemTransferMenu(void);

#endif
