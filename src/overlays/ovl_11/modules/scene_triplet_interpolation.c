/*
 * Scene-kind 15 / id E5: interpolate three words and publish their low
 * halfwords to scene record zero (+8/+A/+C). The components' meaning is not
 * established; they are not named as colors or coordinates. The adjacent
 * clock-driven palette service is the other scene-id branch and uses no
 * endpoint/progress state from this pair. See notes/file-groupings.md and
 * ovl11_scene_triplet_interpolation.h.
 */

#include "common.h"
#include "word_interpolation.h"
#include "game_flags.h"
#include "ovl11_scene_triplet_interpolation.h"

void ovl_11_func_800DCBDC(s16 sceneIndex, void *triplet);

void Ovl11ResetSceneTripletInterpolation(void) {
    u16 initial[3];

    /* Three signed words in storage, but only their low halfwords publish. */
    initial[0] = ((u16 *)gOvl11SceneTripletStart)[0];
    initial[1] = ((u16 *)gOvl11SceneTripletStart)[2];
    initial[2] = ((u16 *)gOvl11SceneTripletStart)[4];
    gOvl11SceneTripletProgress = 0;
    ovl_11_func_800DCBDC(0, initial);
}

void Ovl11StepSceneTripletInterpolation(void) {
    s32 interpolated[3];
    u16 published[3];

    if (GetGameFlag(8U) == 1) {
        InterpolateThreeWords(gOvl11SceneTripletProgress, 0x4B0,
                              gOvl11SceneTripletStart, gOvl11SceneTripletEnd,
                              interpolated);
        published[0] = (u16) interpolated[0];
        published[1] = (u16) interpolated[1];
        published[2] = (u16) interpolated[2];
        ovl_11_func_800DCBDC(0, published);
        if (gOvl11SceneTripletProgress >= 0x4B0) {
            SetGameFlag(8U, 0U);
            return;
        }
        gOvl11SceneTripletProgress += 1;
    }
}
