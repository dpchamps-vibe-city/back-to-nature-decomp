#ifndef OVL11_GEOMETRY_HIT_FLAGS_H
#define OVL11_GEOMETRY_HIT_FLAGS_H

#include "common.h"

/* Interpret the descriptor retained by the resident scene-geometry query.
 * Query geometry first; later queries replace the shared hit pointer.
 * Original identities: ovl_11:800BF3D0/800BF3F4/800BF450. */

/* Return bit 7 of the descriptor's first BYTE. Requires a non-NULL hit. */
s32 Ovl11GetGeometryHitHighBit(void);
/* Return 0 on NULL; otherwise word bits 4 -> 2, 40 -> 3, 2 -> 1, else 0,
 * in that priority order (bit values are hexadecimal). No state change. */
s32 Ovl11ClassifyGeometryHit(void);
/* Cache the pointer and, when non-NULL, its low 16 flag bits in word storage.
 * Low return word: 1 on NULL, 0 on a hit. High return word: 8000 when the hit's
 * bit 2000 is clear; otherwise not a defined output. Callers currently consume
 * only the low word. Preserve this two-register ABI, not a scalar s32 return.
 * On NULL the previous cached flag word is left unchanged. */
unsigned long long Ovl11CacheGeometryHit(void);

#endif
