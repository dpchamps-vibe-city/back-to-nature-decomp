#ifndef OVL11_PRIMARY_WORK_COUNTER_RESET_H
#define OVL11_PRIMARY_WORK_COUNTER_RESET_H

#include "common.h"

/* Area-entry/day-transition reset of the primary object's shared work state.
 * Clear bit 30 at work +0x5234 and zero the three signed words at
 * +0x52D8/+0x52DC/+0x52E0. Other flags and object fields are unchanged.
 * The counters' gameplay meaning is not established. No input or result;
 * repeated calls are idempotent and do not reset the complete object.
 */
void Ovl11ClearPrimaryWorkCounters(void);

#endif
