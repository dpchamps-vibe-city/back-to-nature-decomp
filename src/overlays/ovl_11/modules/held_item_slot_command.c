/*
 * held_item_slot_command — reconstructed translation unit.
 *
 * Resolve an optional value-slot item ID and replace the primary object held
 * item using the published presentation service.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_held_item_replacement.h"
#include "ovl11_held_item_slot_command.h"

s32 Ovl11ReplaceHeldItemCommand(s16 itemOrSlot, s16 fromSlot, s32 presentation) {
    s32 value;
    Ovl11C7010Object *object = (Ovl11C7010Object *)&gPrimaryObjectBase;

    if (fromSlot != 0) {
        value = gOvl11ValueSlots[itemOrSlot];
    } else {
        value = itemOrSlot;
    }
    Ovl11ReplaceHeldItem(object, value, presentation);
    return 1;
}
