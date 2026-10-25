/* Shared 32-bit LCG state; only this unit defines it for GP addressing.
 * Overlay callers also reseed it directly. See include/random.h. */
#include "common.h"
#include "random.h"

u32 gRandomState;

u32 Rand(s32 limit) {
    u32 state;
    u32 multiplier;
    u32 scaled;

    multiplier = 0x41C64E6D;
    state = gRandomState;
    state = state * multiplier;
    state = state + 0x3039;
    gRandomState = state;
    scaled = state >> 0x10;
    scaled = scaled * (limit & 0xFFFF);
    return scaled >> 0x10;
}
