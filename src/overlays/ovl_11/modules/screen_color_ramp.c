/*
 * Persistent screen-color interpolation in ovl_11.
 *
 * This complementary draw/step and target-setting pair shares one RGB/delta/
 * counter layout. DrawScreenSubtractOverlay shifts each channel by 12 before
 * building a full-screen semitransparent quad, establishing the fixed-point
 * units. The caller selects targets from a table and decides when to update;
 * those game-state decisions remain outside this small reconstructed module.
 * The complete original TU boundary is not proven; see notes/file-groupings.md.
 *
 * gScreenColorRamp remains external persistent RAM, not overlay-owned storage.
 * ovl_15 saves/restores its whole 0x24-byte block, including the trailing word
 * whose purpose is unknown and which neither helper touches.
 */

#include "common.h"
#include "screen_color_ramp.h"
#include "screen_fade.h"

/* ovl_11:0x800BFF74 */
void Ovl11DrawAndStepScreenColorRamp(void) {
    struct ScreenColorRampState *ramp = &gScreenColorRamp;

    /* The original tests the sum, not whether any individual channel is set. */
    if ((ramp->rgbQ12[0] + ramp->rgbQ12[1] + ramp->rgbQ12[2]) != 0) {
        DrawScreenSubtractOverlay(ramp->rgbQ12[0], ramp->rgbQ12[1], ramp->rgbQ12[2]);
    }
    if (ramp->durationUpdates > ramp->elapsedUpdates) {
        ramp->elapsedUpdates = ramp->elapsedUpdates + 1;
        ramp->rgbQ12[0] = ramp->rgbQ12[0] + ramp->stepQ12[0];
        ramp->rgbQ12[1] = ramp->rgbQ12[1] + ramp->stepQ12[1];
        ramp->rgbQ12[2] = ramp->rgbQ12[2] + ramp->stepQ12[2];
    }
}

/* ovl_11:0x800C0010 */
void Ovl11SetScreenColorRampTarget(s32 updateCount, const s32 targetRgbQ12[3]) {
    struct ScreenColorRampState *ramp = &gScreenColorRamp;

    ramp->elapsedUpdates = 0;
    ramp->durationUpdates = updateCount;
    ramp->stepQ12[0] = (targetRgbQ12[0] - ramp->rgbQ12[0]) / updateCount;
    ramp->stepQ12[1] = (targetRgbQ12[1] - ramp->rgbQ12[1]) / updateCount;
    ramp->stepQ12[2] = (targetRgbQ12[2] - ramp->rgbQ12[2]) / updateCount;
}
