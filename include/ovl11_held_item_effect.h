#ifndef OVL11_HELD_ITEM_EFFECT_H
#define OVL11_HELD_ITEM_EFFECT_H

#include "common.h"
#include "game_types.h"

/* Set object flags 0x1870, enter state 0x45, select animation from the low BYTE
 * of facing +0x38, play sound 0xF and spawn effect {0x186,0,1} at +0x100.
 * The spawn direction instead reads the signed HALFWORD at +0x38.
 * Reset the shared held-action position/step/signal; choose header 0x1C for
 * item code 0x39 at +0x84, otherwise 0x1B. Returns 1 regardless of spawn success.
 *
 * Caller must have initialized the object's sprite source. The special-item
 * dispatcher uses this for codes 0x38/0x39, then rebinds that source separately.
 */
s32 Ovl11BeginHeldItemEffect(Ovl11C7010Object *object);

#endif
