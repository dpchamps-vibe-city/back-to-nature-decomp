#ifndef SCREEN_FADE_H
#define SCREEN_FADE_H

#include "common.h"

/*
 * Single shared full-screen fade, driven by the frame loop. Starting another
 * effect replaces its counters/settings, not a queued transition. All routines
 * return void except the queries, which return s32 values. Initialize the
 * double-buffered render contexts/ordering tables before drawing.
 *
 * stepCount counts driver updates, not milliseconds or guaranteed VBlanks.
 * The ramp starts accept s32 but narrow stepCount and otSelector to s16 and
 * blendMode to s16. Use positive step counts within signed-halfword range
 * (at most 32766 for increase). Inputs are not validated or lower-clamped.
 * Zero otSelector selects the frame OT; nonzero selects the scene OT at
 * active-context +0xD8, byte offset 0x14. GPU ABR uses blendMode's low two bits:
 * 0 average, 1 add, 2 subtract, 3 add quarter. Thus decreasing/increasing the
 * overlay is fade-in/fade-out only with the usual subtractive blend mode 2.
 */

/* Frame-loop service: advance counters, then link a 320x240 gray quad and draw
 * mode packet if needed. Feedback temporarily disables background clears and
 * enables drawing in the displayed area; completion restores those flags and
 * clears the 320x480 framebuffer. Enters when the effect is incomplete OR the
 * draw latch equals 1, and sets the latch to 1 before any skip/zero-intensity
 * return. Servicing a completed effect need not emit a quad. exe:0x80012D30. */
void UpdateAndDrawScreenFade(void);

/* Draw only, using a separate packet pair and the active context's +0xDC OT.
 * RGB inputs are unsigned Q12 channel values: shift by 12, then keep the low
 * byte (no saturation). GPU blending subtracts this color from the framebuffer.
 * Does not change fade counters. Original identity: exe:0x8001316C. */
void DrawScreenSubtractOverlay(u32 redQ12, u32 greenQ12, u32 blueQ12);

/* Completed mode-1 state: counter 0, denominator 60, blend 2, frame OT.
 * Does not clear the draw latch. Original identity: exe:0x8001328C. */
void ResetScreenFade(void);

/* Counter starts at stepCount-1 and decreases toward zero; denominator is
 * stepCount. Original identity: exe:0x800132B8. */
void BeginScreenFadeDecrease(s32 stepCount, s32 otSelector, s32 blendMode);

/* Counter starts at zero; denominator is stepCount+1. Completion uses another
 * one-step offset, so stepCount is not an exact duration. exe:0x800132F0. */
void BeginScreenFadeIncrease(s32 stepCount, s32 otSelector, s32 blendMode);

/* Framebuffer-feedback subtraction, fixed blend 2 and frame OT. Supported
 * nonnegative counts are capped at 30, then incremented for the denominator.
 * The first driver call is skipped; later pacing uses the existing cadence
 * flag (one increment per call or every other call). Negative input is not
 * clamped and -1 would leave a zero divisor. Also clears the external values
 * at 0x8005E278/0x8005E27C. Original identity: exe:0x80013328. */
void BeginScreenFadeFeedback(s16 stepCount);

/* 1 when complete, 0 otherwise; no state changes. Mode-specific predicates
 * are preserved, including feedback's counter == denominator+3 test and the
 * idle fallback. Poll while servicing the frame loop. exe:0x80013394. */
s32 IsScreenFadeComplete(void);

/* 1 decreasing, 2 increasing, 0 otherwise (including feedback, not just idle).
 * This is not the completion query. Original identity: exe:0x80013400. */
s32 GetScreenFadeRampMode(void);

/* Store the raw s32 latch (no Boolean normalization). Only value 1 keeps the
 * driver eligible after completion. Setting 0 does not stop an incomplete
 * effect: the next service re-latches it. To stop completed-effect servicing,
 * release the latch after completion. No counters, mode, packets or display
 * state are changed here; reset/start routines also leave the latch intact.
 * Original identity: exe:0x80013438. */
void SetScreenFadeDrawLatch(s32 latchValue);

/* Return the stored latch word unchanged; this is neither a completion query
 * nor a guarantee that a quad was drawn. Original identity: exe:0x80013444. */
s32 GetScreenFadeDrawLatch(void);

#endif
