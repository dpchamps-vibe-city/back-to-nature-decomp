#ifndef WIDE_SCREEN_FADE_H
#define WIDE_SCREEN_FADE_H

#include "common.h"

/*
 * One shared 640x480 subtractive effect, independent of screen_fade.h's
 * 320x240 effect. The retained control entrypoints have no identified retail
 * callers. Starting an effect replaces its mode/step/count, not a queue.
 *
 * intensityStep is an integer channel increment, normally 1..255. Both
 * starters set remaining draws to floor(255 / intensityStep) and return 1
 * unconditionally. There is no input validation: zero divides by zero;
 * negative or >255 inputs do not describe a normal fade. Drawing truncates
 * intensity to a byte, without saturation. Counts are service calls that
 * actually emit packets, not milliseconds or guaranteed display refreshes.
 */

/* Repeated fixed-intensity subtraction from the retained framebuffer. Skip
 * the next active service call's draw and decrement, but not its environment
 * setup/handshake writes. Original identity: exe:0x80013450. */
s32 BeginWideScreenFadeFeedback(s32 intensityStep);

/* Intensity is the low byte of ~(intensityStep * remaining), computed before
 * decrementing remaining. Does NOT clear a pending skip from the feedback
 * starter. The final emitted intensity is 255-step for normal inputs, not a
 * forced full-black endpoint. Original identity: exe:0x80013488. */
s32 BeginWideScreenFadeIncrease(s32 intensityStep);

/* 1 exactly when remaining == 0, otherwise 0; no state changes. A negative
 * count becomes zero only when the service clamps it. exe:0x800134B8. */
s32 IsWideScreenFadeComplete(void);

/* Frame-loop service. Requires initialized render contexts and frame OT,
 * with GPU submission/reuse synchronized by the caller. If remaining <= 0,
 * clamp to zero and do nothing else. Otherwise install 640x480 display/draw
 * environments at (0,0), clear the existing 8005E278/8005E27C handshake values,
 * then (unless skipped) link a gray subtractive quad and draw-mode packet and
 * decrement remaining. Uses the packet pair selected by the active context;
 * it does not submit or wait for the OT. Completion does not restore the
 * previous environments or clear the framebuffer. exe:0x800134C4. */
void UpdateAndDrawWideScreenFade(void);

#endif
