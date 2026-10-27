/*
 * condition_code_command — reconstructed translation unit.
 *
 * Resolve a literal or value-slot condition code, enqueue its boolean payload
 * and forward the command control flags.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_condition_evaluation.h"
#include "ovl11_condition_code_command.h"
#include "ovl11_clock_event_transition_command.h"

s32 Ovl11QueueConditionCodeCommand(s16 codeOrSlot, s16 controlFlags, s32 fromSlot, s32 payloadMode) {
    s32 code;

    code = codeOrSlot;
    if (fromSlot != 0) {
        code = gOvl11ValueSlots[code];
    }
    Ovl11QueueConditionCodeAnySlot(code & 0xFFFF, payloadMode == 0);
    Ovl11ClockEventTransitionCommand(0, 0, 0, controlFlags);
    return 1;
}
