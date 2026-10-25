#ifndef OVL11_OBJECT_MODE_TRANSITION_H
#define OVL11_OBJECT_MODE_TRANSITION_H

#include "common.h"
#include "game_types.h"

/* ovl_11's object-handler dispatch table calls this while the overlay is
 * resident. The object must carry the full engine-object prefix as well as
 * the flags/index/retained-state tail described by Ovl11ModeTransitionObject.
 *
 * Advance routing among object modes 1, 4 and 5 using the area/mode lookup
 * helpers. Negative action states -1..-4 encode pending mode changes. Writes
 * movement/control flags, position and action state; may submit action 0xE6.
 * Returns 0 while active (saving +26/+28/+2A into the retained state), or 1
 * after releasing control and dispatching action 7. No elapsed-time input.
 * Must not be reentered for the same object during a helper call.
 * Original identity: ovl_11:0x8010EEE4. */
s32 Ovl11UpdateObjectModeTransition(Ovl11ModeTransitionObject *transition);

#endif
