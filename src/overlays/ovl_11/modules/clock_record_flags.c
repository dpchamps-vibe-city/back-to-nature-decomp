/*
 * clock_record_flags — reconstructed translation unit.
 *
 * The final four clock-event callbacks clear record flag 0x400 for record
 * groups 4, 1, 2 and 0. Each returns 1 so the clock event queue consumes the
 * event.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_clock_record_flags.h"

s32 ovl_11_func_800EC064(s16 group, s16 index, s32 operation, s32 indirectIndex);

s32 Ovl11ClockClearRecordGroup4(void) {
    ovl_11_func_800EC064(4, 0, 0, 0);
    return 1;
}

s32 Ovl11ClockClearRecordGroup1(void) {
    ovl_11_func_800EC064(1, 0, 0, 0);
    return 1;
}

s32 Ovl11ClockClearRecordGroup2(void) {
    ovl_11_func_800EC064(2, 0, 0, 0);
    return 1;
}

s32 Ovl11ClockClearRecordGroup0(void) {
    ovl_11_func_800EC064(0, 0, 0, 0);
    return 1;
}
