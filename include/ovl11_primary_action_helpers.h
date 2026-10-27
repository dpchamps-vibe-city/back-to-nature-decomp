#ifndef OVL11_PRIMARY_ACTION_HELPERS_H
#define OVL11_PRIMARY_ACTION_HELPERS_H

#include "common.h"
#include "game_types.h"

/* Handle input bit 0x80 only when object flag 0x10000000 and itemIndex are
 * nonzero. Delegates to Ovl11TryBeginItemAction(1), whose state/animation,
 * flag and sound effects concern the SHARED primary object. Pass that object;
 * this argument is only the eligibility view. Returns a normalized handled
 * Boolean, not proof that a new action began. The action service requires
 * loaded item tables and initialized interaction state. A failed gate returns
 * zero without calling it. No repeat timing or button state is changed here. */
s32 Ovl11TryItemActionButton(Ovl11C7010Object *object, u16 pressedButtons);

/* Trigger the nearby-event scan for actor (heading +38, scene +FE, fixed-point
 * X/Z +100/+108). May start several reactions and consume random values.
 * Currently returns zero: Ovl11TriggerNearbyObjectReactions always returns
 * zero. The original adapter retains a dormant non-null result's +1A != 2
 * test; its caller ignores the result. Requires initialized object events. */
s32 Ovl11TriggerNearbyReactions(u8 *actor);

#endif
