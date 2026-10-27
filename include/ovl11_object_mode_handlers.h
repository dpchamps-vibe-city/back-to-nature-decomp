#ifndef OVL11_OBJECT_MODE_HANDLERS_H
#define OVL11_OBJECT_MODE_HANDLERS_H

#include "common.h"
#include "game_types.h"

/* Frame callbacks for gOvl11ObjectModeHandlers, indexed by object mode +0x26:
 * 0..5 keyframes, 6 message/choice, 7 movement, 8/9 grid actions,
 * 10 stateful routing, 11 action selection, 12 placement/return.
 * Slot 13 is null. Callbacks mutate the initialized object and shared scene
 * work; 0 retains the current mode, 1 releases it to dispatcher selection.
 * Invoke through the object-mode service, not as independent initializers.
 *
 * Keyframe callbacks use +0x28 as their state and +0x2A as update count.
 * First call sets state 1 and lock mask 0x800; reaching -1 releases the lock
 * on the FOLLOWING call. Modes 0/1/5 then set +0x2C to 300 update ticks.
 * Callback 10's detailed contract is in ovl11_object_mode_transition.h. */

/* Also called directly by action submission. Steps record movement using
 * its configured period and selector 0x70, chooses animation 0/1/6/15 from
 * +0xB6 and motion-vector fields, clears lock 0x800, and returns 1. */
s32 Ovl11StepObjectMode7Movement(Ovl11E644Object *object);

#endif
