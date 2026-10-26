#ifndef OVL11_PRIMARY_OBJECT_DIRECTION_H
#define OVL11_PRIMARY_OBJECT_DIRECTION_H

#include "common.h"

/* Read the substituted held-button mask, save old facing and map direction
 * buttons to facing 0..3. Requests idle/walk/action animation states and adds
 * the facing/mode-indexed world-coordinate X/Z offsets to the primary object.
 * Returns 1 for a serviced direction/action input, 0 for the idle arm (which
 * still requests idle state). Uses shared primary-object and input state;
 * does not clear the offset accumulators. Flag 0x200000 suppresses the action
 * modifier, and the state-change flag is cleared on the handled paths. */
s32 Ovl11UpdatePrimaryObjectDirection(void);

#endif
