#ifndef GAME_RANDOM_H
#define GAME_RANDOM_H

#include "common.h"

/* exe:0x80012A34. Advance gRandomState modulo 2^32 using
 * state = state*1103515245 + 12345, then scale its upper 16 bits by the
 * LOW 16 bits of limit. Returns [0, limit&0xFFFF), or 0 for a zero effective
 * limit. Negative and >65535 limits are truncated, not rejected. State
 * advances even for zero limit. Shared state makes this non-reentrant;
 * writing gRandomState reseeds the next call (no separate seed function).
 */
u32 Rand(s32 limit);

#endif
