/* Record-table queries, original ovl_11:800D5960-800D622C.
 * Four shared work pointers provide record headers, cell grids, flag records
 * and type properties. The signed narrowing in the trailing-grid readers
 * differs from the tag scanner's raw grid index; preserve that distinction.
 * All reads are borrowed and unchecked. Contracts: ovl11_cell_grid_lookup.h.
 * Whole-unit membership and conservative boundaries: notes/file-groupings.md. */

#include "common.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_grid_transform.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

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
    Ovl11TransformGridCells(transformMode & 0xFF, cells, (u8 (*)[13])((char *)tables->cellGrids + gridIndex * 0xB0));
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

/* No known callers; retain external linkage. Only the three extended codes
 * select a variant. Every other low-16-bit code is copied unchanged. */
typedef struct {
    /* 0x00 */ s16 itemId;
    /* 0x02 */ s16 variant;
    /* 0x04 */ s16 reserved;
} Ovl11DecodedItemCode;

void Ovl11DecodeItemCode(Ovl11DecodedItemCode *decoded, s32 code) {
    s32 itemCode;

    itemCode = code & 0xFFFF;
    switch (itemCode) {
    case 0x122:
        decoded->itemId = 0x86;
        decoded->variant = 0;
        break;
    case 0x123:
        decoded->itemId = 0x86;
        decoded->variant = 1;
        break;
    case 0x124:
        decoded->itemId = 0x86;
        decoded->variant = 2;
        break;
    default:
        decoded->itemId = itemCode;
        decoded->variant = 0;
        break;
    }
    decoded->reserved = 0;
}

s32 Ovl11GetItemPrice(u16 itemIndex) {
    return gItemDataTable[itemIndex].field_10;
}

s32 Ovl11GetItemSellValue(s32 itemIndex) {
    s32 index = itemIndex & 0xFFFF;
    s32 bonus = 0;

    if (index == 0x97) {
        if (((Ovl11ItemSaleState *)&D_8006C838)->statusFlags & 0x400000) {
            bonus = 0xA;
        }
    }
    return bonus + *(s16 *)((char *)((Ovl11ItemSaleState *)&D_8006C838)->items + index * 0x28 + 0x12);
}

s32 Ovl11ClassifyHeldItemType(u16 itemIndex) {
    s8 type = gItemDataTable[itemIndex].type;

    if ((gItemDataTable[itemIndex].u0.flags & 0x8100) != 0x8000)
        return 0;
    return type != 1;
}

s32 Ovl11GetRecordItemId(s16 *record) {
    if (*record == 0xA4) {
        return record[1];
    }
    return 0;
}

s32 Ovl11GetItemPackedProperty(s16 itemIndex, s16 propertyIndex) {
    s16 row;
    s32 value;
    u16 flags;
    ItemData *item;
    Ovl11PackedItemGlobals *globals = (Ovl11PackedItemGlobals *)D_8006C838;
    s32 *properties;
    s32 *address;
    s32 shift;

    item = (ItemData *)(itemIndex * 0x28 + (s32)globals->items);
    row = item->field_0E;
    flags = item->u0.flags;
    if (row == -1) {
        return -1;
    }
    if (flags & 0x4000) {
        return -2;
    }
    properties = (s32 *)(row * 0x10 + (s32)globals->properties);
    address = &properties[(s16)(propertyIndex / 10)];
    shift = (s16)(propertyIndex % 10) * 3;
    value = *address >> shift;
    value &= 7;
    if ((flags & 0x180) && (value >= 0)) {
        if (value < 3) {
            value = 5;
        } else if (value < 5) {
            value = 6;
        }
    }
    return value;
}

s32 Ovl11ReadRecordTypeProperty00(s16 recordIndex) {
    Ovl11CellGridTables *tables = (Ovl11CellGridTables *)&D_8006C838;
    s8 type;

    type = ((s8 *)tables->recordHeaders)[recordIndex * 0x28 + 3];
    if (type == -1) {
        return -1;
    }
    return *(u16 *)((u8 *)tables->typeProperties + type * 0x10);
}
