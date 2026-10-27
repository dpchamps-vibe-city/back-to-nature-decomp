/*
 * nearby_reaction_query — reconstructed translation unit.
 *
 * Primary action dispatch's nearby-reaction adapter. Its dependency returns
 * zero unconditionally today; retain the original dormant pointer/state test
 * rather than replacing this side-effecting call with a constant return.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_object_events.h"
#include "ovl11_primary_action_helpers.h"

s32 Ovl11TriggerNearbyReactions(u8 *actor) {
    Ovl11AnimationObject *reaction;

    reaction = (Ovl11AnimationObject *)Ovl11TriggerNearbyObjectReactions(actor);
    if (reaction == 0) {
        return 0;
    } else {
        return reaction->field_1A != 2;
    }
}
