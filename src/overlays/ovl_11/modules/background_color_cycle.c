/*
 * Background-color ping-pong cycle for scene kind 44.
 *
 * The private direction/index and two RGB triplets join this reset/update
 * pair; the neighbors manipulate a separate record/pointer family. Their
 * membership and boundary evidence is in notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_background_color_cycle.h"

/* Three-component integer interpolator; the scalar declarations of the
 * endpoint addresses preserve this unit's original address formation. */
void func_8001F278(s32 index, s32 span, s32 *low, s32 *high, s32 *out);
/* Enables background clearing and sets the RGB bytes in both render contexts. */
void ovl_11_func_800DC114(s32 red, s32 green, s32 blue);

void Ovl11ResetBackgroundColorCycle(void) {
    gOvl11BackgroundCycleDescending = 0;
    gOvl11BackgroundCycleIndex = 0x1FF;
    ovl_11_func_800DC114(0, 0xA5, 0xD5);
}

void Ovl11StepBackgroundColorCycle(void) {
    s32 rgb[3];
    s16 nextIncreasing;
    s16 nextDecreasing;

    func_8001F278((s32)gOvl11BackgroundCycleIndex, 0x1FF,
                 &gOvl11BackgroundCycleLowRgb, &gOvl11BackgroundCycleHighRgb, rgb);
    ovl_11_func_800DC114(rgb[0], rgb[1], rgb[2]);
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
