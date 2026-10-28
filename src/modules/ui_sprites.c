/*
 * Cached UI sprite draws and their two shared animation clocks.
 * All wrappers use the same source/header cache; source indices are retained
 * in names where the asset's visual meaning is not established. Drawing does
 * not advance either clock. See ui_sprites.h for the caller contract.
 */

#include "common.h"
#include "game_types.h"
#include "ui_sprites.h"
#include "sprite_source.h"
#include "sprite_render.h"

/* Existing byte storage, defined here for the original GP-relative accesses. */
u8 gUiSpriteFourFramePhase;
u8 gUiSpriteTwoFramePhase;

void DrawUiSprite2A(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x2A, 0, x, y);
}

void DrawUiSprite27Frame0(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x27, 0, x, y);
}

void DrawUiSprite27Frame2(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x27, 2, x, y);
}

void DrawUiSprite27Frame1(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x27, 1, x, y);
}

void DrawUiSprite27Frame3(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x27, 3, x, y);
}

void StepUiSpriteFourFramePhase(s16 resetOrStep) {
    u8 quotient;

    if (resetOrStep != -1) {
        /* Narrow the increment before division, and keep a byte quotient.
         * These are separate byte operations, not an unbounded frame count. */
        gUiSpriteFourFramePhase++;
        quotient = gUiSpriteFourFramePhase / 120U;
        gUiSpriteFourFramePhase -= quotient * 120;
        return;
    }
    gUiSpriteFourFramePhase = 0;
}

void DrawAnimatedUiSprite29(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x29, gUiSpriteFourFramePhase / 30, x, y);
}

void DrawAnimatedUiSprite28(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x28, gUiSpriteFourFramePhase / 30, x, y);
}

void DrawAnimatedUiSprite0D(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x0D, gUiSpriteFourFramePhase / 30, x, y);
}

void DrawAnimatedUiSprite0C(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x0C, gUiSpriteFourFramePhase / 30, x, y);
}

void DrawUiSprite0BFrame1(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x0B, 1, x, y);
}

void DrawUiSprite18Frame(s32 otAddress, s32 x, s32 y, s32 frame) {
    /* This entry narrows its own word arguments, including old-style calls. */
    s16 x16 = x;
    s16 y16 = y;
    s16 frame16 = frame;

    DrawCachedUiSprite(otAddress, 0x18, frame16, x16, y16);
}

void DrawUiSprite1E(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x1E, 0, x, y);
}

void StepUiSpriteTwoFramePhase(s16 resetOrStep) {
    u8 quotient;

    if (resetOrStep != -1) {
        gUiSpriteTwoFramePhase++;
        quotient = gUiSpriteTwoFramePhase / 60U;
        gUiSpriteTwoFramePhase -= quotient * 60;
        return;
    }
    gUiSpriteTwoFramePhase = 0;
}

void DrawAnimatedUiSprite07(s32 otAddress, s16 x, s16 y) {
    DrawCachedUiSprite(otAddress, 0x07, gUiSpriteTwoFramePhase / 30, x, y);
}

void DrawUiSprite2BFrame(s32 otAddress, s16 x, s16 y, s16 frame) {
    DrawCachedUiSprite(otAddress, 0x2B, frame, x, y);
}

void DrawCachedUiSprite(s32 otAddress, s32 animation, s32 frame, s32 x, s16 y) {
    s16 x16;

    x16 = (s16)x;
    if (gSharedSpriteSource.field_14 != (s32)&gSharedSpriteData) {
        InitSpriteSourceData(&gSharedSpriteSource, (SpriteDataHeader *)&gSharedSpriteData);
    }
    DrawSpriteFrameRects(otAddress, (s32)&gSharedSpriteSource,
                         animation & 0xFF, frame & 0xFF, x16, y);
}
