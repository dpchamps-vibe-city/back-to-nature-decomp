#ifndef OVL11_ITEM_SLOT_SNAPSHOT_H
#define OVL11_ITEM_SLOT_SNAPSHOT_H

#include "common.h"

/* restore == 0 saves the six-byte held item and eight six-byte secondary
 * items in one private snapshot, then removes their slot sprites, clears
 * their live records and resets the primary-object request. Non-nestable:
 * another save overwrites the snapshot; restore requires a preceding save.
 *
 * Any nonzero restore reinstalls the secondary list and attempts all eight
 * sprite registrations. A nonempty saved held item first evicts the current
 * item (with special cleanup for IDs 106/107/65/64 hex). An EMPTY saved held
 * item leaves the current one intact. preferDirectRestore == 0, or saved
 * item-definition flag 100 hex, uses the request installer; otherwise copy
 * directly, register its sprite, set flags 10008000 and clear 4000 hex.
 *
 * Returns 1, not a completion/error result. Requires initialized item/effect
 * pointers, valid item IDs and sprite-cache capacity. Neither indices nor
 * installation failures are checked; snapshot bytes are retained on restore. */
s32 Ovl11SnapshotItemSlots(s16 restore, s16 preferDirectRestore);

#endif
