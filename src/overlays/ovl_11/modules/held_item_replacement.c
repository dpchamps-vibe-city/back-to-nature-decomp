/*
 * Held-item replacement and release, including sprite-slot lifetime.
 * Replacement snapshots the old +8A triplet for the separate state-31
 * cleanup path; release optionally hands a placed item back to its record.
 * See ovl11_held_item_replacement.h for presentation and indexing contracts.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_held_item_replacement.h"
#include "sound_commands.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_record_placement.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_primary_object_sprite_slots.h"

void Ovl11ReplaceHeldItem(Ovl11C7010Object *object, s32 itemId, s32 presentation) {
    u16 replacement[3];
    s16 *pendingItem;
    s16 *heldItem;
    s16 oldId;
    s32 clearedFlags;
    s32 selectedPresentation;
    u16 itemFlags;

    selectedPresentation = presentation;
    replacement[0] = itemId;
    replacement[1] = 0;
    replacement[2] = 1;
    Ovl11ChangePrimaryObjectState(object, 3U, 5, -2);
    gOvl11SavedHeldItem = *(Ovl11ItemState *)((u8 *)object + 0x8A);
    oldId = object->itemIndex;
    heldItem = &object->itemIndex;
    if (oldId != 0) {
        Ovl11RemoveSlotSprite(oldId);
    }
    Ovl11ClearHalfwordTriplet(heldItem);
    pendingItem = (s16 *)((u8 *)object + 0xF0);
    Ovl11ClearHalfwordTriplet(pendingItem);
    Ovl11CopyHalfwordTriplet((u16 *)pendingItem, replacement);
    Ovl11InstallSlotSprite(*(s16 *)((u8 *)object + 0xF0));
    clearedFlags = object->unk6C & ~0x4000;
    object->unk6C = clearedFlags;
    itemFlags = ((Ovl11CBB9CGlobals *)D_8006C838)->items[*(s16 *)((u8 *)object + 0xF0)].u0.flags;
    if ((itemFlags & 0x1200) == 0x1200) {
        selectedPresentation = 2;
    }
    if ((itemFlags & 0x1600) == 0x1600) {
        selectedPresentation = 3;
    }
    switch (selectedPresentation) {
    case 0:
    case 1:
        Ovl11CopyHalfwordTriplet((u16 *)heldItem, (u16 *)pendingItem);
        Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)((Ovl11CBB9CGlobals *)D_8006C838)->object, 0x30U, ((Ovl11CBB9CGlobals *)D_8006C838)->state + 0x21, -2);
        return;
    case 2:
        object->unk6C = clearedFlags | 0x8000;
        Ovl11CopyHalfwordTriplet((u16 *)heldItem, (u16 *)pendingItem);
        Ovl11ChangePrimaryObjectState(object, 0x31U, 5, -2);
        PlaySoundEffect(0x4A);
        return;
    case 3:
        Ovl11CopyHalfwordTriplet((u16 *)heldItem, (u16 *)pendingItem);
        object->unk6C |= 0x5800;
        Ovl11ChangePrimaryObjectState(object, 0x4CU, 0, -2);
        PlaySoundEffect(0x4B);
        return;
    }
}

void Ovl11ReleaseHeldItem(Ovl11C7010Object *object) {
    VECTOR position;
    s16 itemId;

    itemId = object->itemIndex;
    if (gItemDataTable[itemId].u0.flags & 0x100) {
        Ovl11ComputeGridOffsetPosition((Ovl11CE034Object *) object, (Ovl11CE034Position *) &position, 2);
        Ovl11RefreshRecordById(itemId, (Ovl25SetupStatePair *) ((u8 *) object + 0xFC), &position);
    }
    Ovl11RemoveSlotSprite(itemId);
    Ovl11ClearHalfwordTriplet(&object->itemIndex);
}
