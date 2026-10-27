#ifndef OVL11_CLOCK_ADVANCE_COMMAND_H
#define OVL11_CLOCK_ADVANCE_COMMAND_H

#include "common.h"

/* ovl_11 clock adapter. slotMask masks 0x1/0x2 resolve hours/minutes respectively
 * from signed low-halfword value slots; other bits ignored, indices unchecked.
 * relative != 0 requests hours*60+minutes directly and returns 1, even for
 * nonpositive totals. Otherwise treat operands as a target clock time:
 * before 06:00, targets >=06:00 fail; at/after 06:00, target hours <6 gain 24.
 * Request the resulting nonnegative minute delta, returning 1; negative deltas
 * return 0 without a request. Hours/minutes are not range-normalized.
 * Requests REPLACE the pending advance and clear its refresh request. This
 * command does not tick or wait; return 1 does not mean time has advanced.
 * Initialize clock/work/value state; guarded use normally supplies 0..23 hours
 * and 0..59 minutes. Original identity: ovl_11:800E99EC. */
s32 Ovl11ClockAdvanceCommand(s16 hoursOrSlot, s16 minutesOrSlot, s32 relative, s32 slotMask);

#endif
