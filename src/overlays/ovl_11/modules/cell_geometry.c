/*
 * Cell geometry, original ovl_11:800D7EF8-800D81BC. Convert mode-1 world
 * positions, test layer-specific cell bounds/flags, and recover cell corners.
 * The 399-unit lookup and 400-unit corner spacing deliberately differ.
 * These queries do not mutate cell records or install textures.
 */
#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_grid_position.h"
#include "ovl11_cell_geometry.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 padding;
} Ovl11PositionCopy4Words;

s32 Ovl11LookupMode1GridPosition(s32 *position, u16 *column, u16 *row) {
    Ovl11PositionCopy4Words copy;
    u8 *work;
    u8 *upperWork;
    s16 biasedX;
    s16 col;
    s16 biasedZ;
    s16 rowIndex;
    s32 *entry;
    s32 byteIndex;
    s16 requiredMode;
    u8 *pointerGrid;

    /* The unused four-word copy preserves the original aggregate-copy frame. */
    copy = *(Ovl11PositionCopy4Words *)position;
    *column = *(u16 *)position + 0x1130;
    *row = 0x640 - *(u16 *)((u8 *)position + 8);
    biasedZ = (s16)*row;
    biasedX = (s16)*column;
    *column = (s16)*column / 399;
    *row = (s16)*row / 399;
    if (biasedX < 0) {
        *column -= 1;
        return 2;
    }
    col = (s16)*column;
    if (col >= 0x2D) {
        return 3;
    }
    if (biasedZ < 0) {
        *row -= 1;
        return 2;
    }
    rowIndex = (s16)*row;
    if (rowIndex >= 0x19) {
        return 3;
    }
    if ((u16)*column >= 0x2D) {
        return 1;
    }
    if (rowIndex < 0) {
        return 1;
    }
    requiredMode = 1;
    work = (u8 *)&D_8007AFF0;
    byteIndex = (rowIndex * 0x2D + col) * 4;
    upperWork = work + 0x20000;
    pointerGrid = upperWork + 0x3608;
    entry = *(s32 **)(pointerGrid + byteIndex);
    if (*(s16 *)(upperWork + 0x5476) != requiredMode || (*entry & 8) != 0) {
        return 0;
    }
    return 1;
}

s32 Ovl11IsCellUnusable(s16 column, s16 row, s16 requireFlag8, s16 layer) {
    Ovl11CellQueryBounds *bounds;
    Ovl11GridTextureEntry *entry;

    bounds = gOvl11CellQueryLayerBounds + layer;
    if ((column < 0) || (column >= bounds->columnCount)) {
        return 1;
    }
    if ((row < 0) || (row >= bounds->rowCount)) {
        return 1;
    }
    entry = ((Ovl11GridTextureRowsView *)&D_8007AFF0)->entries[row][column];
    if (requireFlag8 != 0) {
        if (!(entry->header & 8)) {
            return 1;
        }
    }
    return 0;
}

s32 Ovl11GetCellCornerPosition(s16 column, s16 row, Vec3 *position, s16 layer) {
    s32 z;
    s32 rowOffset;
    s32 x;

    if (layer == 0) {
        x = column * 400 - 0x1068;
        rowOffset = row * 400;
        position->x = x;
        z = 0x578;
    } else {
        x = column * 400 - 0x58C;
        rowOffset = row * 400;
        position->x = x;
        z = 0x3FC;
    }
    z -= rowOffset;
    position->z = z;
    return z;
}
