#ifndef OVL11_CELL_GRID_LOOKUP_H
#define OVL11_CELL_GRID_LOOKUP_H

#include "common.h"

/* Resolve a grid from the +20/+24 work-area tables, transform its 13x13 byte
 * cells with transformMode's low byte, and search for tag+1 (u32 arithmetic).
 * recordIndex points to a signed table index; adjacentRecord adds another
 * 0x28-byte record offset. A signed -1 in that header's +0xE means absent.
 * Tables, indices and transformation mode must already be valid.
 *
 * Returns 0 with outputs unchanged when absent/not found, otherwise 1.
 * Outputs are signed CELL offsets from (6,6), not world/pixel coordinates.
 * Each matching row overwrites them: result is the FIRST matching column in
 * the LAST matching row. Outputs must be writable; no allocation or persistent
 * state changes. Only the scanner has corroborated external callers. */
s32 Ovl11FindCellGridTag(s16 *recordIndex, u32 tag, s16 transformMode,
                       s16 *outX, s16 *outY, s16 adjacentRecord);

#endif
