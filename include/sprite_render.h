#ifndef SPRITE_RENDER_H
#define SPRITE_RENDER_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/* Frame submission through the active packet-work cursor. Source/OT
 * addresses retain their historical s32 ABI. Indices are narrowed to bytes,
 * x/y are signed pixel offsets. Calls append/prepend packets, not GPU submits;
 * initialize the active render context/cursor and retain packet storage.
 * The Rects path interleaves SPRT and DR_MODE packets through one cursor.
 * The Quads path forwards control6 twice, then control7/control8, to the
 * low-level builder; these controls keep neutral names until established.
 */
void DrawSpriteFrameRects(s32 otAddress, s32 sourceAddress, s32 animation,
                          s32 frame, s16 x, s16 y);
void DrawSpriteFrameQuads(s32 otAddress, s32 sourceAddress, s32 animation,
                          s32 frame, s16 x, s16 y, s32 control6,
                          s32 control7, u16 control8);

/* Draw the current animation/frame, THEN call UpdateSpriteAnimation once.
 * The Quads wrapper takes control6 from source+8 and clears controls 7/8.
 * Rects uses the ordinary packet-work cursor; Build... uses the two supplied
 * packet-cursor addresses. The latter does not update the global cursor.
 * Quads x/y retain their s32 ABI and are narrowed by the implementation.
 */
void DrawSpriteAnimationQuadsAndUpdate(s32 otAddress, SpriteSourceData *source,
                                      s32 x, s32 y);
void DrawSpriteAnimationRectsAndUpdate(s32 otAddress, SpriteSourceData *source,
                                      s16 x, s16 y);
void BuildSpriteAnimationRectsAndUpdate(s32 otAddress, s32 spriteCursorAddress,
                                       s32 drawModeCursorAddress,
                                       SpriteSourceData *source, s16 x, s16 y);
/* Same low-level builder as below, with the address-as-s32 wrapper ABI. */
void BuildSpriteRectPacketsFromAddresses(s32 otAddress, s32 spriteCursorAddress,
                                         s32 drawModeCursorAddress,
                                         s32 sourceAddress, u8 animation,
                                         u8 frame, s16 x, s16 y);

/* Low-level sprite packet builders. Source views alias the 0x30-byte
 * SpriteSourceData initialized by sprite_source, preserving each renderer's
 * observed halfword signedness. Animation/frame indices and table contents
 * are unchecked. Inputs are screen-pixel offsets; source geometry comes
 * from the selected frame. Caller supplies sufficient packet storage and
 * a live ordering-table slot, and retains it until GPU consumption.
 * These functions construct packets; they do not submit or synchronize them.
 */

#endif
