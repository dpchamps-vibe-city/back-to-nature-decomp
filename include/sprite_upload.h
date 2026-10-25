#ifndef SPRITE_UPLOAD_H
#define SPRITE_UPLOAD_H

#include "common.h"

/* Upload tag-0xD image records to VRAM. Each entry has a 12-byte header,
 * byte size in its low 24 bits and decode/type bits in the high byte; the
 * next entry is four-byte aligned. Invalid container tags do nothing.
 * Raw and RLE paths share global decode scratch: calls are non-reentrant.
 * Source sizes/destinations are not validated. Coordinates are GPU VRAM
 * halfword-pixel coordinates; callers must ensure legal rectangles and data.
 */

/* exe:8001719C. Use every entry's stored destination. */
void UploadSpriteImages(u8 *images);
/* exe:800171CC. For entries with high-byte bit 1 set, sets pixel bit 15
 * from pixel 1 through width-1 of each decoded row, leaving pixel 0 alone.
 * The column decode path sets width=1, so it gets no such mask stores.
 * The historical address-as-s32 ABI is retained, not changed to a pointer. */
void UploadSpriteImagesWithMask(s32 imageAddress);
/* exe:80017200. Add (x,y) to non-type-2 entries and (type2X,type2Y) to
 * type-2 entries, with signed-halfword wrapping. */
void UploadSpriteImagesAtOffsets(u8 *images, s16 x, s16 y,
                                 s16 type2X, s16 type2Y);
/* exe:80017240. Same offsets, except type-2 entries whose width is at
 * least 256 retain their stored destination instead. */
void UploadSpriteImagesKeepingWideType2Positions(u8 *images, s16 x, s16 y,
                                                s16 type2X, s16 type2Y);

#endif
