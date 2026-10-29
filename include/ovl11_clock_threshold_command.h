#ifndef OVL11_CLOCK_THRESHOLD_COMMAND_H
#define OVL11_CLOCK_THRESHOLD_COMMAND_H

#include "common.h"

/* Clock callback with signed-halfword operands carried in the existing s32
 * ABI. Bits 0/1 of operandFlags resolve hour/minute from four-byte value slots
 * using signed halfword loads; indices are unchecked. Each comparison hour
 * below 6 has 24 added once, giving the normal clock its 06:00 day boundary.
 * The minute operand is compared directly, without range normalization.
 *
 * Return 1 when the current time has reached the threshold, then set the
 * pending-selection halfword to -1 and clear game flag 46; otherwise return 0.
 * Repeated successful calls repeat those writes.
 *
 * A raw hour operand whose low halfword is -1 selects a DIFFERENT operation
 * before slot resolution: return 1 only at exactly 06:00 with an incomplete
 * screen fade. That path never clears the selection or flag. This is not a
 * blocking wait and does not advance time. Resident clock/work state required.
 */
s32 Ovl11ClockThresholdCommand(s32 targetHourOrFade, s32 targetMinute,
                               s32 operandFlags);

#endif
