#ifndef OVL11_HELD_ITEM_ACTIONS_H
#define OVL11_HELD_ITEM_ACTIONS_H

#include "common.h"
#include "game_types.h"

/* Examine substituted button 0x80 and the held record ID at +84. On a new
 * press try low, grouped, high and special code handlers in that order;
 * their selected states, pending-record copies and collision cleanup differ.
 * While held, increment nonzero charge at +18. Return 1 for handled/charging
 * input or the >=100 +16 gate; return 0 for blocked/idle input. Unhandled
 * input clears charge. Some item codes enter map-camera mode or play sound.
 * Uses shared input, collision-dispatch and charge-frame state; not reentrant.
 * IDs outside the recognized ranges are not silently treated as generic use. */
s32 Ovl11UpdateHeldItemAction(Ovl11C7010Object *object);

#endif
