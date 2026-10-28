#ifndef OVL11_PRIMARY_OBJECT_GRID_ACTIONS_H
#define OVL11_PRIMARY_OBJECT_GRID_ACTIONS_H

#include "common.h"
#include "game_types.h"

/* Advance the primary object's current action and optional six-byte script.
 * Returns 0 while waiting/continuing, 1 when the action/script is finished.
 * May alter object state, held records, grid cells, sprite sources, gauges,
 * sound and actuator sequences. The asynchronous footprint request is one
 * shared slot: callers must not interleave actions for different objects.
 * Position inputs are world coordinates; converters select the scene grid.
 * Footprint power accumulates tier-scaled additions in a wrapping u16 shared
 * with Ovl11UpdatePrimaryObject, which resets it on a new dispatched action.
 * Item/tier table indices are caller-validated; this service does not clamp
 * them. Grid operations skip mode 2 (no grid), and retain distinct completion,
 * score application and request-reset gates. */
s32 Ovl11StepPrimaryObjectAction(Ovl11C580CObject *object);

/* Begin transfer of the selected nonempty six-byte record to pending +F0.
 * sourceKind==0 selects +84; otherwise slotIndex selects +90+6*slotIndex.
 * Requests state 0x15 / animation 0x47 with the supplied animation parameter,
 * marks the pending record and plays sound 7. Does not clear the source. */
void Ovl11BeginSelectedRecordTransfer(Recon_ovl_11_func_800C7270_A0View *object,
                                     s32 sourceKind, s32 slotIndex, s32 animationParameter);

/* For held IDs 0x6D/0x6E only: play sound 8, clear two flags and request
 * state 0x34 with the facing-relative animation. Otherwise no effects. */
void Ovl11SelectHeldPairChangeAnimation(Ovl11C7010Object *object);

#endif
