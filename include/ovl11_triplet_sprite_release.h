#ifndef OVL11_TRIPLET_SPRITE_RELEASE_H
#define OVL11_TRIPLET_SPRITE_RELEASE_H

#include "game_types.h"

/* Subtract one from the signed tail at +4. When its truncated s16 result
 * is zero, remove the sprite slot identified by the signed first halfword
 * and clear all six bytes. A negative result is stored as -1 and does not
 * release the slot. The caller must supply a valid slot when release occurs.
 * This is an update-count operation, not an elapsed-time timer. */
void Ovl11StepTripletSpriteRelease(Ovl11TripletTailView *record);

#endif
