#ifndef OVL11_CLOCK_RECORD_FLAGS_H
#define OVL11_CLOCK_RECORD_FLAGS_H

#include "common.h"

/* Clock-event table slots 6..9. Each clears record flag 0x400 through the
 * common group dispatcher and returns 1 unconditionally, consuming the event.
 * Codes 4 and 0 select fixed records; codes 1 and 2 select the first flagged
 * entry of the same 20-record pool. No flagged entry is also a consumed event.
 * These callbacks do not advance time or change the clock queue themselves. */
s32 Ovl11ClockClearRecordGroup4(void);
s32 Ovl11ClockClearRecordGroup1(void);
s32 Ovl11ClockClearRecordGroup2(void);
s32 Ovl11ClockClearRecordGroup0(void);

#endif
