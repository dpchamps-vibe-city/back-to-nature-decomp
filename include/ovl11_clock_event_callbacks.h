#ifndef OVL11_CLOCK_EVENT_CALLBACKS_H
#define OVL11_CLOCK_EVENT_CALLBACKS_H

#include "common.h"

/* gOvl11ClockEventHandlers slots 0..4. The clock queue retains its head while
 * a callback returns zero and removes it on nonzero. No callback advances
 * the clock itself; all operate on the current resident/area work state. */
s32 Ovl11ClockAcknowledgeEvent(void);

/* Shift three older eight-byte word pairs at resident+49C4 upward one slot,
 * then clear the newest pair. Returns 1. */
s32 Ovl11ClockShiftValueHistory(void);

/* Multi-call phase machine at resident+D0: suppresses input, waits for fades,
 * refreshes daily conditions, rebuilds scene/sprites/events/weather/sound,
 * then clears the transition flag. Returns 0 until phase 6, then 1. Requires
 * initialized graphics, resident state and area resources. The sound-transfer
 * wait inside the rebuild phase is blocking. Clock-advance mode defers the
 * final fade/scene refresh to the clock service. */
s32 Ovl11ClockRebuildDayScene(void);

/* Reload the current area's offset sprite set, refresh its record sources,
 * and clear the associated counter. Returns 1. */
s32 Ovl11ClockRefreshAreaSprites(void);

/* Run the record-pool and seven-record first passes, apply hourly deltas,
 * and clear event-slot time origins at hours 12 or 18. Returns 1. */
s32 Ovl11ClockApplyHourUpdates(void);

#endif
