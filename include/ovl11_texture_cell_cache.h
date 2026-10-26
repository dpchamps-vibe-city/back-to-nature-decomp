#ifndef OVL11_TEXTURE_CELL_CACHE_H
#define OVL11_TEXTURE_CELL_CACHE_H

#include "common.h"

/* Clear IDs AND activity for 72 records; preserve all geometry/VRAM contents. */
void Ovl11ResetTextureCellCache(void);

/* Begin an activity pass: retain cached IDs/geometry but clear all 72 marks.
 * Subsequent successful draws mark their entries active (not evictable). */
void Ovl11ClearTextureCellActivity(void);

/*
 * Draw textureId at signed pixel (x,y), linking packets into ot. A hit marks
 * the entry active. A miss evicts the highest inactive record, loads into the
 * first free record and draws; if every record is active, draw nothing.
 * ID zero invokes the shared-sprite fallback with ot+1, not a cache lookup.
 *
 * Loading blocks on CD and overwrites 0x800 bytes of gMessageWindowReadBuffer;
 * uploads image/CLUT data to slot-assigned VRAM. Not reentrant with message
 * loading, batch streaming or other users of that scratch area/VRAM region.
 * Asset animation/frame/piece tables are unchecked (the last piece supplies
 * geometry). Cache reset must precede first use; activity clearing controls
 * retention. Packet capacity/lifetime remain the caller's responsibility.
 */
void Ovl11DrawTextureCell(s32 *ot, u16 textureId, s16 x, s16 y);

#endif
