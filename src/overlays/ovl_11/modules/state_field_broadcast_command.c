/*
 * state_field_broadcast_command — update flagged event records' state fields.
 * Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_state_field_access.h"
#include "ovl11_object_events.h"
#include "ovl11_state_field_broadcast_command.h"

s32 Ovl11BroadcastStateFieldCommand(s16 fieldId, s16 operation, s32 operand, s32 specialOperand) {
    s32 slot;
    s32 nextSlotFixed;
    s32 next;
    u8 *recordWalk;
    u8 *base;
    s32 splitField;
    s32 fieldOffset;

    slot = 1;
    fieldOffset = fieldId - 0x1F;
    splitField = (u32) fieldOffset < 2U;
    nextSlotFixed = 0x20000;
    base = (u8 *) D_8006C838;
    recordWalk = base + 0x1D4;
    /* The separate Q16 next-index accumulator preserves the record walk. */
    for (; slot < 0x25; slot = next >> 0x10) {
        if ((*(u16 *) (recordWalk + 0x9A06)) & 0x800) {
            if (splitField) {
                if (Ovl11IsSpecialCounterSlot(slot) == 1) {
                    Ovl11UpdateStateField(0x1F, slot, specialOperand, (s32) operation);
                } else {
                    Ovl11UpdateStateField(0x20, slot, operand, (s32) operation);
                }
            } else {
                Ovl11UpdateStateField((s32) fieldId, slot, operand, (s32) operation);
            }
        }
        next = nextSlotFixed;
        nextSlotFixed += 0x10000;
        recordWalk = recordWalk + 0x1D4;
    }
    return 1;
}
