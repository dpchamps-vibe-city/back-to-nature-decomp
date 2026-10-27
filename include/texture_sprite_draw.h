#ifndef TEXTURE_SPRITE_DRAW_H
#define TEXTURE_SPRITE_DRAW_H

#include "common.h"

/* Queue a cache record as an opaque, neutral-color SPRT plus DR_MODE pair.
 * (x,y) is a 32x32 screen-pixel cell origin: dimensions below 32 are centered;
 * larger dimensions are NOT cropped or centered. Also draws the Ovl11DrawEmptySlot
 * backing at the original cell origin, using ot+1. Commits packet_work cursor.
 * textureIndex is unchecked (cache has 72 entries); data must be resident. */
void Ovl11DrawTextureSpriteInCell(s32 *ot, s16 textureIndex, s16 x, s16 y);

/* Queue an opaque, neutral-color POLY_FT4 centered at the screen-pixel point.
 * Multiplier is trunc(scale4096/4096), THEN multiplied by each dimension and
 * narrowed to s16. This is not fractional Q12 scaling; no clipping/validation.
 * UVs still cover the original record. Advances active context field_118.
 * Both calls require usable ordering-table/packet storage and preserve cache
 * contents; neither checks the record's active flag nor loads its image. */
void Ovl11DrawScaledTextureQuad(s32 *ot, s16 textureIndex, s16 centreX,
                              s16 centreY, s32 scale4096);

#endif
