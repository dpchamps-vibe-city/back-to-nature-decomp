#ifndef OVL11_CALENDAR_RECORD_GATE_H
#define OVL11_CALENDAR_RECORD_GATE_H

#include "common.h"

/* ovl_11:800DE76C. Set resident+4450 bit 0 iff the signed season/day at
 * +44BA/+44BC matches one of Ovl11FindCalendarDate's first fourteen rows;
 * otherwise clear it, preserving all other bits. Returns 1 in BOTH cases,
 * not the match result. No clock advancement or event is requested.
 * Record request services F12D0/F1354 reject codes below 0x12C while this
 * bit is set. Called during scene initialization and day-scene rebuilding;
 * the caller must have installed the calendar fields first. */
s32 Ovl11UpdateCalendarRecordGate(void);

#endif
