/*
 * record_family_list_command — reconstructed translation unit.
 *
 * Adapt the record-family browser to command result slots and success versus
 * cancellation codes.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_record_family_list.h"
#include "ovl11_record_family_list_command.h"

s32 Ovl11RunRecordFamilyListCommand(s16 familyKind, s16 interactive, s32 resultSlot) {
    s16 browserMode;
    s32 selection;

    browserMode = 0;
    if (interactive == 0) {
        browserMode = -1;
    }
    selection = Ovl11DrawAndProcessRecordFamilyList(familyKind, 1, browserMode);
    gOvl11ValueSlots[resultSlot] = selection;
    if (selection == -2) {
        return 1;
    }
    return selection != -1;
}
