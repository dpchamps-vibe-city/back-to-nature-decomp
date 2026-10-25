#ifndef SCREEN_PRIMITIVES_H
#define SCREEN_PRIMITIVES_H

#include "common.h"

/*
 * Untextured screen-space drawing. Coordinates and extents are signed 16-bit
 * pixel values; rectangle vertices use x + width and y + height, with no
 * clipping or validation here. Colors are packed 0xRRGGBB (not GPU byte order).
 * Any nonzero semitransparent value sets the primitive's blend-enable bit.
 * Except WithBlendMode, these calls leave the GPU's current blend mode alone.
 *
 * Each call reserves bytes from packet_work and prepends a packet to the
 * supplied ordering-table slot. Initialize the active render context and
 * packet cursor first; preserve packet storage until the GPU consumes it.
 * These routines construct commands; they do not submit or synchronize them.
 * The historical s32/u32 slot-pointer distinctions are retained in the ABI.
 */

/* exe:0x80014E90: TILE_1, 12 packet bytes. */
void DrawScreenPoint(u32 *ot, s16 x, s16 y, s32 color, s16 semitransparent);

/* exe:0x80014F0C: LINE_F2, 16 packet bytes; both endpoints are coordinates. */
void DrawScreenLine(u32 *ot, s16 x0, s16 y0, s16 x1, s16 y1,
                    s32 color, s16 semitransparent);

/* exe:0x80014FAC: POLY_G4, 36 bytes. Colors follow GPU vertex order:
 * top-left, top-right, bottom-left, bottom-right. */
void DrawScreenGradientRect(s32 *ot, s16 x, s16 y, s16 width, s16 height,
                            s32 topLeft, s32 topRight, s32 bottomLeft,
                            s32 bottomRight, s16 semitransparent);

/* exe:0x80015074: POLY_F4, 24 bytes. */
void DrawScreenFlatRect(s32 *ot, s16 x, s16 y, s16 width, s16 height,
                        s32 color, s16 semitransparent);

/* exe:0x80015114: TILE, 16 bytes. Uses the GPU rectangle command rather
 * than a polygon; keep this distinct from DrawScreenFlatRect. */
void DrawScreenTileRect(u32 *ot, s16 x, s16 y, s16 width, s16 height,
                        s32 color, s16 semitransparent);

/* exe:0x800151B4: POLY_F4 plus DR_TPAGE, 32 bytes. blendMode & 3 selects
 * the PSY-Q ABR mode (0 average, 1 add, 2 subtract, 3 add quarter source).
 * Prepending the tpage AFTER the quad makes it execute BEFORE the quad.
 * Also sets texture-page bits to zero, dithering on, draw-to-display off;
 * does not restore the previous draw mode afterward. */
void DrawScreenFlatRectWithBlendMode(s32 *ot, s16 x, s16 y, s16 width,
                                     s16 height, s32 color,
                                     s16 semitransparent, u8 blendMode);

#endif
