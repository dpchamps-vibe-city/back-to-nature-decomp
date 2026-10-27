/*
 * ready_kind36_cell — reconstructed translation unit.
 *
 * Refresh the first threshold-ready kind-36 main-grid cell, preferring a
 * local window around a supplied position before the full grid.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_ready_kind36_cell.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_map_grid_cells.h"
#include "ovl11_grid_position.h"
#include "ovl11_cell_grid_lookup.h"

s32 Ovl11RefreshFirstReadyKind36Cell(s32 *position) {
    s16 firstColumn;
    s16 firstRow;
    s16 threshold;
    s32 result;
    s32 row;
    s32 column;
    Ovl11D124Entry *entry;

    result = 0;
    threshold = Ovl11ReadRecordTypeProperty02(0x36);
    if (Ovl11LookupMode1GridPosition(position, (u16 *)&firstColumn, (u16 *)&firstRow) == 0) {
        /* Clamping the window origin shifts, rather than shrinks, the
         * low-edge window. Keep the full-grid fallback after this search. */
        firstRow--;
        firstRow = firstRow < 0 ? 0 : firstRow;
        firstColumn--;
        firstColumn = firstColumn < 0 ? 0 : firstColumn;
        for (row = (s16)firstRow; row < (s16)firstRow + 3 && row < 25; row++) {
            for (column = (s16)firstColumn; column < (s16)firstColumn + 3 && column < 45; column++) {
                entry = &gMainAreaGrid[row][column];
                if ((u16)entry->unk0 == 0x36 && entry->unk4 >= threshold) {
                    Ovl11RefreshCellTextureVariant(row, column, 0);
                    result = 1;
                    goto done;
                }
            }
        }
    }
    for (row = 0; row < 25; row++) {
        for (column = 0; column < 45; column++) {
            entry = &gMainAreaGrid[row][column];
            if ((u16)entry->unk0 == 0x36 && entry->unk4 >= threshold) {
                Ovl11RefreshCellTextureVariant(row, column, 0);
                result = 1;
                goto done;
            }
        }
    }
done:
    return result;
}
