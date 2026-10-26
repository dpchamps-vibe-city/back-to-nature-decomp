/* Record-table queries, original ovl_11:800D5960-800D5FA8.
 * Four shared work pointers provide record headers, cell grids, flag records
 * and type properties. The signed narrowing in the trailing-grid readers
 * differs from the tag scanner's raw grid index; preserve that distinction.
 * All reads are borrowed and unchecked. Contracts: ovl11_cell_grid_lookup.h.
 * Whole-unit membership and conservative boundaries: notes/file-groupings.md. */

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

s32 Ovl11ReadRecordTypeProperty02(s16 recordIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    s8 type;

    type = ((s8 *)tables->recordHeaders)[recordIndex * 0x28 + 3];
    if (type == -1) {
        return -1;
    }
    return *(u16 *)((u8 *)tables->typeProperties + type * 0x10 + 2);
}

s32 Ovl11ReadRecordTypeProperty04(s16 recordIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    s8 type;

    type = ((s8 *)tables->recordHeaders)[recordIndex * 0x28 + 3];
    if (type == -1) {
        return -1;
    }
    return *(s16 *)((u8 *)tables->typeProperties + type * 0x10 + 4);
}

s32 Ovl11ReadRecordTypeProperty0E(s16 recordIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    s8 type;

    type = ((s8 *)tables->recordHeaders)[recordIndex * 0x28 + 3];
    if (type == -1) {
        return 0;
    }
    return *(u16 *)((u8 *)tables->typeProperties + type * 0x10 + 0xE);
}

/* A -1 row entry is absent. Named-zero comparisons preserve the existing
 * mask/comparison form; they are not combined into a generic mask helper. */
s32 Ovl11RecordPropertyHasFlag8000(u16 recordIndex, u16 propertyIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    Ovl11RecordPropertyRow *rows = (Ovl11RecordPropertyRow *)tables->recordHeaders;
    s16 entryIndex;
    u32 zero = 0;

    entryIndex = rows[recordIndex].propertyIndex[propertyIndex];
    if (entryIndex == -1) {
        return 0;
    }
    return (*(u16 *)((u8 *)tables->flagEntries + entryIndex * 8 + 6) & 0x8000) > zero;
}

s32 Ovl11RecordPropertyHasFlag2000(u16 recordIndex, u16 propertyIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    Ovl11RecordPropertyRow *rows = (Ovl11RecordPropertyRow *)tables->recordHeaders;
    s16 entryIndex;
    u32 zero = 0;

    entryIndex = rows[recordIndex].propertyIndex[propertyIndex];
    if (entryIndex == -1) {
        return 0;
    }
    return (*(u16 *)((u8 *)tables->flagEntries + entryIndex * 8 + 6) & 0x2000) > zero;
}

s32 Ovl11RecordPropertyHasFlag4000(u16 recordIndex, u16 propertyIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    Ovl11RecordPropertyRow *rows = (Ovl11RecordPropertyRow *)tables->recordHeaders;
    s16 entryIndex;
    u32 zero = 0;

    entryIndex = rows[recordIndex].propertyIndex[propertyIndex];
    if (entryIndex == -1) {
        return 0;
    }
    return (*(u16 *)((u8 *)tables->flagEntries + entryIndex * 8 + 6) & 0x4000) > zero;
}

s32 Ovl11ReadRecordPropertyValue0(u16 recordIndex, u16 propertyIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    Ovl11RecordPropertyRow *rows = (Ovl11RecordPropertyRow *)tables->recordHeaders;
    s16 entryIndex;
    u16 flags;
    u16 value;

    entryIndex = rows[recordIndex].propertyIndex[propertyIndex];
    if (entryIndex == -1) {
        return -1;
    }
    flags = *(u16 *)((u8 *)tables->flagEntries + entryIndex * 8 + 6);
    value = *(u16 *)((u8 *)tables->flagEntries + entryIndex * 8);
    if ((flags & 0xE000) == 0) {
        return -1;
    }
    return value;
}

s32 Ovl11ReadRecordPropertyValue2(u16 recordIndex, u16 propertyIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    Ovl11RecordPropertyRow *rows = (Ovl11RecordPropertyRow *)tables->recordHeaders;
    s16 entryIndex;
    u16 flags;
    u16 value;

    entryIndex = rows[recordIndex].propertyIndex[propertyIndex];
    if (entryIndex == -1) {
        return -1;
    }
    flags = *(u16 *)((u8 *)tables->flagEntries + entryIndex * 8 + 6);
    value = *(u16 *)((u8 *)tables->flagEntries + entryIndex * 8 + 2);
    if ((flags & 0xE000) == 0) {
        return -1;
    }
    return value;
}

s32 Ovl11RecordPropertyHasFlag0800(u16 recordIndex, u16 propertyIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    Ovl11RecordPropertyRow *rows = (Ovl11RecordPropertyRow *)tables->recordHeaders;
    s16 entryIndex;
    u32 zero = 0;

    entryIndex = rows[recordIndex].propertyIndex[propertyIndex];
    if (entryIndex == -1) {
        return 0;
    }
    return (*(u16 *)((u8 *)tables->flagEntries + entryIndex * 8 + 6) & 0x800) > zero;
}
