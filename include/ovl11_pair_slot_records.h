#ifndef OVL11_PAIR_SLOT_RECORDS_H
#define OVL11_PAIR_SLOT_RECORDS_H

#include "common.h"

/* Refresh dispatch-list 3 records 10/11 from pair slots 2/3. Enable bit 0
 * is set only when the slot-state query returns 1. Signed halfwords at
 * work+0x99D2/+0x99D6 select kind 0xE8 only when exactly 1; all other
 * values select 0x123. Other record flags and fields are preserved.
 * Requires initialized shared work, pair slots and dispatch-list pointers;
 * this writes existing records, allocates nothing and returns no result. */
void Ovl11RefreshPairSlotRecords(void);

/* The corresponding record writer: index 0/1 selects list-3 entry 10/11
 * (24-byte stride); indices are unchecked. enabled == 1 sets bit 0,
 * every other value clears it. alternateKind == 0 writes kind 0x123,
 * any nonzero value writes 0xE8, independently of the enable bit.
 * Requires writable resident list storage. No other fields are changed. */
void Ovl11UpdatePairSlotRecord(s32 index, s32 enabled, s16 alternateKind);

#endif
