#ifndef OVL11_OBJECT_RENDER_H
#define OVL11_OBJECT_RENDER_H

#include "common.h"

/* Projected overlay-11 object rendering. object is the existing initialized
 * object base, not just a coordinate pointer: flags are at +0x34, world XYZ
 * at +0x38/+0x3C/+0x40, sprite state at +0x78 and a companion part at +0xA8.
 * Projection narrows the coordinates to 16 bits with (+150,-100,-150) offsets.
 * The active render context, ordering tables, packet space and GTE transform
 * must already be valid. These calls submit packets, not wait/present frames.
 *
 * Work flag 0x00800000 suppresses the entire service. Otherwise animation
 * advances even when projection rejects the object, unless work flag
 * 0x08000000 is set. Object flag 0x1000 suppresses the marker; the object's
 * sign bit chooses the marker variant. No linkage is made private by folding.
 * The two scratch-backed variants are not reentrant.
 */

/* Normal sprite/companion path. Object flag 0x01000000 projects the marker
 * at fixed input Y=-100, independently of the sprite's projection/depth.
 * Original identity: ovl_11:0x800CFE40. */
void Ovl11DrawObjectWithMarker(u8 *object);

/* Companion is shifted up 20 screen pixels; nonzero +0xB6 also draws the
 * frame block at +0xC8 up 16 pixels. Marker uses the main projection, with
 * no fixed-height reprojection. Original identity: ovl_11:0x800D001C. */
void Ovl11DrawObjectWithExtraSprite(u8 *object);

/* Alternate sprite builder; textureBase is an absolute address carried in
 * the existing s32 ABI, not an offset. sourceBias is added to the shared
 * source-base word at exe:8005E3BC. The observed caller passes the address
 * object+0xE0+contextIndex*8 and bias 0x2800; neither is validated here.
 * Marker/animation rules match the normal path.
 * Original identity: ovl_11:0x800D0198. */
void Ovl11DrawObjectWithSourceOffsets(u8 *object, s32 textureBase, s32 sourceBias);

#endif
