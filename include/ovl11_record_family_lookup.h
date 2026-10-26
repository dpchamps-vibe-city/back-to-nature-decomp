#ifndef OVL11_RECORD_FAMILY_LOOKUP_H
#define OVL11_RECORD_FAMILY_LOOKUP_H

#include "common.h"

/* Read-only query over initialized shared work pools. family selects:
 * 0: ten 0xB4-byte records, type 0x109, excluding flag 0x40000 (filter ignored).
 * 1: twenty 0xB8-byte records, type 0x162; filter 0 requires record +0xB2 == 0,
 *    filter 1 additionally requires +0xAE == 0.
 * 2: same 0xB8 pool, type 0x165; filter 0 accepts all, filter 1 requires
 *    +0xB4 >= 7 and both +0xAE/+0xB2 == 0. These are unsigned-halfword tests.
 *
 * ordinal -1 returns the eligible count. Positive ordinals are ONE-based
 * match numbers; selection returns the ZERO-based physical pool index.
 * A missing match or invalid family returns 0, indistinguishable from index
 * zero: count first when that distinction matters. For families 1/2 only
 * filters 0/1 increment the count. Ordinal zero is outside the normal API:
 * comparison follows the filter, so it can select an excluded record.
 * No mutation, allocation, callbacks or random-state consumption. */
s32 Ovl11FindFamilyRecordIndex(s16 family, s16 filter, s16 ordinal);

#endif
