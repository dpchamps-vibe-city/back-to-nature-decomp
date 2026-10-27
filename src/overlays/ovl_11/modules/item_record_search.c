/* Item-record search/removal across the held, primary and overflow banks,
 * ovl_11:800EEC1C–800EEECC. Counting is by records, not stack quantities.
 * The item command, progress and rule clients share this service; none of
 * the neighboring state-field adapters or scheduled-date helpers uses its
 * item-bank traversal. No storage or private linkage is introduced here.
 * Contracts and unusual removal behavior: ovl11_item_record_search.h.
 * Membership/boundary evidence: notes/file-groupings.md.
 */
#include "ovl11_item_record_search.h"
#include "game_types.h"
#include "integer_power.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_held_item_replacement.h"

s32 Ovl11CountOrRemoveItemRecords(s16 itemId, s16 bankSelector,
                                 s16 lastGroup, s32 removeCount) {
    s32 key;
    s32 endGroup;
    s32 first;
    s32 group;
    s32 index;
    s32 recordCount;
    s16 found;
    Ovl11ItemState *records;
    Ovl11ItemState *record;

    key = itemId;
    endGroup = lastGroup;
    records = 0;
    recordCount = 0;
    found = 0;
    if (endGroup == -1) {
        first = 1;
        endGroup = 1;
    } else {
        first = 0;
    }
    for (group = first; group <= endGroup; group++) {
        switch (group) {
        case 0:
            records = (Ovl11ItemState *)&D_80071A84 + bankSelector;
            recordCount = 1;
            break;
        case 1:
            if (bankSelector == 0) {
                records = (Ovl11ItemState *)&gItemStackSlotBase;
            } else {
                records = (Ovl11ItemState *)&gPrimaryObjectSecondaryItemList;
            }
            recordCount = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
            break;
        case 2:
            if (bankSelector == 0) {
                records = (Ovl11ItemState *)&gItemOverflowSlotBase;
                recordCount = 64;
            } else {
                records = &D_80070D42_slots[0][0];
                recordCount = 64;
            }
            break;
        case 3:
            if (bankSelector == 0) {
                records = D_80071042;
                recordCount = 64;
            } else {
                records = 0;
                recordCount = 0;
            }
            break;
        }
        /* lastGroup == -1 selects only group 1 AND skips its first record. */
        for (index = first; index < recordCount; index++) {
            record = records + index;
            if (record->id == key) {
                if (removeCount != 0) {
                    switch (group) {
                    case 0:
                        if (bankSelector != 0) {
                            ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->flags &= 0xEFFF7FFF;
                            Ovl11ResetPrimaryObjectRequest();
                            Ovl11ReleaseHeldItem((Ovl11C7010Object *)((u8 *)D_8006C838 + 0x51C8));
                        } else {
                            Ovl11RemoveSlotSprite(record->id);
                            Ovl11ClearHalfwordTriplet((s16 *)record);
                        }
                        /* Intentional fallthrough: group 0 also runs the
                         * group-1 sprite removal/clear, even after clearing. */
                    case 1:
                        Ovl11RemoveSlotSprite(record->id);
                        Ovl11ClearHalfwordTriplet((s16 *)record);
                        break;
                    case 2:
                    case 3:
                        Ovl11ClearHalfwordTriplet((s16 *)record);
                        break;
                    }
                    removeCount--;
                }
                found++;
            }
        }
    }
    return found;
}
