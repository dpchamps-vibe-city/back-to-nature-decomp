/*
 * Full-screen fade rendering and controls. Shared GP-relative state, paired
 * start/query callers and neighboring double-buffered packet pools support
 * this reconstructed module, not a proven original file boundary.
 *
 * Ramp modes scale a gray overlay by counter/denominator. Feedback mode instead
 * subtracts a fixed gray step while retaining the previous framebuffer image.
 * The frame loop calls UpdateAndDrawScreenFade; overlay code starts effects
 * and polls IsScreenFadeComplete. See screen_fade.h for timing and input limits.
 */

#include "common.h"
#include "screen_fade.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void SetVal8005E29C(s32 arg0);
void SetVal8005E27C(s32 arg0);
u8 GetFlag8005E274(void);
void func_80012A68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6);
void SetVal8005E278(s32 arg0);

/* Tentative definitions retain the original GP-relative accesses. These names
 * label existing storage; no state is made static or moved into a new struct. */
s32 D_8005E28C;                  /* Install feedback display environment once. */
s16 gScreenFadeMode;             /* 0 idle, 1 decrease, 2 increase, 3 feedback. */
s32 gScreenFadeSkipUpdate;
s32 D_8005E29C;                  /* External latch allows continued drawing. */
s16 gScreenFadeBlendMode;        /* GPU ABR bits, used only by ramp modes. */
s32 D_8005E3C8;                  /* Alternating 0/1 feedback pacing phase. */
s16 gScreenFadeCounter;
s16 gScreenFadeDenominator;
s32 gScreenFadeOtSelector;

void UpdateAndDrawScreenFade(void) {
    POLY_F4 *fadeQuad;
    DR_MODE *drawMode;
    s32 *ot;
    s32 blendMode;
    s32 intensity;
    s32 intensityStepQ12;
    s32 channelValue;
    s32 product;

    intensityStepQ12 = 0;
    if (IsScreenFadeComplete() != 1 || D_8005E29C == 1) {
        SetVal8005E29C(1);
        D_8005E3C8 = (u32)D_8005E3C8 < 1;
        if (gScreenFadeSkipUpdate == 1) {
            gScreenFadeSkipUpdate = 0;
            return;
        }
        if (gScreenFadeMode == 1) {
            if (gScreenFadeCounter > 0)
                gScreenFadeCounter = gScreenFadeCounter - 1;
        } else if (gScreenFadeMode == 2) {
            if (gScreenFadeCounter <= gScreenFadeDenominator + 6)
                gScreenFadeCounter = gScreenFadeCounter + 1;
        } else if (gScreenFadeMode == 3) {
            SetVal8005E27C(0);
            if (gScreenFadeDenominator > gScreenFadeCounter) {
                if (GetFlag8005E274() == 0)
                    gScreenFadeCounter = gScreenFadeCounter + D_8005E3C8;
                else
                    gScreenFadeCounter = gScreenFadeCounter + 1;
            }
        } else {
            goto draw_level;
        }
        if (gScreenFadeMode == 3) {
            if (gScreenFadeDenominator == gScreenFadeCounter) {
                gScreenFadeMode = 0;
                gRenderContexts[0].unk18 = 1;
                gRenderContexts[1].unk18 = 1;
                gRenderContexts[0].unk17 = 0;
                gRenderContexts[1].unk17 = 0;
                func_80012A68(0, 0, 0x140, 0x1E0, 0, 0, 0);
            } else if (gScreenFadeDenominator < gScreenFadeCounter) {
                gScreenFadeCounter = gScreenFadeDenominator;
            }
            intensityStepQ12 = 0xFF000 / gScreenFadeDenominator;
            intensity = 0xFF;
        } else {
draw_level:
            if (gScreenFadeCounter < gScreenFadeDenominator + 1)
                goto scale_level;
            intensity = 0xFF;
            if ((s16)gScreenFadeMode != 2 || gScreenFadeCounter <= gScreenFadeDenominator + 6)
                gScreenFadeMode = 0;
            goto emit_level;
scale_level:
            if (gScreenFadeDenominator != 0) {
                intensityStepQ12 = 0xFF000 / gScreenFadeDenominator;
                product = intensityStepQ12 * gScreenFadeCounter;
                /* Signed division by 4096 truncates toward zero. */
                if (product < 0)
                    product += 0xFFF;
                channelValue = product >> 12;
                if (channelValue < 0)
                    channelValue = 0;
                if (channelValue >= 0x100)
                    channelValue = 0xFF;
                intensity = channelValue & 0xFF;
                goto emit_level;
            }
            intensity = 0;
            gScreenFadeMode = 0;
emit_level:
            ;
        }
        if (gScreenFadeOtSelector == 0)
            ot = gFrameOrderingTable;
        else
            ot = (s32 *)(gActiveRenderContext->field_D8 + 0x14);
        if (intensity == 0)
            return;
        fadeQuad = (POLY_F4 *)&D_8005E8E0[D_8005E3A4];
        drawMode = (DR_MODE *)&D_8005E910[D_8005E3A4];
        if (gScreenFadeMode == 3) {
            if (D_8005E28C == 1) {
                D_8005E28C = 0;
                PutDispEnv((DISPENV *)&gRenderDisplayEnvView[D_8005E3A4]);
            }
            /* Preserve the framebuffer instead of clearing it between steps;
             * enable drawing into the displayed area for the feedback effect. */
            gRenderContexts[0].unk18 = 0;
            gRenderContexts[1].unk18 = 0;
            gRenderContexts[0].unk17 = 1;
            gRenderContexts[1].unk17 = 1;
            channelValue = intensityStepQ12;
            if (channelValue < 0)
                channelValue += 0xFFF;
            channelValue >>= 12;
            blendMode = 2;
        } else {
            channelValue = intensity;
            blendMode = gScreenFadeBlendMode;
        }
        setPolyF4(fadeQuad);
        setRGB0(fadeQuad, channelValue, channelValue, channelValue);
        setXYWH(fadeQuad, 0, 0, 0x140, 0xF0);
        setSemiTrans(fadeQuad, 1);
        setDrawMode(drawMode, 1, 1, ((blendMode & 3) << 5), 0);
        addPrim(ot, fadeQuad);
        addPrim(ot, drawMode);
    }
}

