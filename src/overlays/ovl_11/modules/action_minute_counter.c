/*
 * action_minute_counter — reconstructed translation unit.
 *
 * Update the object halfword counter once per normal clock-minute rollover,
 * gated by its action flags.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_action_minute_counter.h"

void Ovl11UpdateActionMinuteCounter(void *object) {
    u32 flags;
    u16 *minuteCounter;

    flags = *(u32 *) ((u8 *) object + 0x6C);
    minuteCounter = (u16 *) ((u8 *) object + 0x1A);
    if (flags & 0x18000100) {
        *minuteCounter = 0;
        return;
    }
    if ((flags & 0x04800000) == 0x04000000) {
        *minuteCounter = (u16) (*minuteCounter + 1);
        return;
    }
    *minuteCounter = 0;
}
