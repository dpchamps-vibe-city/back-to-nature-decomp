/*
 * 640x480 subtractive screen fades, separate from the 320x240 screen_fade
 * driver. The two starters replace a shared countdown: mode 1 increases the
 * subtraction intensity; mode 2 repeatedly subtracts a fixed intensity from
 * the retained framebuffer. No retail callers of the starters/query have
 * been identified; the frame loop does call the service below.
 *
 * Shared GP-relative state and the service's direct environment-setup call
 * support this reconstructed module, not a proven original file boundary.
 * See wide_screen_fade.h for the control contract and service side effects.
 */

#include "common.h"
#include "wide_screen_fade.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/* Existing words, tentatively defined here to retain GP-relative ownership.
 * Keep external linkage: reconstruction does not establish private linkage. */
s32 gWideScreenFadeUpdatesRemaining;
s32 gWideScreenFadeIntensityStep;
s32 gWideScreenFadeMode;
s32 gWideScreenFadeSkipDraw;

/* Internal call within this slice, but the original external linkage remains. */
void SetWideScreenEnvironments(void);
void SetVal8005E278(s32 arg0);
void SetVal8005E27C(s32 arg0);

s32 BeginWideScreenFadeFeedback(s32 intensityStep) {
    gWideScreenFadeMode = 2;
    gWideScreenFadeIntensityStep = intensityStep;
    gWideScreenFadeUpdatesRemaining = 0xFF / intensityStep;
    gWideScreenFadeSkipDraw = 1;
    return 1;
}

s32 BeginWideScreenFadeIncrease(s32 intensityStep) {
    gWideScreenFadeMode = 1;
    gWideScreenFadeIntensityStep = intensityStep;
    gWideScreenFadeUpdatesRemaining = 0xFF / intensityStep;
    return 1;
}

s32 IsWideScreenFadeComplete(void) {
    return gWideScreenFadeUpdatesRemaining == 0;
}

void UpdateAndDrawWideScreenFade(void) {
    s32 packetBufferIndex;
    u8 intensity;
    POLY_F4 *fadeQuad;
    DR_MODE *drawMode;

    if (gWideScreenFadeUpdatesRemaining <= 0) {
        gWideScreenFadeUpdatesRemaining = 0;
        return;
    }

    /* The shared context has two existing struct views. The original test
     * selects packet pair 1 for any address other than the first context. */
    packetBufferIndex = (s32)gActiveRenderContext != (s32)&gRenderContexts;
    SetWideScreenEnvironments();
    SetVal8005E278(0);
    SetVal8005E27C(0);

    /* Environment/handshake changes still happen on the skipped draw. */
    if (gWideScreenFadeSkipDraw == 1) {
        gWideScreenFadeSkipDraw = 0;
        return;
    }

    if (gWideScreenFadeMode == 1) {
        /* Low-byte complement is 255 - step*remaining for normal inputs;
         * retain the byte wrapping rather than inventing saturation. */
        intensity = ~(gWideScreenFadeIntensityStep * gWideScreenFadeUpdatesRemaining) & 0xFF;
    } else {
        intensity = (u8)gWideScreenFadeIntensityStep;
    }

    fadeQuad = &((POLY_F4 *)&gWideScreenFadeQuadStorage)[packetBufferIndex];
    drawMode = &((DR_MODE *)&gWideScreenFadeDrawModeStorage)[packetBufferIndex];

    setPolyF4(fadeQuad);
    setRGB0(fadeQuad, intensity, intensity, intensity);
    setXYWH(fadeQuad, 0, 0, 0x280, 0x1E0);
    setSemiTrans(fadeQuad, 1);
    /* ABR 2 subtracts; 0x100 also selects 16-bit texture depth (untextured
     * quad). Enable dithering and drawing in the displayed area. */
    setDrawMode(drawMode, 1, 1, 0x140, 0);

    /* Head insertion makes the draw-mode packet execute before the quad. */
    addPrim(gFrameOrderingTable, fadeQuad);
    addPrim(gFrameOrderingTable, drawMode);

    gWideScreenFadeUpdatesRemaining--;
    if (gWideScreenFadeUpdatesRemaining < 0) {
        gWideScreenFadeUpdatesRemaining = 0;
    }
}

/* Install a common origin and 640x480 extent. SDK defaults leave background
 * clearing disabled; this helper neither clears VRAM nor waits for the GPU. */
void SetWideScreenEnvironments(void) {
    DISPENV displayEnv;
    DRAWENV drawEnv;

    SetDefDispEnv(&displayEnv, 0, 0, 0x280, 0x1E0);
    SetDefDrawEnv(&drawEnv, 0, 0, 0x280, 0x1E0);
    PutDispEnv(&displayEnv);
    PutDrawEnv(&drawEnv);
}
