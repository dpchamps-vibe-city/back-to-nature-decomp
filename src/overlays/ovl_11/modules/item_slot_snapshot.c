/*
 * One saved held-item record and one eight-item secondary-list snapshot.
 * Saving detaches their sprite registrations; restoration reinstalls them
 * and handles the displaced held item's effect-specific cleanup.
 * See ovl11_item_slot_snapshot.h for the asymmetric restore rules.
 */
#include "common.h"
#include "ovl11_item_slot_snapshot.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_slot_sprite_cache.h"

void ovl_11_func_8010941C(s32 arg0);

s32 Ovl11SnapshotItemSlots(s16 restore, s16 preferDirectRestore) {
    s16 *slot;
    s32 slotIndex;
    u16 replacedId;

    if (restore == 0) {
        Ovl11CopyHalfwordTriplet((u16 *)&gOvl11SnapshotHeldItem, (u16 *)&Ovl11Item_80071A8A);
        *(Ovl11ItemSnapshot *)gOvl11SnapshotSecondaryItems = *(Ovl11ItemSnapshot *)((u8 *)&Ovl11Item_80071A8A + 0x36);
        if (((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem.id != 0) {
            ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->flags &= 0xE7FF7FFF;
            Ovl11RemoveSlotSprite(((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem.id);
            Ovl11ClearHalfwordTriplet((s16 *)&((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem);
            Ovl11ResetPrimaryObjectRequest();
        }
        slot = (s16 *)&((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->secondaryItems;
        for (slotIndex = 0; slotIndex < 8; slotIndex++) {
            Ovl11RemoveSlotSprite(*slot);
            Ovl11ClearHalfwordTriplet(slot);
            slot += 3;
        }
        Ovl11ResetPrimaryObjectRequest();
    } else {
        replacedId = ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem.id;
        /* An empty saved held item does NOT evict the current held item. */
        if (gOvl11SnapshotHeldItem.id != 0) {
            if (replacedId != 0) {
                ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->flags &= 0xE7FF7FFF;
                Ovl11RemoveSlotSprite(((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem.id);
                Ovl11ClearHalfwordTriplet((s16 *)&((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem);
                Ovl11ResetPrimaryObjectRequest();
                switch (replacedId) {
                case 0x106:
                case 0x107:
                    ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->effectFlags &= 0xFFFDFFFF;
                    ovl_11_func_8010941C(0);
                    break;
                case 0x65:
                    ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->effectHalfwords[0xE] |= 1;
                    Ovl11PublishNextPoolRecord();
                    break;
                case 0x64:
                    ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->effectHalfwords[2] |= 1;
                    Ovl11PublishCurrentPoolRecord();
                    break;
                }
            }
            if ((preferDirectRestore == 0) || (((ItemData *)((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->itemDefinitions)[gOvl11SnapshotHeldItem.id].u0.flags & 0x100)) {
                Ovl11InstallPrimaryObjectRequest((u16 *)&gOvl11SnapshotHeldItem);
            } else {
                ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem = gOvl11SnapshotHeldItem;
                Ovl11InstallSlotSprite(((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->heldItem.id);
                ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->flags = (((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->flags | 0x10008000) & ~0x4000;
                Ovl11ResetPrimaryObjectRequest();
            }
        }
        gPrimaryObjectSecondaryItemList = *(Ovl11ItemSnapshot *)gOvl11SnapshotSecondaryItems;
        slot = (s16 *)&gPrimaryObjectSecondaryItemList;
        for (slotIndex = 0; slotIndex < 8;) {
            Ovl11InstallSlotSprite(*slot);
            slotIndex++;
            slot += 3;
        }
    }
    return 1;
}
