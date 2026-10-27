#ifndef OVL11_ACTIVE_RECORD_SPRITE_PACKET_H
#define OVL11_ACTIVE_RECORD_SPRITE_PACKET_H

#include "common.h"

/* Original ovl_11:800E559C. Emit the shared active record's sprite at signed
 * screen-pixel coordinates into one initialized OT entry. The slot-texture
 * selection/copy must already have installed its descriptor and VRAM pixels.
 * Width/height are pixels; U is four times the signed texture-word X remainder
 * modulo 64 and V is the descriptor's low byte. No clipping is performed.
 *
 * Reserves 32 packet-work bytes (20 for SPRT, 12 for the page command), links
 * the draw-page command before the sprite, uses neutral RGB (128,128,128),
 * disables semitransparency and sets dither/draw-in-display on. The page is
 * derived from the descriptor's CLUT origin, as in the original. Advances
 * the shared packet cursor and modifies the OT link, not the descriptor.
 * Requires an initialized render context and sufficient packet capacity. */
void Ovl11DrawActiveRecordSpritePacket(void *otEntry, s16 screenX, s16 screenY);

#endif
