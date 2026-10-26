#ifndef OVL11_HOUR_COUNTER_ADJUSTMENT_H
#define OVL11_HOUR_COUNTER_ADJUSTMENT_H

#include "common.h"

/* Unless primary-object +0x6C bit 0x400000 is set, scan the four private
 * {hour, delta} records and apply every entry equal to the signed clock hour.
 * The current table applies +3/+4/+5/+10 at hours 0/2/4/6. D622C performs
 * its existing counter clamps/guards on primary-object +0x16. This routine
 * neither advances the clock nor latches an hour as handled; repeated calls
 * repeat the adjustment. No particular gameplay meaning is assigned to the
 * counter. */
void Ovl11ApplyHourCounterDeltas(void);

#endif
