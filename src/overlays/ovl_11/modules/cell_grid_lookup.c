/* Two-table cell-grid resolution, transformed tag search and trailing fields.
 * The shared pointer view is in globals_override.h. The signed narrowing in
 * the readers is part of the indexing contract, not interchangeable with the
 * scanner's raw grid index. Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_cell_grid_lookup.h"

void ovl_11_func_800D6488(u8 mode, u8 dst[][13], u8 src[][13]);

s32 Ovl11FindCellGridTag(s16 *recordIndex, u32 tag, s16 transformMode,
                       s16 *outX, s16 *outY, s16 adjacentRecord) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    u8 cells[13][13];
    s16 gridIndex;
    s32 x;
    s32 y;
    s32 found;

    gridIndex = *(s16 *)((char *)tables->recordHeaders + (*recordIndex * 0x28) + (adjacentRecord * 0x28) + 0x0E);
    if (gridIndex == -1) {
        return 0;
    }
    ovl_11_func_800D6488(transformMode & 0xFF, cells, (u8 (*)[13])((char *)tables->cellGrids + gridIndex * 0xB0));
    tag = tag + 1;
    found = 0;
    for (y = 0; y < 13; y++) {
        for (x = 0; x < 13; x++) {
            if (cells[y][x] == tag) {
                found = 1;
                *outX = x - 6;
                *outY = y - 6;
                break;
            }
        }
    }
    return found;
}

/* These three entries have no known callers. Each key has a signed index at
 * +0 and an unsigned variant at +2. Absent headers return -1; otherwise the
 * result is an unsigned halfword, promoted to s32. Keep the intermediate
 * s16 truncations, and do not guess a meaning for the three trailing fields. */
s32 Ovl11ReadCellGridHalfwordAA(void *key) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    u16 *field;
    s16 gridIndex;
    s32 blockIndex, combinedIndex, record;

    gridIndex = *(s16 *)((char *)tables->recordHeaders + (*(s16 *)key * 0x28) + 0x0E);
    if (gridIndex == -1) {
        return -1U;
    }
    blockIndex = (s16)(gridIndex * 5);
    combinedIndex = blockIndex + (u16)*(u16 *)((char *)key + 2);
    record = (s16)combinedIndex;
    field = (u16 *)((char *)tables->cellGrids + record * 0xB0 + 0xAA);
    return *field;
}

s32 Ovl11ReadCellGridHalfwordAC(void *key) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    u16 *field;
    s16 gridIndex;
    s32 blockIndex, combinedIndex, record;

    gridIndex = *(s16 *)((char *)tables->recordHeaders + (*(s16 *)key * 0x28) + 0x0E);
    if (gridIndex == -1) {
        return -1U;
    }
    blockIndex = (s16)(gridIndex * 5);
    combinedIndex = blockIndex + (u16)*(u16 *)((char *)key + 2);
    record = (s16)combinedIndex;
    field = (u16 *)((char *)tables->cellGrids + record * 0xB0 + 0xAC);
    return *field;
}

s32 Ovl11ReadCellGridHalfwordAE(void *key) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    u16 *field;
    s16 gridIndex;
    s32 blockIndex, combinedIndex, record;

    gridIndex = *(s16 *)((char *)tables->recordHeaders + (*(s16 *)key * 0x28) + 0x0E);
    if (gridIndex == -1) {
        return -1U;
    }
    blockIndex = (s16)(gridIndex * 5);
    combinedIndex = blockIndex + (u16)*(u16 *)((char *)key + 2);
    record = (s16)combinedIndex;
    field = (u16 *)((char *)tables->cellGrids + record * 0xB0 + 0xAE);
    return *field;
}
