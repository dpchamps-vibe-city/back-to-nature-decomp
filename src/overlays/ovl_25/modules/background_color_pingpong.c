/*
 * Background RGB for both render contexts and a 512-position ping-pong
 * cycle. The driver samples before advancing, clamps at each reversal,
 * and therefore displays each endpoint on two consecutive updates.
 *
 * Original ovl_25 entries BBA30/BBA7C. The latter calls the former and owns
 * the cycle's direction/index and endpoint use; the neighboring record-value
 * increments share neither protocol nor private state. See file-groupings.md.
 */

#include "common.h"
#include "word_interpolation.h"
#include "ovl25_background_color_cycle.h"

void Ovl25SetBackgroundColor(s16 red, s16 green, s16 blue) {
    u8 *context0 = (u8 *)&gRenderContexts[0];
    u8 *context1 = (u8 *)&gRenderContexts[1];

    /* DRAWENV isbg/r0/g0/b0, at +18/+19/+1A/+1B of each context. */
    context0[0x19] = red;
    context0[0x1A] = green;
    context0[0x1B] = blue;
    context1[0x19] = red;
    context1[0x1A] = green;
    context1[0x1B] = blue;
    context0[0x18] = 1;
    gRenderContexts[1].unk18 = 1;
}

void Ovl25StepBackgroundColorCycle(void) {
    s32 rgb[3];
    s16 nextIncreasing;
    s16 nextDecreasing;

    InterpolateThreeWords((s32) gOvl25BackgroundCycleIndex, 0x1FF,
        gOvl25BackgroundCycleLowRgb, gOvl25BackgroundCycleHighRgb, rgb);
    Ovl25SetBackgroundColor(rgb[0], rgb[1], rgb[2]);
    if (gOvl25BackgroundCycleDescending == 0) {
        gOvl25BackgroundCycleIndex = gOvl25BackgroundCycleIndex + 1;
        nextIncreasing = gOvl25BackgroundCycleIndex;
        if (nextIncreasing >= 0x200) {
            gOvl25BackgroundCycleDescending = 1;
            gOvl25BackgroundCycleIndex = 0x1FF;
        }
    } else {
        gOvl25BackgroundCycleIndex = gOvl25BackgroundCycleIndex - 1;
        nextDecreasing = gOvl25BackgroundCycleIndex;
        if (nextDecreasing < 0) {
            gOvl25BackgroundCycleDescending = 0;
            gOvl25BackgroundCycleIndex = 0;
        }
    }
}
