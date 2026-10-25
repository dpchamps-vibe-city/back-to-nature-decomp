#ifndef OVL11_OBJECT_EVENT_POOL_H
#define OVL11_OBJECT_EVENT_POOL_H

#include "common.h"

/* ovl_11's fixed pool is 37 contiguous 0x1D4-byte records in shared state.
 * A signed -1 ID at +0 marks a free slot. Calls require initialized state;
 * no memory allocation or bounds checking is performed.
 */

/* Try IDs 0..36 in order using the first-free-slot allocator, then rebuild
 * schedule pairings. Does NOT first clear the pool: use ResetAllEventSlots
 * before this call when a full fresh population is required. */
void Ovl11AllocateAllEventSlots(void);

/* Zero only the schedule-time offsets at +0x0A, not schedules or counters. */
void Ovl11ClearEventSlotTimeOrigins(void);

/* Clear every record, mark each free, clear each +0xE0 timer pair, then rebuild
 * schedule pairings. Does not allocate IDs into the resulting free slots. */
void Ovl11ResetAllEventSlots(void);

/* Clear only +0x1E bit 0x10 across the pool. */
void Ovl11ClearEventSlotScheduleChanges(void);

/* Maintain +2/+4 counters: transient flags can increase them; Rand(10)==0
 * can decrease them for eligible SLOT INDICES. Clamp to 0..255 and 0..65535,
 * clear +0x1E bits 0x600, publish flag 0x4B and latch flags 0xE0..0xE4 when
 * the five existing thresholds reach 1501. Their gameplay meaning is unknown.
 * The classification helper is indexed by physical slot, not stored ID. */
void Ovl11UpdateEventSlotCounters(void);

#endif
