/*
 * Clock threshold and 06:00 fade command, original ovl_11:800E7660.
 * Public operand, result and state-change contract:
 * ovl11_clock_threshold_command.h.
 *
 * Input hour and comparison hour remain distinct: minute-slot resolution sits
 * between their births, before the 06:00-based comparison is normalized.
 */
#include "common.h"
#include "ovl11_clock_threshold_command.h"
#include "screen_fade.h"
#include "game_flags.h"

s32 Ovl11ClockThresholdCommand(s32 targetHourOrFade, s32 targetMinute,
                               s32 operandFlags) {
    s16 targetHour;
    s16 minute;
    s16 comparisonHour;
    s16 currentHour;
    s32 reached;
    s32 result;

    currentHour = ((struct Ovl11StateInitView *)D_8006C838)->hour;
    targetHour = targetHourOrFade;
    minute = targetMinute;
    if (targetHour == -1) {
        if (currentHour == 6) {
            if (((struct Ovl11StateInitView *)D_8006C838)->minute == 0) {
                if (IsScreenFadeComplete() == 0) {
                    return 1;
                }
            }
        }
        return 0;
    }
    if (operandFlags & 1) {
        targetHour = *(s16 *)((u8 *)gOvl11ValueSlots + (targetHour * 4));
    }
    comparisonHour = targetHour;
    if (operandFlags & 2) {
        minute = *(s16 *)((u8 *)gOvl11ValueSlots + (minute * 4));
    }
    if (targetHour < 6) {
        comparisonHour = targetHour + 0x18;
    }
    if (currentHour < 6) {
        currentHour += 0x18;
    }
    reached = 0;
    if ((comparisonHour < currentHour) ||
        ((comparisonHour == currentHour) &&
         (((struct Ovl11StateInitView *)D_8006C838)->minute >= minute))) {
        reached = 1;
    }
    result = reached;
    if (reached == 1) {
        ((struct Ovl11PendingSelectionWorkView *)D_8006C838)->pendingRecord = -1;
        SetGameFlag(0x46U, 0U);
        result = reached;
    }
    return result;
}
