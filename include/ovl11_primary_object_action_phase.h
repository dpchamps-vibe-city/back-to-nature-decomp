#ifndef OVL11_PRIMARY_OBJECT_ACTION_PHASE_H
#define OVL11_PRIMARY_OBJECT_ACTION_PHASE_H

#include "common.h"

/* Prioritized action phase for the shared primary object. Samples pressed and
 * held pad masks, then gates area contact, forced/rotation state, menus and
 * volume input, record interaction, paired list selection, item actions and
 * fade-gated idle state. Direction update/sound runs last unless special
 * vertical motion is active. Return 0 when no action handled (some helpers
 * deliberately consume input but return 0), 1 for handled, or 2 for a chosen
 * list-button path regardless of whether that helper started an action.
 * May change object/request flags, state, animation, position/velocity and
 * selected items; opens menus/messages, advances the direction sound delay
 * and plays effects. Requires the initialized object/input/area/render state.
 * It neither clears the pad masks nor integrates position for this frame. */
s32 Ovl11DispatchPrimaryObjectAction(void);

#endif
