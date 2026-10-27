/*
 * Object-program adapters for literal, descriptor-field and value-slot operands.
 * Comparisons return the rule result; updates return 1 to advance the program.
 * The shared value bank belongs to the engine, not this translation unit.
 */
#include "common.h"
#include "game_flags.h"
#include "ovl11_state_field_access.h"
#include "ovl11_state_value_operand_commands.h"

s32 Ovl11TestStateFieldAgainstLiteral(s16 fieldId, s16 elementIndex, s32 condition, s32 operand) {
    s32 value;

    value = Ovl11ReadStateField((s32) fieldId, (s32) elementIndex);
    /* This command alone suppresses field 14's non--1 tests under flag 1B. */
    if (fieldId == 0x14 && GetGameFlag(0x1BU) == 1 && operand != -1) {
        return 0;
    }
    return Ovl11TestStateFieldCondition(value, operand, condition);
}

s32 Ovl11TestStateFieldAgainstValueSlot(s16 fieldId, s16 elementIndex, s32 condition, s32 operandSlot) {
    return Ovl11TestStateFieldCondition(
        Ovl11ReadStateField(fieldId, elementIndex),
        gOvl11ValueSlots[operandSlot], condition);
}

s32 Ovl11TestValueSlotAgainstLiteral(s32 valueSlot, s32 unused, s32 condition, s32 operand) {
    return Ovl11TestStateFieldCondition(gOvl11ValueSlots[(s16)valueSlot], operand, condition);
}

s32 Ovl11TestValueSlotAgainstValueSlot(s16 valueSlot, s16 operandSlot, s32 condition) {
    return Ovl11TestStateFieldCondition(gOvl11ValueSlots[valueSlot], gOvl11ValueSlots[operandSlot], condition);
}

s32 Ovl11TestStateFieldWithinLiteralRange(s16 fieldId, s16 elementIndex, s32 lower, s32 upper) {
    s32 value;
    s32 inRange;

    inRange = 0;
    value = Ovl11ReadStateField((s32) fieldId, (s32) elementIndex);
    if (value >= lower) {
        inRange = upper >= value;
    }
    return inRange;
}

s32 Ovl11TestValueSlotWithinSelectedRange(s16 slotMask, s32 valueSlot, s32 lower, s32 upper) {
    s32 value;
    s32 lowerBound;
    s32 upperBound;

    value = gOvl11ValueSlots[(s16)valueSlot];
    if (slotMask & 1) {
        lowerBound = gOvl11ValueSlots[lower];
    } else {
        lowerBound = lower;
    }
    if (slotMask & 2) {
        upperBound = gOvl11ValueSlots[upper];
    } else {
        upperBound = upper;
    }
    return value >= lowerBound && upperBound >= value;
}

s32 Ovl11UpdateStateFieldFromLiteral(s16 fieldId, s16 elementIndex, s32 operand, s32 operation) {
    Ovl11UpdateStateField(fieldId, elementIndex, operand, operation);
    return 1;
}

s32 Ovl11UpdateStateFieldFromValueSlot(s16 fieldId, s16 elementIndex, s32 operandSlot, s32 operation) {
    Ovl11UpdateStateField(fieldId, elementIndex, gOvl11ValueSlots[operandSlot], operation);
    return 1;
}

s32 Ovl11ApplyValueSlotOperation(s16 destinationSlot, s16 operandIsSlot, s32 operand, s32 operation) {
    s32 *destination;
    s32 resolvedOperand;

    destination = &gOvl11ValueSlots[destinationSlot];
    if (operandIsSlot != 0) {
        resolvedOperand = gOvl11ValueSlots[operand];
    } else {
        resolvedOperand = operand;
    }
    Ovl11ApplyStateFieldOperation(destination, resolvedOperand, operation);
    return 1;
}

s32 Ovl11ApplyStateFieldToValueSlot(s16 fieldId, s16 elementIndex, s32 destinationSlot, s32 operation) {
    Ovl11ApplyStateFieldOperation(&gOvl11ValueSlots[destinationSlot],
        Ovl11ReadStateField(fieldId, elementIndex), operation);
    return 1;
}
