/*
 * hour_counter_adjustment — reconstructed translation unit.
 *
 * Apply the private hour-indexed signed deltas to the primary-object +0x16
 * counter, unless its suppression flag is set.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_hour_counter_adjustment.h"

s32 ovl_11_func_800D622C(s8 arg0, s8 arg1, s16 arg2, s32 arg3);

void Ovl11ApplyHourCounterDeltas(void) {
    char *base = (char *)&gPrimaryObjectBase;
    s16 *clockHour;
    Ovl11HourCounterDelta *entry;
    u32 index;

    if (!(*(s32 *)(base + 0x6C) & 0x400000)) {
        /* Retain the shared work-area view: this address is gClockHour. */
        clockHour = (s16 *)(base - 0xD08);
        entry = gOvl11HourCounterDeltas;
        index = 0;
        do {
            if (*clockHour == entry->hour) {
                ovl_11_func_800D622C(0, entry->delta, 0, 0);
            }
            index += 1;
            entry++;
        } while (index < 4U);
    }
}
