/*
 * random_value_slot_command — reconstructed translation unit.
 *
 * Generate one bounded random value, optionally store it to an interpreter
 * value slot, then invoke the shared comparison operation.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_random_value_slot_command.h"
#include "random.h"
#include "ovl11_state_field_access.h"

s32 Ovl11RandomValueSlotCommand(s32 limit, s16 slot, s32 condition, s32 operand) {
    s32 value;

    value = Rand(limit & 0xFFFF);
    if (slot != -1) {
        gOvl11ValueSlots[slot] = value;
    }
    return Ovl11TestStateFieldCondition(value, operand, condition);
}
