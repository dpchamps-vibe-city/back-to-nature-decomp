#ifndef OVL11_STATE_VALUE_OPERAND_COMMANDS_H
#define OVL11_STATE_VALUE_OPERAND_COMMANDS_H

#include "common.h"

/* ovl_11 object-program operand commands, original entries E6744..E6A50.
 * The interpreter supplies four operands; signatures below retain each
 * entry's actual widths and omit the final unused operand of the slot/slot
 * test. All indices are unchecked. Value slots hold signed 32-bit values;
 * these commands do not own or initialize the shared twenty-word bank.
 * Descriptor field IDs/element indices follow ovl11_state_field_access.h.
 *
 * Conditions: 0 >=, 1 >, 2 <=, 3 <, 4 ==; unknown codes return true.
 * Predicates return 0/1, which the interpreter uses to hold/advance the PC.
 * The literal state-field test reads the field first, then forces false
 * for field 14 (hex), game flag 1B == 1 and operand != -1. Its slot-sourced
 * counterpart deliberately has no such suppression.
 * The literal value-slot test ignores operand 1 and narrows valueSlot to
 * s16. The slot/slot test consumes the incoming third operand as condition. */
s32 Ovl11TestStateFieldAgainstLiteral(s16 fieldId, s16 elementIndex, s32 condition, s32 operand);
s32 Ovl11TestStateFieldAgainstValueSlot(s16 fieldId, s16 elementIndex, s32 condition, s32 operandSlot);
s32 Ovl11TestValueSlotAgainstLiteral(s32 valueSlot, s32 unused, s32 condition, s32 operand);
s32 Ovl11TestValueSlotAgainstValueSlot(s16 valueSlot, s16 operandSlot, s32 condition);

/* Inclusive ranges. slotMask bit 0 resolves lower through the value bank;
 * bit 1 resolves upper; other bits are ignored. valueSlot narrows to s16,
 * but selected bound-slot indices retain all 32 bits. */
s32 Ovl11TestStateFieldWithinLiteralRange(s16 fieldId, s16 elementIndex, s32 lower, s32 upper);
s32 Ovl11TestValueSlotWithinSelectedRange(s16 slotMask, s32 valueSlot, s32 lower, s32 upper);

/* Field updates delegate descriptor masks, clamping and width truncation to
 * Ovl11UpdateStateField. Slot operations use 0 assign, 1 add, 2 subtract,
 * 3 multiply, 4 divide, 5 modulo; zero divisors/unknown operations do nothing.
 * operandIsSlot treats any nonzero value as slot mode. Arithmetic is not
 * saturating and callers must avoid signed overflow. All four return 1,
 * including no-ops; aliased source/destination slots are allowed. */
s32 Ovl11UpdateStateFieldFromLiteral(s16 fieldId, s16 elementIndex, s32 operand, s32 operation);
s32 Ovl11UpdateStateFieldFromValueSlot(s16 fieldId, s16 elementIndex, s32 operandSlot, s32 operation);
s32 Ovl11ApplyValueSlotOperation(s16 destinationSlot, s16 operandIsSlot, s32 operand, s32 operation);
s32 Ovl11ApplyStateFieldToValueSlot(s16 fieldId, s16 elementIndex, s32 destinationSlot, s32 operation);

#endif
