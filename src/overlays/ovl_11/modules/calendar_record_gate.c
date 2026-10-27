/*
 * calendar_record_gate — reconstructed translation unit.
 *
 * Derive resident work flag bit 0 from the calendar service
 * first-fourteen-date lookup. This bit gates low-numbered record requests;
 * returns one regardless of the lookup result.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_calendar_record_gate.h"
#include "ovl11_clock_calendar.h"

s32 Ovl11UpdateCalendarRecordGate(void) {
    u8 *state;

    state = (u8 *)&D_8006C838;
    if (Ovl11FindCalendarDate(*(s16 *)(state + 0x44BA), *(s16 *)(state + 0x44BC)) != 0) {
        *(s32 *)(state + 0x4450) |= 1;
    } else {
        *(s32 *)(state + 0x4450) &= ~1;
    }
    return 1;
}
