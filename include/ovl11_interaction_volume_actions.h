#ifndef OVL11_INTERACTION_VOLUME_ACTIONS_H
#define OVL11_INTERACTION_VOLUME_ACTIONS_H

#include "common.h"
#include "game_types.h"

/* Handle the primary interaction/menu input in priority order. Nonzero
 * selection mode returns 1; shared request bit 10000000 blocks with 0.
 * Buttons 100/800/10 open the selection/alternate/item-transfer menus and
 * set request bits 18000000. Object flags 40000108 block later actions.
 * Button 40 first probes a flagged interaction volume at the object's current
 * position, then dispatches the facing-offset volume if the probe found none.
 * Other buttons do nothing here. Input is the raw button bitmask.
 * Return 0 for unhandled/rejected input, 1 for a consumed menu/probe, or the
 * dispatcher's volume/action code (not necessarily a state-change success).
 * Actions can change object state/animation/flags, install request triplets,
 * open messages/menus, set game/area flags and play sound effects. Requires
 * resident area header, initialized object/item state and message services.
 * The type-specific helpers remain external but are not public interfaces. */
s32 Ovl11DispatchPrimaryInteractionInput(Recon_ovl_11_func_800C7270_A0View *object,
                                        u16 buttons);

#endif
