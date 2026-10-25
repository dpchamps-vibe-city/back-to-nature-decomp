#ifndef OVL27_BACKDROP_SPRITE_POOL_H
#define OVL27_BACKDROP_SPRITE_POOL_H

#include "common.h"

/* Stage archive 0, offset 0x2000, length 0x23000 at graphics heap+0x4290.
 * Begin waits for GPU completion and clears the active 0x800-entry OT.
 * Poll returns 0 until ready, then uploads images from heap+0x4FAC, copies
 * the 0xD1C-byte sprite header to 800C4C00, initializes eight stride-0x30
 * sources at 800C4A80..800C4BD0 and selects their initial animations; returns
 * 1. Requires the graphics heap and a compatible CD-read lifecycle. It does
 * not reset the six effect counters or the two backdrop phase halfwords. */
void Ovl27BeginBackdropStageLoad(void);
s32 Ovl27PollBackdropSpritePoolLoad(void);

/* Require the initialized pool and current render context. Fixed draws use
 * source 800C4BD0, frame 0, OT byte offset +0x88. Suffixes are the demonstrated
 * decimal animation ids, not guessed scene-feature names. x/y are pixels.
 * Anim36 additionally selects its position using the shared scene state. */
void Ovl27DrawBackdropAnim30(void);
void Ovl27DrawBackdropAnim32(s16 y);
void Ovl27DrawBackdropAnim31(s16 y);
void Ovl27DrawBackdropAnim33(s16 x);
void Ovl27DrawBackdropAnim36(void);
void Ovl27DrawBackdropAnim34(void);
void Ovl27DrawBackdropAnim35(void);

/* Draw two 3x3 tiled layers (animation 37) at OT+0x90, then animation 29 at
 * OT+0x94. Phase 800C4A4C advances only when D_8006C840 bit 0 is set;
 * 800C4A4E advances every call. Both reduce modulo 960; sine/cosine sway
 * has amplitude 12 pixels. The separate halfword temporaries retain the
 * original truncation and draw order, not a generalized tile-loop API. */
void Ovl27DrawTiledBackdrop(void);

/* Advance/draw slots 0/1/2 or the slot-3/4/5 group at OT+0x8C. Counters at
 * 800C4A2C+2*i pair with sources at 800C4A80+0x30*i. Each call increments and
 * reduces the signed-halfword phase modulo 1800, draws, then on threshold
 * 90/60/60/15/20/30 resets and randomly selects the next animation. Thus
 * visible animation changes take effect after that call's draw. Repeated
 * draws of one source also advance its animation repeatedly. Not reentrant. */
void Ovl27UpdateEffectSlot0(void);
void Ovl27UpdateEffectSlot1(void);
void Ovl27UpdateEffectSlot2(void);
void Ovl27UpdateEffectSlots3To5(void);

/* Draw the text-bank entry selected by the four-choice table at 800C4A50
 * and its panel at fixed screen positions. index is unchecked (scene callers
 * supply 0..3). Requires the loaded text bank; consumes packet work. */
void Ovl27DrawTextBankLabel(s16 index);

#endif
