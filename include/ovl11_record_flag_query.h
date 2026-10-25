#ifndef OVL11_RECORD_FLAG_QUERY_H
#define OVL11_RECORD_FLAG_QUERY_H

#include "common.h"

/* Return 1 on the first accepted record, otherwise 0; no state changes.
 * poolMask bit 0 selects primary records whose parallel flag 0x10000 is
 * clear, bit 1 those where it is set, and bit 2 the secondary pool.
 * selectionMask bit 1 accepts flags with 0x100 set and 0x80000 clear.
 * Bit 0 preserves an original masked-value==1 test that never succeeds;
 * other selection bits have no effect. No pointer or count is supplied. */
s32 Ovl11AnyRecordFlagMatch(s32 poolMask, s32 selectionMask);

#endif
