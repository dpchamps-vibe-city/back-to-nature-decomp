#ifndef SCREEN_COLOR_RAMP_H
#define SCREEN_COLOR_RAMP_H

#include "common.h"

/*
 * Screen-color ramp helpers; ovl_11 must be resident.
 * State is gScreenColorRamp (original EXE RAM address 0x8007AF80), also
 * preserved by ovl_15 save/restore. These functions do not allocate it.
 */

/* Draw the CURRENT RGB through the full-screen semitransparent quad helper,
 * unless the channel sum is zero, then advance by one stored step while
 * elapsedUpdates < durationUpdates. Rendering therefore precedes advancement.
 * Counts measure calls to this function, not elapsed time or unconditional
 * frames: the caller can suppress updates. Color values use 12 fractional bits.
 * No clamping or final target snap is performed.
 * Original identity: ovl_11:0x800BFF74. */
void Ovl11DrawAndStepScreenColorRamp(void);

/* Keep the current RGB, reset elapsedUpdates, and set a new duration and
 * per-channel step: (target - current) / updateCount, truncated toward zero.
 * targetRgbQ12 points to three signed 32-bit RGB components, each with 12
 * fractional bits. The caller must supply a positive updateCount and values
 * whose differences fit s32; neither constraint is checked here.
 * The target is read immediately and is not retained. Fractional remainders
 * are discarded, so the ramp can finish short of the requested target.
 * Original identity: ovl_11:0x800C0010. */
void Ovl11SetScreenColorRampTarget(s32 updateCount, const s32 targetRgbQ12[3]);

#endif
