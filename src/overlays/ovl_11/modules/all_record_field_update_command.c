/*
 * Bulk state-field command, ovl_11:800EBF18 (opcode entry).
 * Update IDs 1..36 except 27/33. For field 1F or 20, route special counter
 * slots to field 1F/specialOperand and others to field 20/operand, then update
 * field 2B for elements 0..6 with operand. Other fields use operand for all
 * eligible IDs and omit the secondary pass. operation is the state-field
 * service's raw operation code. Requires valid initialized descriptors/work;
 * there is no per-record flag filter and no rollback on a failed update.
 * Return 1 regardless of helper results. No C callers.
 * Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_state_field_access.h"
#include "ovl11_object_events.h"

s32 Ovl11UpdateAllRecordFieldsCommand(s16 fieldId, s16 operation,
                                     s32 operand, s32 specialOperand) {
    s16 recordId;

    for (recordId = 1; recordId < 0x25; recordId++) {
        if (recordId != 0x1B && recordId != 0x21) {
            if ((u32)(fieldId - 0x1F) < 2U) {
                if (Ovl11IsSpecialCounterSlot(recordId) == 1) {
                    Ovl11UpdateStateField(0x1F, recordId, specialOperand, operation);
                } else {
                    Ovl11UpdateStateField(0x20, recordId, operand, operation);
                }
            } else {
                Ovl11UpdateStateField(fieldId, recordId, operand, operation);
            }
        }
    }
    /* Preserve the original narrow comparison in this second guard. */
    if ((u16)(fieldId - 0x1F) < 2U) {
        for (recordId = 0; recordId < 7; recordId++) {
            Ovl11UpdateStateField(0x2B, recordId, operand, operation);
        }
    }
    return 1;
}
