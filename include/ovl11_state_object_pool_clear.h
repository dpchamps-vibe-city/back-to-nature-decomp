#ifndef OVL11_STATE_OBJECT_POOL_CLEAR_H
#define OVL11_STATE_OBJECT_POOL_CLEAR_H

#include "common.h"

/* Visit all six 0xB0-byte gStateObjectPool records, clearing each eligible
 * record through Ovl11ClearB0ObjectRecord. No compaction or allocation.
 *
 * A record with flags +34 bit 0x20000 is retained unless includeFlag20000
 * is nonzero. Independently, a record whose tag has ANY bit in mask 0x15B
 * is retained while shared-work +44F8 bit 0x01000000 is set. The second
 * guard is not overridden by the argument, and is a bit test, not tag ==
 * 0x15B. Empty records are visited too. The shared work must be initialized. */
void Ovl11ClearEligibleStateObjects(s32 includeFlag20000);

#endif
