/*
 * status_text_command — reconstructed translation unit.
 *
 * Service cancelable or required status-text input, copy the shared 18-byte
 * scratch to selected status destinations, and publish cancellation to a
 * value slot.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_status_text_command.h"
#include "game_types.h"
#include "text_entry.h"

s32 Ovl11StatusTextCommand(s16 target, s16 requireInput, s32 resultSlot) {
    s32 result;
    s32 textEntryStatus;

    result = 0;
    textEntryStatus = UpdateTextEntry();
    switch (textEntryStatus) {
    case -1:
        if (resultSlot != textEntryStatus) {
            gOvl11ValueSlots[resultSlot] = 1;
        }
        result = 1;
        break;
    case 1:
        if (resultSlot != -1) {
            gOvl11ValueSlots[resultSlot] = 0;
        }
        switch (target) {
        case 0:
            *(Ovl11StatusTextRecord *)&D_80075AD8 = gOvl11StatusTextScratch;
            break;
        case 4:
            *(Ovl11StatusTextRecord *)&D_8007AFBC = gOvl11StatusTextScratch;
            break;
        }
        result = 1;
        break;
    case 0:
        switch (target) {
        case 0:
        case 4:
            BeginRequiredTextEntry((u16 *)&gOvl11StatusTextScratch);
            break;
        case 1:
        case 2:
        case 3:
            if (requireInput == 0) {
                BeginTextEntry((u16 *)&gOvl11StatusTextScratch);
            } else {
                BeginRequiredTextEntry((u16 *)&gOvl11StatusTextScratch);
            }
            break;
        }
        break;
    }
    return result;
}
