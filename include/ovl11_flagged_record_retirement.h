#ifndef OVL11_FLAGGED_RECORD_RETIREMENT_H
#define OVL11_FLAGGED_RECORD_RETIREMENT_H

#include "common.h"

/* Scan twenty B8 records, then ten B4 records, clearing those with +34 bit
 * 0100. For a marked B4 record with bit 00020000, also clear primary-object
 * flags 10008000, reset its global request and release its held item first.
 * For each retired record, increment work +523E (u16, signed test clamps at
 * 15) and +44CD (u8, clamps at 99 AFTER byte truncation). Other clients also
 * use these counters; they are not a count of the records still allocated.
 * No return value; shared pools and request state must be initialized. */
void Ovl11RetireFlaggedRecords(void);

#endif
