#ifndef OVL11_CELL_GRID_SELECTION_H
#define OVL11_CELL_GRID_SELECTION_H

#include "common.h"

/* Return grid slot 0 for shared selector 1, slot 1 for selector 6, or 2 for
 * every other signed-halfword value. Cell-action clients treat 2 as no grid;
 * only slots 0/1 index gOvl11AreaCellConverters. Read-only, no caching. */
s32 Ovl11GetCellGridSelection(void);

#endif
