/*
 * sixty_step_deltas — reconstructed translation unit.
 *
 * Complementary signed delta calculations over a common input quantized in
 * steps of sixty; used together by the primary-object reset path.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_sixty_step_deltas.h"

s32 Ovl11ComputeSixtyStepDecrease(s32 value) {
    s32 delta;

    delta = -((value / 60) * 2) - 5;
    if (delta < -0x80) {
        delta = -0x80;
    }
    return delta;
}

s32 Ovl11ComputeSixtyStepIncrease(s32 value) {
    s32 delta;

    delta = ((value / 60) * 7) + 0x32;
    if (delta >= 0x80) {
        delta = 0x7F;
    }
    return delta;
}
