#ifndef OVL11_CLOCK_CALENDAR_H
#define OVL11_CLOCK_CALENDAR_H

#include "common.h"

/* ovl_11 must be resident. Shared clock/calendar and a fifty-ID callback
 * queue; no reentrancy. Time fields use integer hours/minutes. Calendar
 * rollover is 30 days/season, four seasons/year, seven weekdays; day advances
 * at 06:00, not midnight. Year is capped at 98. Init starts year/season 0,
 * day/weekday 1 at 06:00 and resets the queue and batched advance state.
 * ResetClockEvents clears queue/advance state without resetting the date.
 * Neither reset clears the deferred refresh request.
 *
 * Update ticks subject to fade, scene and game-state gates, then services
 * the queue head once. A head stays queued until its callback returns nonzero.
 * RunClockAdvance forces minute ticks until the requested count is exhausted
 * or an event remains queued. It must be called again after that event ends.
 * The final tick is consumed before suspension; completion runs the original
 * scene refresh hooks. Requests replace, rather than add to, remaining minutes.
 * Nonpositive counts finish on the next Run without ticking.
 *
 * RequestClockAdvance clears the refresh request; SetClockAdvanceMinutes does
 * not. RequestClockAdvanceToSix adds offsetMinutes to the distance to the next
 * 06:00 (at 06:00 this means 24 hours) and returns that minute count. The
 * refresh hook is requested only by value 1; the setter retains other values.
 * HasQueuedClockEvent/IsClockAdvanceActive do not mutate state. */

/* Original identities, ovl_11: 800C0688, 06B0, 0824. */
void Ovl11UpdateClockAndEvents(void);
void Ovl11RunClockAdvance(void);
s32 Ovl11IsClockAdvanceActive(void);
/* 800C0834: 5..6 -> 0, 7..14 -> 1, 15..16 -> 2, every other hour -> 3. */
s32 Ovl11ClassifyClockHour(s16 hour);
/* 800C087C, 08E8, 090C, 09D0, 0A28, 0A40, 0D9C. */
s32 Ovl11RequestClockAdvanceToSix(s32 offsetMinutes);
void Ovl11RequestClockAdvance(s32 minutes);
void Ovl11InitClockAndCalendar(void);
void Ovl11ResetClockEvents(void);
s32 Ovl11HasQueuedClockEvent(void);
void Ovl11SetClockAdvanceRefresh(s32 requested);
void Ovl11SetClockAdvanceMinutes(s32 minutes);

/* 800C1224/1280: search the first 14/all 18 date rows. Return one-based
 * index, or zero; input values are compared literally, without normalization.
 * 800C141C: only values 0x34..0x37 are eligible for remapping using today's
 * 14-row lookup. Missing dates or table value -1 leave the input unchanged.
 * These lookups do not advance the clock or publish flags. */
s32 Ovl11FindCalendarDate(s16 season, s16 day);
s32 Ovl11FindCalendarDateExtended(s16 season, s16 day);
s16 Ovl11RemapCalendarValue(s16 value);

#endif
