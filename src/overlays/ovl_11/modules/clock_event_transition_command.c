/*
 * Clock/event transition command, ovl_11:800E7798.
 * This adapter does not service the clock or wait for a transition.
 * Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_clock_event_transition_command.h"
#include "game_flags.h"
#include "ovl11_object_event_slot_flags_command.h"
#include "ovl11_clock_advance_command.h"
#include "ovl11_flag_group_command.h"

s32 Ovl11ClockEventTransitionCommand(s16 mode, s16 hours, s32 setWorkFlag,
                                    s32 controlFlags) {
    s32 clearEventFlags;

    switch (mode) {
    case 1: {
        char *farWork = (char *)&D_8007AFF0;

        (*(s32 *)(farWork + 0x254A0)) = 0xC;
        ((Ovl11ClockEventTransitionWork *)D_8006C838)->flags5234 |= 0x400;
        SetGameFlag(0x47U, 1U);
        break;
    }
    case 2:
        Ovl11ClockAdvanceCommand(hours, 0, 1, 0);
        break;
    case 3:
        Ovl11ClockAdvanceCommand(hours, 0, 0, 0);
        break;
    default:
        break;
    }
    clearEventFlags = controlFlags & 2;
    if (clearEventFlags != 0) {
        Ovl11ClearObjectEventSlotFlags(0x2A, 2, 0, 0);
    }
    /* The flag-group service ignores its second operand. */
    Ovl11SetFlagGroupMode(2, controlFlags & 1, 0, 0);
    if (setWorkFlag != 0) {
        ((Ovl11ClockEventTransitionWork *)D_8006C838)->flags4450 |= 8;
    }
    ((Ovl11ClockEventTransitionWork *)D_8006C838)->field7A74 = 2;
    return 1;
}
