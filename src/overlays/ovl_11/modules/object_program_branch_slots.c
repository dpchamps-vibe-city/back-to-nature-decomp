/*
 * Object-program branch targets exchanged with command value slots.
 * The two exclusive cursor-accessor clients form a read/write pair over
 * the same sixteen-byte program record. FF means sequential execution;
 * explicit branch targets are zero-based record indices.
 * Contract: ovl11_object_program_branch_slots.h.
 */
#include "common.h"
#include "ovl11_object_archive_pipeline.h"
#include "ovl11_object_program_branch_slots.h"

s32 Ovl11StoreProgramFailureTargetToSlot(s16 destinationSlot) {
    Ovl11ObjectProgramRecord *scan;
    s32 recordIndex;
    Ovl11ObjectProgramRecord *current;

    scan = gOvl11ObjectProgramRecords;
    current = (Ovl11ObjectProgramRecord *)Ovl11GetObjectProgramCursor();
    if (current->failureTarget == 0xFF) {
        recordIndex = 0;
        if (current != scan) {
            while (++recordIndex < 0xCE) {
                scan++;
                if (current == scan) {
                    break;
                }
            }
        }
        if (recordIndex == 0xCE) {
            return 0;
        }
        gOvl11ValueSlots[destinationSlot] = recordIndex + 1;
    } else {
        gOvl11ValueSlots[destinationSlot] = current->failureTarget;
    }
    return 1;
}

s32 Ovl11SetProgramSuccessTargetFromSlot(s16 sourceSlot) {
    Ovl11ObjectProgramRecord *current;

    current = (Ovl11ObjectProgramRecord *)Ovl11GetObjectProgramCursor();
    current->successTarget = ((u8 *)gOvl11ValueSlots)[sourceSlot * 4];
    return 1;
}
