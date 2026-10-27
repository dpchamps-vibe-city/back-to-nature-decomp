#ifndef OVL11_SECONDARY_GAUGE_CLOCK_ADVANCE_H
#define OVL11_SECONDARY_GAUGE_CLOCK_ADVANCE_H

#include "common.h"
#include "game_types.h"

/* Read signed object+0x16 and return an extra clock-advance offset in minutes:
 * 50..69 -> 120, 70..79 -> 240, 80..99 -> 360; all other values -> 0.
 * The pointer must be valid. No state is changed and -1 is never returned.
 * This is the secondary status gauge, not a proven gameplay statistic. */
s32 Ovl11GetSecondaryGaugeClockOffset(Ovl11SecondaryGaugeView *object);

/* For gPrimaryObjectBase, snapshot shared-work halfword +44CA into object
 * halfword +2C, then request clock advancement to six with the gauge offset
 * above. It requests the clock's asynchronous transition; it does not step
 * the clock itself. Work/object storage must already be installed. */
void Ovl11ApplySecondaryGaugeClockAdvance(void);

#endif
