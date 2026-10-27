/* Complementary object-program commands over one addressing protocol:
 * fieldId selects the state field; the selected value slot supplies its
 * ELEMENT INDEX, not the field value. Slots/indices are unchecked and -1
 * is not a discard sentinel here. Both functions retain external linkage
 * for the opcode table; there are no direct C clients to publish a header to.
 * See ovl11_state_field_access.h for descriptor, condition and operation codes.
 * Membership and the outer protocol cuts are recorded in notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_state_field_access.h"

/* The full word slot argument is narrowed BEFORE indexing. Conditions 0..4
 * mean >=, >, <=, <, ==; other codes return true. A read is not a mutation. */
s32 Ovl11TestStateFieldFromSlot(s16 fieldId, s32 slot, s32 condition, s32 operand) {
    return Ovl11TestStateFieldCondition(Ovl11ReadStateField(fieldId, gOvl11ValueSlots[(s16)slot]), operand, condition);
}

/* Update the resolved element, respecting descriptor masking/clamping.
 * Return 1 even for a no-op operation; this is the interpreter success result. */
s32 Ovl11UpdateStateFieldFromSlot(s16 fieldId, s16 slot, s32 operand, s32 operation) {
    Ovl11UpdateStateField(fieldId, gOvl11ValueSlots[slot], operand, operation);
    return 1;
}
