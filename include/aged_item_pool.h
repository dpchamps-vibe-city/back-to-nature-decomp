#ifndef AGED_ITEM_POOL_H
#define AGED_ITEM_POOL_H

#include "common.h"

/* ovl_11 only. A shared pool of 99 {id, age} cells, with id=0 meaning free.
 * Ages are counts of requested updates, not elapsed frames or seconds. */

/* Add ID A1/A2/A3 with initial age 20/10/0 to the first free cell.
 * Return 1 on success, 0 for a different ID or a full pool. On success only,
 * refresh gAgedItemLastAddCount; that word is not a live occupancy counter. */
s32 Ovl11AddAgedItem(s32 id);

/* Clear all IDs and the update count, retaining ages in the freed cells. */
void Ovl11ClearAgedItemPool(void);

/* Coalesce one or more requests into one update of every occupied cell. */
void Ovl11QueueAgedItemUpdate(void);

/* Consume the request bit, increment ages, refresh ID tiers (<10=A3,
 * <20=A2, otherwise A1), and allow random additions for age >=30.
 * Refresh the count at game-state +0x99E4 even without a request.
 * Legacy s32 ABI has no defined return value: callers must ignore it.
 * Newly added cells later in the scan can advance during the SAME update. */
s32 Ovl11UpdateAgedItemPool(void);

/* Remove the first cell in ID priority A1, A2, A3; return its ID, or 0.
 * This prefers age tiers, not the greatest age within a tier. Does not
 * refresh either cached occupancy word. */
s32 Ovl11TakeAgedItem(void);

/* Compute current occupancy without changing state. */
s32 Ovl11CountAgedItems(void);

#endif
