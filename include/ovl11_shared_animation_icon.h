#ifndef OVL11_SHARED_ANIMATION_ICON_H
#define OVL11_SHARED_ANIMATION_ICON_H

#include "common.h"

/* Append SPRT/DR_MODE packets for animation 24 of gSharedSpriteSource.
 * otAddress is an ordering-table slot address, x/y narrow to signed screen
 * pixels, frame to its low byte (unchecked). Each call resets the shared
 * source to animation 24/frame 0, but draws the explicitly requested frame.
 * Requires an initialized shared sprite bank and live packet cursor/storage.
 * Does not advance animation, synchronize the GPU or present the frame. */
void Ovl11DrawSharedAnimationIcon(s32 otAddress, s32 x, s32 y, s32 frame);

#endif
