#ifndef OVL11_EVENT_SCHEDULE_H
#define OVL11_EVENT_SCHEDULE_H

#include "common.h"

/* Reset 37 resident event slots and all thirty 8-byte schedule entries in
 * each. Resolve each non-NULL stream through game flags; offsets are relative
 * to the loaded area's scriptBlobBase. Copy thirty entries from the selected
 * stream, store its relative offset, and seed current values from entry 0.
 * Unresolved streams keep zero keys and 36/36 defaults. Resets effect timers,
 * positions and masked flags as well as schedules. Streams/offsets must be
 * valid and terminated; this routine performs no bounds validation. */
void Ovl11InitEventSchedules(s32 scriptBlobBase);

/* For each slot, choose the first signed time key >= timeOfDay. Keys use the
 * clock's minute-of-day units. Exactly 360 (06:00) is deliberately a no-op.
 * No qualifying key leaves the slot unchanged. Otherwise set the selected
 * index, clear +C, copy the LOW BYTE of entry +2 to +E and its final two
 * halfwords to +2C/+2E. Does not advance the schedule or invoke events. */
void Ovl11SelectScheduleEntryForTime(s16 timeOfDay);

#endif
