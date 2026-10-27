/*
 * action_result_override_command — reconstructed translation unit.
 *
 * Install or replace one action-result override in a five-entry
 * signed-halfword pair table.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_action_result_override_command.h"

s32 Ovl11SetActionResultOverrideCommand(s16 actionOrSlot, s16 result, s32 fromSlot) {
    s32 target;
    s32 *slot;
    s16 slotId;
    s32 i;

    if (fromSlot != 0) {
        target = gOvl11ValueSlots[actionOrSlot];
    } else {
        target = actionOrSlot;
    }
    slot = gActionResultOverrides;
    i = 0;
    do {
        slotId = *(s16 *)slot;
        /* A free entry wins immediately; only occupied entries advance i. */
        if (slotId == -1 || (i++, slotId == target)) {
            *(s16 *)slot = target;
            *((s16 *)slot + 1) = result;
            return 1;
        }
        slot++;
    } while (i < 5);
    return 0;
}
