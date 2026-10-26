/*
 * Background-color ping-pong cycle for scene kind 44.
 *
 * The private direction/index and two RGB triplets join this reset/update
 * pair; the neighbors manipulate a separate record/pointer family. Their
 * membership and boundary evidence is in notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_background_color_cycle.h"
#include "word_interpolation.h"
#include "ovl11_scene_setup_install.h"

void Ovl11ResetBackgroundColorCycle(void) {
    gOvl11BackgroundCycleDescending = 0;
    gOvl11BackgroundCycleIndex = 0x1FF;
    Ovl11SetRenderContextColor(0, 0xA5, 0xD5);
}

void Ovl11StepBackgroundColorCycle(void) {
    s32 rgb[3];
    s16 nextIncreasing;
    s16 nextDecreasing;

    InterpolateThreeWords((s32)gOvl11BackgroundCycleIndex, 0x1FF,
                 &gOvl11BackgroundCycleLowRgb, &gOvl11BackgroundCycleHighRgb, rgb);
    Ovl11SetRenderContextColor(rgb[0], rgb[1], rgb[2]);
    /* Update after writing, and clamp instead of immediately stepping away
     * from an endpoint. That deliberately holds each endpoint for two calls. */
    if (gOvl11BackgroundCycleDescending == 0) {
        gOvl11BackgroundCycleIndex = gOvl11BackgroundCycleIndex + 1;
        nextIncreasing = gOvl11BackgroundCycleIndex;
        if (nextIncreasing >= 0x200) {
            gOvl11BackgroundCycleDescending = 1;
            gOvl11BackgroundCycleIndex = 0x1FF;
        }
    } else {
        gOvl11BackgroundCycleIndex = gOvl11BackgroundCycleIndex - 1;
        nextDecreasing = gOvl11BackgroundCycleIndex;
        if (nextDecreasing < 0) {
            gOvl11BackgroundCycleDescending = 0;
            gOvl11BackgroundCycleIndex = 0;
        }
    }
}
