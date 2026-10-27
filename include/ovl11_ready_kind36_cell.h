#ifndef OVL11_READY_KIND36_CELL_H
#define OVL11_READY_KIND36_CELL_H

#include "common.h"

/* Search the 45x25 main grid for kind 0x36 whose unsigned byte +4 is at
 * least its signed-halfword type-property +02 threshold. position borrows
 * three signed XYZ words in the mode-1 grid conversion's world units.
 *
 * Prefer a row-major 3x3 window starting at max(converted cell - 1, 0),
 * clipped at the high grid edges. If conversion fails or that window has no
 * match, search the entire grid row-major. Refresh the first match through
 * Ovl11RefreshCellTextureVariant(row, column, 0) and return 1; return 0 if
 * none qualifies. The refresh helper's result is ignored. Grid/type tables
 * and position must be initialized; position itself is not modified. */
s32 Ovl11RefreshFirstReadyKind36Cell(s32 *position);

#endif
