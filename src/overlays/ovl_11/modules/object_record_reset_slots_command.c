/* Resolve four optional command slots for the shared object-record reset API.
 * Membership/boundaries: notes/file-groupings.md. */

#include "common.h"
#include "ovl11_object_record_reset_command.h"
#include "ovl11_object_record_reset_slots_command.h"

s32 Ovl11ResetObjectRecordsFromSlotsCommand(s16 firstSlot, s16 secondSlot, s32 thirdSlot, s32 fourthSlot) {
    s16 firstId;
    s16 secondId;
    s32 thirdId;
    s32 fourthId;

    if (firstSlot != -1) {
        firstId = *((s16 *)&gOvl11ValueSlots[firstSlot]);
    } else {
        firstId = -1;
    }
    if (secondSlot != -1) {
        secondId = *((s16 *)&gOvl11ValueSlots[secondSlot]);
    } else {
        secondId = -1;
    }
    if (thirdSlot != -1) {
        thirdId = gOvl11ValueSlots[thirdSlot];
    } else {
        thirdId = -1;
    }
    if (fourthSlot != -1) {
        fourthId = gOvl11ValueSlots[fourthSlot];
    } else {
        fourthId = -1;
    }
    Ovl11ResetObjectRecordCommand(firstId, secondId, thirdId, fourthId);
    return 1;
}