void DrawScreenSubtractOverlay(u32 redQ12, u32 greenQ12, u32 blueQ12) {
    POLY_F4 *poly;
    DR_MODE *dr;

    poly = (POLY_F4 *)&D_8005E930[D_8005E3A4];
    dr = (DR_MODE *)&D_8005E960[D_8005E3A4];

    setPolyF4(poly);
    setRGB0(poly, redQ12 >> 12, greenQ12 >> 12, blueQ12 >> 12);
    setXYWH(poly, 0, 0, 0x140, 0xF0);
    setSemiTrans(poly, 1);
    setDrawMode(dr, 1, 1, 0x40, 0);
    addPrim((s32 *)gActiveRenderContext->field_DC, poly);
    addPrim((s32 *)gActiveRenderContext->field_DC, dr);
}

void ResetScreenFade(void) {
    s16 setting;

    setting = 1;
    gScreenFadeMode = setting;
    setting = 0x3C;
    gScreenFadeDenominator = setting;
    gScreenFadeCounter = 0;
    gScreenFadeSkipUpdate = 0;
    setting = 2;
    gScreenFadeBlendMode = setting;
    gScreenFadeOtSelector = 0;
}

void BeginScreenFadeDecrease(s32 stepCount, s32 otSelector, s32 blendMode) {
    s32 modeOrCounter;
    s16 steps;
    s16 selectedOt;

    /* Keep the shared mode/counter temporary: it also preserves the original
     * constant and narrowing order in these small initializers. */
    modeOrCounter = 1;
    steps = (s16) stepCount;
    gScreenFadeMode = (s16) modeOrCounter;
    modeOrCounter = steps - 1;
    selectedOt = (s16) otSelector;
    gScreenFadeCounter = (s16) modeOrCounter;
    gScreenFadeDenominator = steps;
    gScreenFadeSkipUpdate = 0;
    gScreenFadeBlendMode = blendMode;
    gScreenFadeOtSelector = selectedOt;
}

void BeginScreenFadeIncrease(s32 stepCount, s32 otSelector, s32 blendMode) {
    s16 steps;
    s16 selectedOt;
    s32 denominator;

    steps = (s16) stepCount;
    denominator = steps + 1;
    selectedOt = (s16) otSelector;
    gScreenFadeMode = 2;
    gScreenFadeCounter = 0;
    gScreenFadeDenominator = (s16) denominator;
    gScreenFadeSkipUpdate = 0;
    gScreenFadeBlendMode = blendMode;
    gScreenFadeOtSelector = selectedOt;
}

void BeginScreenFadeFeedback(s16 stepCount) {
    s32 denominator;
    s32 setting;

    setting = 3;
    gScreenFadeMode = setting;
    gScreenFadeCounter = 0;
    if (stepCount >= 0x1F) {
        stepCount = 0x1E;
        gScreenFadeDenominator = stepCount;
    }
    denominator = stepCount + 1;
    gScreenFadeDenominator = denominator;
    D_8005E28C = 1;
    gScreenFadeSkipUpdate = 1;
    SetVal8005E278(0);
    SetVal8005E27C(0);
    setting = 2;
    gScreenFadeBlendMode = setting;
    gScreenFadeOtSelector = 0;
}

s32 IsScreenFadeComplete(void) {
    if (gScreenFadeMode == 1) {
        return gScreenFadeCounter < 1;
    }
    if (gScreenFadeMode == 3) {
        /* Preserve this exact predicate; it is not a generic >= target test.
         * The normal feedback driver also reports completion by going idle. */
        return gScreenFadeCounter == (gScreenFadeDenominator + 3);
    }
    if (gScreenFadeMode == 2) {
        return gScreenFadeCounter >= (gScreenFadeDenominator + 1);
    }
    return 1;
}

s32 GetScreenFadeRampMode(void) {
    switch (gScreenFadeMode) {
    case 1:
        return 1;
    case 2:
        return 2;
    default:
        return 0;
    }
}
