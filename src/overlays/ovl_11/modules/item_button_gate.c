/*
 * item_button_gate — reconstructed translation unit.
 *
 * Primary action dispatch's input-bit/object-flag gate. The object supplies
 * eligibility, while Ovl11TryBeginItemAction operates on the shared primary
 * object. See ovl11_primary_action_helpers.h for the handled-input contract.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_item_action_gate.h"
#include "ovl11_primary_action_helpers.h"

s32 Ovl11TryItemActionButton(Ovl11C7010Object *object, u16 pressedButtons) {
    if (!(pressedButtons & 0x80)) {
        return 0;
    }
    if (!(object->unk6C & 0x10000000)) {
        return 0;
    }
    if (object->itemIndex != 0 && Ovl11TryBeginItemAction(1) != 0) {
        return 1;
    }
    return 0;
}
