/* Condition evaluation's state-field read adapter (ovl_11:800EEBF4).
 * Keep both inputs: the condition-range caller supplies field ID and element
 * index in a0/a1, and this entry forwards both registers to the shared reader.
 * The reader owns descriptor/flag decoding; this adapter adds no conversion
 * or state. Membership/boundaries: notes/file-groupings.md. */
#include "ovl11_condition_field_read.h"
#include "ovl11_state_field_access.h"

s32 Ovl11ReadConditionField(s32 fieldId, s32 elementIndex) {
    s32 value;

    value = Ovl11ReadStateField(fieldId, elementIndex);
    return value;
}
