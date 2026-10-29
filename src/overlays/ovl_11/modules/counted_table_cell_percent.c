/* Count nonexcluded halfword IDs in the two work-area cell tables.
 * The optional table is 7 by 7; the permanent table is 45 by 25. Within
 * each table, row starts are eight bytes apart and columns use the full
 * row stride (56 or 360 bytes). The query does not change either table.
 */
#include "common.h"
#include "game_types.h"
#include "game_flags.h"
#include "ovl11_counted_table_cell_percent.h"

s32 Ovl11IsCountedTableCellId(s16 id);

s32 Ovl11GetCountedTableCellPercent(void) {
    s16 *optionalCell;
    s16 *cell;
    s32 gridBase;
    u8 *workBase;
    s32 optionalColumn;
    s32 column;
    s32 row;
    s32 countedCells;
    s32 totalCells;
    s32 optionalRowOffset;
    s32 rowOffset;
    struct { s16 value; } optionalGridOffset;
    struct { s16 value; } gridOffset;

    countedCells = 0;
    totalCells = 0x465;
    if (GetGameFlag(0x3EU) == 1) {
        s32 counted;

        totalCells = 0x496;
        counted = 1;
        /* Keep these halfword stores outside their loops, with the base
         * additions inside: the original has independent address hoists.
         */
        optionalGridOffset.value = 0x78EE;
        for (row = 0; row < 7; row++) {
            workBase = (u8 *) D_8006C838;
            gridBase = optionalGridOffset.value + (s32) workBase;
            optionalRowOffset = row * 8;
            optionalCell = (s16 *) (optionalRowOffset + gridBase);
            for (optionalColumn = 6; optionalColumn >= 0; optionalColumn--) {
                if (Ovl11IsCountedTableCellId(*optionalCell) == counted) {
                    countedCells += 1;
                }
                optionalCell += 0x1C;
            }
        }
    }
    {
        s32 counted;

        row = 0;
        counted = 1;
        gridOffset.value = 0x55C6;
        for (; ; ) {
            workBase = (u8 *) D_8006C838;
            gridBase = gridOffset.value + (s32) workBase;
            rowOffset = row * 8;
            row += 1;
            cell = (s16 *) (rowOffset + gridBase);
            column = 0x18;
next_cell:
            if (Ovl11IsCountedTableCellId(*cell) == counted) {
                countedCells += 1;
            }
            column -= 1;
            cell += 0xB4;
            if (column >= 0) {
                goto next_cell;
            }
            if (row >= 0x2D) {
                break;
            }
        }
    }
    return (s32) (countedCells * 0x64) / totalCells;
}

/* The order of this exclusion chain is retained from the original. */
s32 Ovl11IsCountedTableCellId(s16 id) {
    if (id == 0x3A || id == 0x40 || id == 0x3E || id == 0x3F ||
        id == 0x175 || id == 0x3C || id == 0x3B || id == 0x16F ||
        id == 0x16C || id == 0x170 || id == 0x16D || id == 0x171 ||
        id == 0x16E || id == 0x3D || id == 0x172 || id == 0x173 ||
        id == 0x174 || id == 0x167) {
        return 0;
    }
    return 1;
}
