#ifndef OVL11_ACTION_MINUTE_COUNTER_H
#define OVL11_ACTION_MINUTE_COUNTER_H

#include "common.h"

/* Update the object's +1A u16 counter from its +6C flags. Normal clock-minute
 * rollovers call this once; accelerated clock advance does not. Any bit in
 * 0x18000100 resets it; otherwise 0x04000000 set with 0x00800000 clear advances
 * it modulo 65536. All other flag combinations reset it. No other state changes.
 * Requires a writable record containing both fields. */
void Ovl11UpdateActionMinuteCounter(void *object);

#endif
