#ifndef OVL11_RECORD_TEXTURE_RELOCATION_H
#define OVL11_RECORD_TEXTURE_RELOCATION_H

#include "common.h"

/* ovl_11:800F0A58. Search the first two 9-ID slot rows (row 0 wins at a
 * shared column), copy the paired sprite descriptor to the active record,
 * move its texture to VRAM (704,448) and its 16-word CLUT to (704,447), then
 * rewrite the active descriptor's source/CLUT coordinates and clear its final
 * halfword. The texture source adds X base 832 or 960 according to whether
 * CLUT X is below 1008; width is descriptor width/4 VRAM words.
 * Return 1 on a hit; on a miss print Error and return 0 without changing the
 * active record. Requires initialized slot IDs, paired descriptors and VRAM;
 * dimensions/source ranges are unchecked. No explicit DrawSync is performed. */
s32 Ovl11RelocateRecordSlotTexture(s16 spriteId);

#endif
