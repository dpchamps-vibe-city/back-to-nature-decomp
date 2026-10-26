#ifndef OVL11_MAP_GRID_CELLS_H
#define OVL11_MAP_GRID_CELLS_H

#include "common.h"

/* Layer 0 selects the 45-column, 25-row main grid; nonzero selects the
 * 7x7 alternate grid. Return NULL for an out-of-range coordinate. */
Ovl11D124Entry *Ovl11GetMapGridCell(s32 layer, s16 column, s16 row);

/* Recompute variant nibbles in the 3x3 neighborhood. Only layers 0/1 are
 * valid (unchecked table indices). Neighbor membership means kind 0x36 and
 * nonzero first state byte. useCountFallback enables the original count-based
 * fallback; skipTextureDispatch updates flags without uploading textures.
 * With both zero, texture 5 is sent for the center BEFORE its bounds check.
 * Uses shared probe scratch and is not reentrant. */
void Ovl11ApplyCellNeighborhoodVariants(s16 row, s16 column, s16 layer,
                                        s16 useCountFallback, s16 skipTextureDispatch);

/* Refresh kind-0x36 state, clear the second byte and set flag 0x8000;
 * if the refreshed byte is nonzero, dispatch its variant texture. Layer 0/1. */
void Ovl11RefreshCellTextureVariant(s16 row, s16 column, s32 layer);

/* INVERTED selector: mainMap!=0 searches main, zero searches alternate only
 * when its enable word is nonzero. Consume one Rand(count) starting index and
 * scan with wrap. Mask bits 1/2/4 ignore kind/value/first-state-byte respectively;
 * bit 8 counts all hits instead of stopping at the first. Return hit count;
 * output coordinates describe the last hit, or remain untouched on no hit. */
s32 Ovl11FindRandomGridCell(s16 *columnOut, s16 *rowOut, s32 mainMap,
                           Ovl11D124Entry *key, s32 matchMask);

/* Normalize the second kind 169->168 (also first if equal), or 16B->16A.
 * The pointer must expose at least two halfwords. */
void Ovl11NormalizeCellKindPair(u16 *kinds);

/* Spawn an effect at y=0, in 400-unit grid spacing. Layer 0 uses origins
 * (-4200,1400), other layers (-1400,1000); z decreases with row. Coordinates
 * are not bounds checked; effectId narrows to signed s16 before spawning.
 * All original clients discard the pool result; no result is published. */
void Ovl11SpawnCellEffectSprite(u16 effectId, s16 layer, s16 column, s16 row);

#endif
