#ifndef OVL11_QUEUED_ITEM_INTERACTION_H
#define OVL11_QUEUED_ITEM_INTERACTION_H

#include "common.h"

/* Submit item ID 0x51..0x55 iff the resident work +44F8 bit 1 is set and
 * the queued-item latch (+44D6) is empty. Returns 1 when stored, else 0.
 * No replacement, allocation or timer reset occurs. */
s32 Ovl11QueueItemInteraction(s16 itemId);
/* Empty latch: no work. Otherwise clear an out-of-season queued ID:
 * 51/55 require season 0, 52 requires 1, 53/54 require 2. Unknown IDs stay.
 * Then decrement the signed halfword timer at work+51DE by 5 and clamp
 * negative results to zero, including a tick that just cleared the latch. */
void Ovl11UpdateQueuedItemInteraction(void);
/* Clear only gQueuedItemInteractionId, the same resident-work halfword. */
void Ovl11ClearQueuedItemInteraction(void);

#endif
