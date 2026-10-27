/*
 * Object-program message command (ovl_11:800E58DC). The private remap table
 * adapts nine entries in one work mode; the window and value slots remain
 * shared external services. See ovl11_message_command.h and file-groupings.md.
 */
#include "common.h"
#include "ovl11_message_command.h"
#include "cd_audio.h"
#include "message_window.h"

s32 Ovl11StepMessageCommand(s16 archive, s16 entryOrSlot, s32 resultSlot, s32 entryIsSlot) {
    s32 entry;
    u32 remapIndex;
    s32 complete;
    char *workUpper;

    complete = 0;
    if (entryIsSlot != 0) {
        entry = gOvl11ValueSlots[entryOrSlot];
    } else {
        entry = entryOrSlot;
    }
    workUpper = (char *)&D_8006C838;
    workUpper += 0x8000;
    if ((*(s16 *)(workUpper + 0x64C8) == 2) && (archive == 0)) {
        for (remapIndex = 0; remapIndex < 9; remapIndex++) {
            if (entry == gOvl11MessageRemapEntries[remapIndex]) {
                entry = remapIndex + 0x731;
                break;
            }
        }
    }
    if (GetCdAudioState() == 0) {
        RequestMessageWindow(archive, entry);
        if (GetMessageWindowStatus() == 2) {
            complete = 1;
            if (resultSlot != -1) {
                gOvl11ValueSlots[resultSlot] = GetMessageWindowChoice();
            }
        }
    }
    return complete;
}
