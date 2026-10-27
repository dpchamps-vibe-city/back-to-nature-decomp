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
#include "psyq/stddef.h"
#include "ovl11_object_group_flag_command.h"

s32 Ovl11ClockClearRecordGroup4(void) {
    Ovl11ApplyObjectGroupCommand(4, 0, 0, 0);
    return 1;
}

s32 Ovl11ClockClearRecordGroup1(void) {
    Ovl11ApplyObjectGroupCommand(1, 0, 0, 0);
    return 1;
}

s32 Ovl11ClockClearRecordGroup2(void) {
    Ovl11ApplyObjectGroupCommand(2, 0, 0, 0);
    return 1;
}

s32 Ovl11ClockClearRecordGroup0(void) {
    Ovl11ApplyObjectGroupCommand(0, 0, 0, 0);
    return 1;
}
