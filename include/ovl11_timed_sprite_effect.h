#ifndef OVL11_TIMED_SPRITE_EFFECT_H
#define OVL11_TIMED_SPRITE_EFFECT_H

#include "common.h"

/* Each state points to four halfword-aligned bytes: signed animation ID at
 * +0, signed remaining ticks at +2 (Ovl11TimedSpriteEffect in game_types.h).
 * Raw pointers also accept the existing embedded +0xA8/+0xE0 object views. */
void Ovl11ResetTimedSpriteEffect(void *state); /* ID = -1, ticks = 0 */
void Ovl11StartTimedSpriteEffect(void *state, s16 animationId, s16 ticks);

/* Decrement nonzero ticks unless work-state flag 0x08000000 is set.
 * Returns 0 after decrementing, even on the tick that reaches zero;
 * returns 1 if already zero OR the flag suppresses the update. */
s32 Ovl11TickTimedSpriteEffect(void *state);

/* Draw only while ticks != 0; does not advance the countdown.
 * Frame = frameCount - 1 - ((ticks / 10) % frameCount), masked to a byte.
 * x/y are screen pixels; ot is the ordering-table address. The shared sprite
 * source must have a nonzero frame count for the full signed animation ID.
 * Drawing selects the ID's LOW BYTE, unlike the frame-count lookup. */
void Ovl11DrawTimedSpriteEffect(void *state, s32 ot, s16 x, s16 y);

#endif
