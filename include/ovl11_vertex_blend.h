#ifndef OVL11_VERTEX_BLEND_H
#define OVL11_VERTEX_BLEND_H

#include "common.h"
#include "game_types.h"

/* ovl_11 must be resident. Three shared descriptors, each holding two source
 * projected-primitive blocks and one writable output block. Pool reset zeroes
 * all descriptors and marks them free (flag 8000). It invalidates outstanding
 * descriptors; it does not free or modify the referenced blocks.
 *
 * Alloc claims the first free slot, initializes the three projected sources,
 * copies three position halfwords, and returns NULL if all slots are in use.
 * Blocks and the position pointer must be valid/aligned. Their coordinate
 * units are retained. The returned ratio starts at 0/0: the caller MUST set a
 * nonzero denominator before Update, and keep all three blocks alive.
 *
 * Update resolves vertices relative to each block's +C offset, then blends
 * as many eight-byte records as the OUTPUT block's +10 count. All arrays must
 * be that long. Per component: A + (s16)(B-A) * numerator / denominator,
 * stored modulo 65536. Delta wraps to signed 16 bits BEFORE multiplication;
 * division truncates toward zero. There is no ratio clamp or overflow guard.
 * The fourth halfword of each vertex is untouched. Update does not advance
 * the ratio, draw, allocate, or release the descriptor. */

/* Original identities, ovl_11:800DCECC, 800DCF10, 800DCFEC. */
void Ovl11ResetVertexBlendPool(void);
Ovl11VertexBlend *Ovl11AllocVertexBlend(void *sourceA, void *sourceB,
                                      void *output, u8 *position);
void Ovl11UpdateVertexBlend(Ovl11VertexBlend *blend);

#endif
