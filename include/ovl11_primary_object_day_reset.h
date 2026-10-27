#ifndef OVL11_PRIMARY_OBJECT_DAY_RESET_H
#define OVL11_PRIMARY_OBJECT_DAY_RESET_H

#include "common.h"

/* Day-transition reset for the bound primary object and shared work state.
 * Clears action/request fields and game flags 3F/40/0A/76/A0, conditionally
 * resets the shared area sprite, and reinitializes object/shared sprite slots.
 * Applies status deltas through Ovl11ApplyStatusDelta according to shared
 * +4450 bit 4, game flag 47 and the retained +2C value; repeating this call
 * can therefore change gauges again. It updates level bands and the signed
 * +5242 counter (adds hour-8 when hour>=8, then caps signed values >=16 at 15).
 * Halfword truncation and the lack of a lower clamp are intentional.
 * Does not advance the calendar, perform a fade or load a new scene. */
void Ovl11ResetPrimaryObjectForDayTransition(void);

#endif
