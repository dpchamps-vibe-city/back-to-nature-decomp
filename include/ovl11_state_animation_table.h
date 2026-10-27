#ifndef OVL11_STATE_ANIMATION_TABLE_H
#define OVL11_STATE_ANIMATION_TABLE_H

#include "common.h"

/* Read the private [state][variant] animation/frame pair. The table spans
 * 18 states and four variants; both indices are unchecked. animation must
 * be writable; frame may be NULL. Always returns 0, not a success flag.
 * The state-object renderer treats frame zero as "do not select a frame";
 * this reader merely copies it and does not mutate animation/sprite state. */
s32 Ovl11ReadStateSpriteAnimation(u16 state, u16 variant,
                                u16 *animation, u16 *frame);

#endif
