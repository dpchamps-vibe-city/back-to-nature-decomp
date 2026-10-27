/* Held-item flag synchronization, separate from sprite binding and transfer.
 * Original entry: ovl_11:800CCF58. Boundary evidence: file-groupings.md. */
#include "ovl11_held_item_flags.h"
#include "ovl11_primary_object_requests.h"

void Ovl11SyncHeldItemFlags(Ovl11C7010Object *object) {
    s16 itemIndex;
    s32 flags;
    s32 *flagWord;

    itemIndex = object->itemIndex;
    if (itemIndex == 0) {
        object->unk6C &= 0xE7FF7FFF;
        Ovl11ResetPrimaryObjectRequest();
    } else {
        flagWord = &object->unk6C;
        flags = *flagWord & 0xE7FFFFFF;
        *flagWord = flags;
        if (gItemDataTable[itemIndex].u0.flags & 0x180) {
            object->unk6C = flags | 0x08000000;
        } else {
            object->unk6C = flags | 0x10000000;
        }
        object->unk6C |= 0x8000;
    }
}
