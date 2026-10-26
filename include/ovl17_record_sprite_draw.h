#ifndef OVL17_RECORD_SPRITE_DRAW_H
#define OVL17_RECORD_SPRITE_DRAW_H

#include "common.h"
#include "game_types.h"

/* Resident ovl_17 and initialized render/record state required. record is a
 * 0x50-stride record view: +10 is a signed 12-bit-fraction position, +14 points
 * to {y, textureOffsetY, otIndex} halfwords, +20 is its sprite. Screen X is
 * 160 - trunc((position-viewOriginX)/4096), narrowed to s16. OT indices are
 * unchecked. Both draw the companion glyph (frame 1); bit 0 of record flags
 * additionally draws the threshold indicator at (x-56,y-56), on OT slot 4.
 * Its animation is selected from work+CA: >=170 -> 0, >=3 -> 1, else 2.
 * With bit 0 clear, the record's texture offsets are reset from its descriptor.
 * These operations mutate sprite state and append packets to the frame. */

/* Animated quads advance both record and optional indicator animation.
 * The unsigned ((tick+1)/10)*10-1 == tick test calls a periodic position
 * service at ticks 9,19,... and also UINT_MAX because of wraparound. The
 * tick counter itself is not advanced here. */
void Ovl17DrawRecordSpriteQuads(Recon_ovl_17_func_800BAD9C_A0View *record, s32 viewOriginX);

/* Rectangles use the current unsigned animation/frame bytes at +24/+25;
 * no animation update or periodic position service is performed. */
void Ovl17DrawRecordSpriteRects(Recon_ovl_17_func_800BAD9C_A0View *record, s32 viewOriginX);

#endif
