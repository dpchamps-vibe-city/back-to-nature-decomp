/* ovl_11:800EC304, object-opcode adapter for record_approach_sequence.
 * phaseSlot is narrowed to s16 before indexing. Target slots use full s32
 * indices and supply full-word raw X/Z coordinates. The phase slot is updated
 * in place; return 1 only when the ten-phase sequence finishes. All slots and
 * recordId are unchecked. Retains external opcode linkage; no direct C client.
 * Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_record_approach_sequence.h"

s32 Ovl11StepRecordApproachCommand(s16 phaseSlot, s16 recordId,
                                  s32 targetXSlot, s32 targetZSlot) {
    return Ovl11StepRecordApproachSequence(recordId, gOvl11ValueSlots[targetXSlot],
                                         gOvl11ValueSlots[targetZSlot],
                                         (u32 *) &gOvl11ValueSlots[(s16) phaseSlot]);
}
