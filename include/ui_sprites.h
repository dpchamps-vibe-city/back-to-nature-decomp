#ifndef UI_SPRITES_H
#define UI_SPRITES_H

#include "common.h"

/* UI sprites from gSharedSpriteData, loaded by the current scene. The draw
 * functions lazily bind gSharedSpriteSource to that header, then append SPRT
 * and draw-mode packets using the active packet-work cursor. Initialize the
 * render context/cursor and retain packet storage until GPU consumption.
 * otAddress names an ordering-table slot; x/y are signed pixel offsets.
 * Animation/frame indices are unchecked and narrowed to bytes by the core.
 * Loading new data at the same header address does not invalidate the cache.
 * The shared source is also used by other render services; it is not private.
 * Draws do not advance clocks, submit the GPU list, or wait for completion.
 * Hex suffixes identify demonstrated animation indices, not guessed icons.
 */
void DrawCachedUiSprite(s32 otAddress, s32 animation, s32 frame, s32 x, s16 y);
void DrawUiSprite2A(s32 otAddress, s16 x, s16 y);
void DrawUiSprite27Frame0(s32 otAddress, s16 x, s16 y);
void DrawUiSprite27Frame2(s32 otAddress, s16 x, s16 y);
void DrawUiSprite27Frame1(s32 otAddress, s16 x, s16 y);
void DrawUiSprite27Frame3(s32 otAddress, s16 x, s16 y);
void DrawAnimatedUiSprite29(s32 otAddress, s16 x, s16 y);
void DrawAnimatedUiSprite28(s32 otAddress, s16 x, s16 y);
void DrawAnimatedUiSprite0D(s32 otAddress, s16 x, s16 y);
void DrawAnimatedUiSprite0C(s32 otAddress, s16 x, s16 y);
void DrawUiSprite0BFrame1(s32 otAddress, s16 x, s16 y);
/* This entry narrows x/y/frame inside the implementation; preserve word ABI. */
void DrawUiSprite18Frame(s32 otAddress, s32 x, s32 y, s32 frame);
void DrawUiSprite1E(s32 otAddress, s16 x, s16 y);
void DrawAnimatedUiSprite07(s32 otAddress, s16 x, s16 y);
void DrawUiSprite2BFrame(s32 otAddress, s16 x, s16 y, s16 frame);

/* -1 resets the selected phase byte; EVERY other value advances it once.
 * Units are calls, not seconds or elapsed frames. Phases wrap at 120/60;
 * animated draws select phase/30 (four/two frames). Increment narrows to a
 * byte before division, even if another client has supplied an invalid phase.
 */
void StepUiSpriteFourFramePhase(s16 resetOrStep);
void StepUiSpriteTwoFramePhase(s16 resetOrStep);

#endif
