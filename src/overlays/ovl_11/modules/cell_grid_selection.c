/*
 * cell_grid_selection — reconstructed translation unit.
 *
 * Map the shared selector to the two cell-grid converter slots or the no-grid
 * result.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_cell_grid_selection.h"

s32 Ovl11GetCellGridSelection(void) {
    char *farState = (char *)&D_8007AFF0;
    s32 selection;

    selection = (*(s16 *)(farState + 0x25476) != 1) * 2;
    if (*(s16 *)(farState + 0x25476) == 6) {
        selection = 1;
    }
    return selection;
}
