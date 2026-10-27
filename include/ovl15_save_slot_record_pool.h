#ifndef OVL15_SAVE_SLOT_RECORD_POOL_H
#define OVL15_SAVE_SLOT_RECORD_POOL_H

#include "common.h"

/* Transfer the five selected entries in each Ovl15Slots row. Row 1 records
 * leave the save-work pools for gOvl15StagedSlotRecords, with their staging
 * addresses recorded in the selection table; row 0 runtime records then move
 * into free save-work records. Sources are cleared after copying. Each moved
 * record's state is cleared and its placement updated before the copy.
 *
 * Kind -1 skips an entry; kind 0 copies 0xB4 bytes, kinds 1/2 copy 0xB8.
 * Indices, staging capacity and free destination capacity must be validated
 * by the caller. No rollback, bounds check or NULL-allocation guard exists.
 * Placement consumes Rand(4) for each X-row record. These calls change the
 * shared save/runtime pools and must not be interleaved with their consumers.
 */
void Ovl15StageSelectedSlotTransfers(void);

/* Restore staged row-1 records into newly allocated runtime records and
 * reapply placement. Run after staging and before the staging buffer/table
 * is reused; it does not clear the staged copies or selection descriptors.
 */
void Ovl15RestoreStagedSlotRecords(void);

/* Occupied capacity units, not always a count of records. Kind 0 scans ten
 * 0xB4 records, counting nonzero type OR reserved flag 0x02000000. Kind 1 scans
 * twenty 0xB8 records at base+0x708: a nonzero type counts once, and nonzero
 * halfword +0xB2 counts once more for that same occupied record. Other kinds
 * return zero. Read-only; result is a signed halfword.
 */
s16 Ovl15CountOccupiedPoolUnits(u8 *poolBase, s16 kind);

#endif
