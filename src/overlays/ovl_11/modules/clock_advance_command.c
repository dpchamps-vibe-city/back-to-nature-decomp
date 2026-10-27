/* Resolve command time operands and request a clock-minute advance.
 * Membership/boundaries: notes/file-groupings.md. */

#include "common.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_clock_advance_command.h"

s32 Ovl11ClockAdvanceCommand(s16 hoursOrSlot, s16 minutesOrSlot, s32 relative, s32 slotMask) {
    s32 minutes;
    s32 hours;
    s16 targetHour;
    s32 delta;
    s32 result;

    if (slotMask & 1) {
        hours = *(s16 *)((u8 *)gOvl11ValueSlots + (hoursOrSlot * 4));
        targetHour = hours;
    } else {
        hours = hoursOrSlot;
        targetHour = hours;
    }
    if (slotMask & 2) {
        minutes = *(s16 *)((u8 *)gOvl11ValueSlots + (minutesOrSlot * 4));
    } else {
        minutes = minutesOrSlot;
    }
    if (relative != 0) {
        Ovl11RequestClockAdvance((hours * 0x3C) + minutes);
        return 1;
    }
    if (((struct struct_8006C838_time *)D_8006C838)->field_44C0 < 6) {
        if (hours >= 6) {
            return 0;
        }
    } else if (hours < 6) {
        targetHour = hours + 0x18;
    }
    delta = ((targetHour - ((struct struct_8006C838_time *)D_8006C838)->field_44C0) * 0x3C) + (minutes - ((struct struct_8006C838_time *)D_8006C838)->field_44C2);
    if (delta < 0) {
        result = 0;
    } else {
        Ovl11RequestClockAdvance(delta);
        result = 1;
    }
    return result;
}
