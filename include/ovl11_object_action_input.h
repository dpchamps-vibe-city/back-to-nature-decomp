#ifndef OVL11_OBJECT_ACTION_INPUT_H
#define OVL11_OBJECT_ACTION_INPUT_H

#include "common.h"
#include "game_types.h"

/* Process held/pressed substituted buttons for the object's current state.
 * Charge states 0x16/18/1A/1C/1E increment +18 up to 20*the +86 word, then
 * count saturated-charge updates; at 61, request state 0x3E and clear count.
 * Releasing button 0x80 selects the tier/facing animation and records the
 * PlaySoundEffect voice handle for later cancellation. Other recognized states
 * perform their own animation, movement or position-transition work.
 * Returns 1 only for the handled state-0x2D exit, otherwise 0 (effects can
 * still have occurred). Flag 0x40 suppresses processing. Uses shared input,
 * clock-period, charge and voice state; charge units are update calls. */
s32 Ovl11DispatchObjectActionInput(Ovl11C7010Object *object);

#endif
