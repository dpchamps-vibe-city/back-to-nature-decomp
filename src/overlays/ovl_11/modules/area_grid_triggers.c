/*
 * area_grid_triggers — reconstructed translation unit.
 *
 * Ten-plane 10-by-10 area-cell state lifecycle: reset and probe the grid,
 * classify and texture cells, resolve contact triggers and rewards, then
 * restore the recorded position. The external lifecycle interface and its
 * units/preconditions are documented in area_grid_triggers.h. The grid's
 * active plane and its centre/last-cell handoff are one protocol; the restore
 * pair is deliberately included rather than split from its state writers.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "area_grid_triggers.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "game_flags.h"
#include "random.h"

void Ovl11ResetAreaGridState(void) {
    u8 *b;
    u8 *tail;
    u8 *p;
    s32 idx;
    u32 i;
    u32 j;
    u32 k;

    i = 0;
    while (i < 10) {
        b = gOvl11AreaGridState;
        p = b + 0xC;
        p[i] = 0;
        for (j = 0; j < 10; j++) {
            idx = 0x16 + i * 100;
            for (k = 0; k < 10; k++) {
                b[idx + j * 10 + k] = 0;
            }
        }
        i++;
    }
    tail = gOvl11AreaGridState;
    *(u32 *)(tail + 0x400) = 0;
    *(u16 *)gOvl11AreaGridState = 0;
}

s32 Ovl11AreaGridCellFromPosition(s32 *arg0, u16 *arg1, u16 *arg2);
s32 Ovl11LatchAreaGridTrigger(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

s32 Ovl11CheckAreaGridContact(Ovl11C7010Object *object) {
    VECTOR position;
    u16 column;
    u16 row;
    s16 plane;

    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x37) {
        SetGameFlag(0x63U, 1U);
    }
    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 == 0x48) {
        SetGameFlag(0x64U, 1U);
    }
    if (!(object->unk6C & 8)) {
        if (!(((Ovl11GridState *)gOvl11AreaGridState)->flags & 0xE000)) {
            position.vx = *(s32 *)((u8 *)object + 0x100);
            position.vy = *(s32 *)((u8 *)object + 0x104);
            position.vz = *(s32 *)((u8 *)object + 0x108);
            if (Ovl11AreaGridCellFromPosition((s32 *)&position, &column, &row) == 0) {
                plane = *(s16 *)gOvl11AreaGridState;
                if (plane != 0) {
                    if (Ovl11LatchAreaGridTrigger(0, plane - 1, (s16)column, (s16)row) != 0) {
                        return 1;
                    }
                }
                if (Ovl11LatchAreaGridTrigger(1, *(s16 *)gOvl11AreaGridState, (s16)column, (s16)row) != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800CB4E0(void);

s32 Ovl11LatchAreaGridTrigger(s32 currentPlane, s32 plane, s16 column, s16 row) {
    if (((Ovl11GridState *)gOvl11AreaGridState)->grid[plane][column][row] == 2) {
        ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x18000000;
        ((Ovl11GridState *)gOvl11AreaGridState)->flags |= currentPlane != 0 ? 0x8000 : 0x4000;
        SetGameFlag(3, 1);
        if (currentPlane == 0) {
            ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x3C, -1, 0);
        }
        ovl_11_func_800CB4E0();
        return 1;
    }
    return 0;
}

s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);
/* Overlay alias of the matched word-returning GetVal8005E524. */
s32 GetVal8005E524(void);

s32 Ovl11BuildAreaGrid(s32 resumeWord) {
    s32 *entry;
    u8 *base;
    s32 **grid;
    s32 **cell;

    ((Ovl11GridState *)gOvl11AreaGridState)->probability = 10;
    gOvl11AreaGridState[0x15] = 1;
    if ((resumeWord << 16) == 0) {
        gOvl11AreaGridScanRow = 0;
        gOvl11AreaGridProbePosition[0] = -2000;
        gOvl11AreaGridProbePosition[1] = 0;
        gOvl11AreaGridProbePosition[2] = 1800;
    }
    while (gOvl11AreaGridScanRow < 10) {
        base = D_8009AFF0;
        gOvl11AreaGridProbePosition[0] = -1800;
        for (gOvl11AreaGridScanColumn = 0; gOvl11AreaGridScanColumn < 10;) {
            grid = (s32 **)(base + 0x3608);
            ovl_11_func_800BDB88(gOvl11AreaGridProbePosition, 0xC000);
            entry = (s32 *)GetVal8005E524();
            if (*entry & 8) {
                cell = grid;
                cell += gOvl11AreaGridScanRow * 45 + gOvl11AreaGridScanColumn;
                *cell = entry;
            }
            gOvl11AreaGridScanColumn++;
            gOvl11AreaGridProbePosition[0] += 400;
        }
        gOvl11AreaGridScanRow++;
        gOvl11AreaGridProbePosition[2] -= 400;
    }
    ((Ovl11GridState *)gOvl11AreaGridState)->flags &= ~0x2000;
    return 1;
}

s32 Ovl11SetAreaGridCellTexture(s16 arg0, s16 arg1, s16 arg2);

void Ovl11ApplyAreaGridCellTextures(void) {
    u32 x;
    u32 y;
    u8 *cell;

    cell = gOvl11AreaGridState + 0x16 + *(s16 *)gOvl11AreaGridState * 100;
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++, cell++) {
            if (*cell == 1) {
                Ovl11SetAreaGridCellTexture(x, y, 1);
            }
            if (*cell == 2) {
                Ovl11SetAreaGridCellTexture(x, y, 2);
            }
        }
    }
}

s32 Ovl11SetAreaGridCellTexture(s16 column, s16 row, s16 selector) {
    Ovl11GridTextureEntry *entry;
    Ovl11GridUvRecord *uv;
    s32 header;
    s32 kind;

    entry = ((Ovl11GridTextureRowsView *)&D_8007AFF0)->entries[row][column];
    header = entry->header;
    if (header & 8) {
        uv = (Ovl11GridUvRecord *)((u8 *)&gOvl11AreaGridUvRecords + selector * 0xA);
        kind = header & 0xFDFF0000;
        if (kind == 0x3D010000) {
            entry->u0 = uv->u0;
            entry->v0 = uv->v0;
            entry->u1 = uv->u1;
            entry->v1 = uv->v1;
            entry->u2 = uv->u2;
            entry->v2 = uv->v2;
            entry->u3 = uv->u3;
            entry->v3 = uv->v3;
            entry->clut = uv->clut;
            return 0;
        }
        if (kind == 0x2D010000) {
            entry->u0 = uv->u0;
            entry->v0 = uv->v0;
            entry->u1 = uv->u1;
            entry->v1 = uv->v1;
            entry->u2 = uv->u2;
            entry->v2 = uv->v2;
            entry->u3 = uv->u3;
            entry->v3 = uv->v3;
            entry->clut = uv->clut;
            return 0;
        }
    }
    return 1;
}

s32 Ovl11PickAreaGridReward(s32 arg0);
s32 ovl_11_func_800CB540(u16 *arg0);

#define GRID ((Ovl11GridState *)gOvl11AreaGridState)

s32 Ovl11ClassifyAreaGridCell(Ovl11CE034Object *object) {
    s32 position[4];
    struct {
        s16 x;
        s16 y;
    } cell;
    struct {
        u16 id;
        u16 value;
        u16 count;
    } item;
    s16 *offset;

    offset = (s16 *)(gOvl11AreaGridCellOffsets + object->unk38 * 4);
    position[0] = object->unk100 + offset[0];
    position[1] = object->unk104;
    position[2] = object->unk108 + offset[1];
    if (Ovl11AreaGridCellFromPosition(position, (u16 *)&cell.x, (u16 *)&cell.y) != 0) {
        return 0;
    }
    if (GRID->grid[(s16)GRID->plane][cell.x][cell.y] != 0) {
        return 0;
    }
    if (cell.x == 0 || cell.y == 0 || cell.x == 9 || cell.y == 9) {
        Ovl11SetAreaGridCellTexture(cell.x, cell.y, 1);
        GRID->grid[(s16)GRID->plane][cell.x][cell.y] = 1;
        return 0;
    }
    if ((s16)GRID->plane != 0) {
        if (GRID->grid[(s16)GRID->plane - 1][cell.x][cell.y] == 2) {
            return 0;
        }
        if (cell.x >= GRID->centerX - 2 &&
            cell.x <= GRID->centerX + 2 &&
            cell.y >= GRID->centerY - 2 &&
            cell.y <= GRID->centerY + 2) {
            Ovl11SetAreaGridCellTexture(cell.x, cell.y, 1);
            GRID->grid[(s16)GRID->plane][cell.x][cell.y] = 1;
            return 0;
        }
    }
    if (GRID->probability > (s32)Rand(100) &&
        GRID->found[(s16)GRID->plane] == 0) {
        GRID->grid[(s16)GRID->plane][cell.x][cell.y] = 2;
        GRID->found[(s16)GRID->plane] = 1;
        Ovl11SetAreaGridCellTexture(cell.x, cell.y, 2);
        GRID->lastX = cell.x;
        GRID->lastY = cell.y;
    } else {
        Ovl11SetAreaGridCellTexture(cell.x, cell.y, 1);
        GRID->grid[(s16)GRID->plane][cell.x][cell.y] = 1;
        item.count = 1;
        item.value = 0;
        item.id = Ovl11PickAreaGridReward(((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474);
        if ((s16)item.id >= 0x186) {
            item.id = 0x10E;
        }
        if ((s16)item.id < 0) {
            item.id = 0x10F;
        }
        if ((s16)item.id != 0) {
            ovl_11_func_800CB540((u16 *)&item);
            GRID->probability += 2;
            return 1;
        }
        GRID->probability++;
    }
    return 0;
}

#undef GRID

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 pad;
} AreaGridPositionCopy;

/* D_8007AFF0 is the base of a large main-RAM work area; this function reads a
 * pointer out of the table at +0x23608, indexed by (row * 0x2D + column). */
typedef struct {
    char pad_0[0x23608];
    s32 *table[1];
} AreaGridPointerTableView;

s32 Ovl11AreaGridCellFromPosition(s32 *position, u16 *column, u16 *row) {
    AreaGridPositionCopy snapshot;
    s16 shiftedX;
    s16 cellColumn;
    s16 shiftedZ;
    s16 cellRow;
    s32 *entry;

    snapshot = *(AreaGridPositionCopy *)position;
    *column = *(u16 *)position + 0x7D0;
    *row = 0x7D0 - *(u16 *)((u8 *)position + 8);
    shiftedZ = (s16)*row;
    shiftedX = (s16)*column;
    *column = (s16)*column / 400;
    *row = (s16)*row / 400;
    if (shiftedX < 0) {
        *column -= 1;
        return 2;
    }
    cellColumn = (s16)*column;
    if (cellColumn < 0xA) {
        if (shiftedZ < 0) {
            *row -= 1;
            return 2;
        }
        cellRow = (s16)*row;
        if (cellRow < 0xA) {
            entry = ((AreaGridPointerTableView *)&D_8007AFF0)->table[cellRow * 0x2D + cellColumn];
            return (*entry & 8) < 1;
        }
        return 3;
    }
    return 3;
}

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void ovl_11_func_800CD670(s16 arg0);
void Ovl11UpdateAreaGridTrigger(void) {
    Ovl11TriggerWorkView *work;
    s16 mode;
    s32 object;
    s32 result;
    s32 dataOffset;

    if (((Ovl11GridState *)gOvl11AreaGridState)->flags & 0xC000) {
        dataOffset = 0;
        if (((Ovl11GridState *)gOvl11AreaGridState)->flags & 0x4000) {
            dataOffset = 0x34;
        }
        result = ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x3C, 0x20, 0x20);
        object = gActiveRenderContext->field_D8;
        func_8001AC10((u32 *)(object + 0x3C), object + 0x28,
                      (s32)(gOvl11AreaGridTriggerText + dataOffset));
        if (result == 1) {
            work = (Ovl11TriggerWorkView *)&D_8007AFF0;
            work->field_254A0 = 5;
            BeginScreenFadeFeedback(0xA);
            if (((Ovl11GridState *)gOvl11AreaGridState)->flags & 0x8000) {
                work->field_25480++;
                (*(u16 *)gOvl11AreaGridState)++;
                SetGameFlag(0x44, 1);
                ((Ovl11GridState *)gOvl11AreaGridState)->flags |= 0x2000;
            } else {
                mode = work->field_25474;
                if (mode == 0x37) {
                    work->field_25480 = 0x44;
                    SetGameFlag(0x43, 1);
                }
                if (work->field_25474 == 0x43) {
                    work->field_25480 = 0x48;
                    SetGameFlag(0x42, 1);
                }
                Ovl11ResetAreaGridState();
            }
            ((Ovl11GridState *)gOvl11AreaGridState)->flags &= ~0xC000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x10000000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x08000000;
            SetGameFlag(3, 0);
        } else if (result == 2) {
            ((Ovl11GridState *)gOvl11AreaGridState)->flags &= ~0xC000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x10000000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x08000000;
            SetGameFlag(3, 0);
            ovl_11_func_800CD670(0xA);
        }
    }
}

s32 Ovl11PickAreaGridReward(s32 areaId) {
    s16 area;
    s16 *weight;
    s16 *items;
    s32 cumulativeWeight;
    s32 roll;
    u32 itemIndex;
    s16 plane;

    area = (s16) areaId;
    plane = *(s16 *) gOvl11AreaGridState;
    weight = (s16 *) ((u8 *) gOvl11AreaGridRewardWeights + (plane << 4));
    items = gOvl11AreaGridRewardItems;
    if (area == 0x37) {
        weight = (s16 *) ((u8 *) gOvl11Area37RewardWeights + (plane << 4));
        items = gOvl11Area37RewardItems;
    }
    cumulativeWeight = 0;
    roll = Rand(100);
    for (itemIndex = 0; itemIndex < 8; itemIndex++, weight++) {
        cumulativeWeight += *weight;
        if (roll < cumulativeWeight) {
            if (area == 0x37) {
                if (items[itemIndex] == 0x67) {
                    if (GetGameFlag(0x62) != 0) {
                        return 0;
                    }
                    SetGameFlag(0x62, 1);
                }
                if (items[itemIndex] == 0x63) {
                    if (GetGameFlag(0x60) != 0) {
                        return 0;
                    }
                    SetGameFlag(0x60, 1);
                }
            } else if (items[itemIndex] == 0x67) {
                if (GetGameFlag(0x61) != 0) {
                    return 0;
                }
                SetGameFlag(0x61, 1);
            }
            return items[itemIndex];
        }
    }
    return 0;
}

void Ovl11RestoreAreaGridCenter(void);

void Ovl11RestoreAreaGridPosition(void) {
    u8 *base;
    u8 *b;
    s32 var_s1;

    base = (u8 *) D_8006C838;
    var_s1 = (*(s16 *) (base + 0x52C6)) == 0x42;
    if (GetGameFlag(0x44U) == 1) {
        b = gOvl11AreaGridState;
        (*(s32 *) (base + 0x52CC)) = 0;
        (*(s32 *) (base + 0x52C8)) = (s32) ((*(s16 *) (b + 8) * 0x190) - 0x708);
        (*(s32 *) (base + 0x52D0)) = (s32) (0x7D0 - ((*(s16 *) (b + 0xA) + 2) * 0x190));
        SetGameFlag(0x44U, 0U);
        var_s1 = 1;
    }
    if (var_s1 != 0) {
        Ovl11RestoreAreaGridCenter();
    }
}

void Ovl11RestoreAreaGridCenter(void) {
    char *base;
    char *p;
    char *b;

    if (*(s16 *)gOvl11AreaGridState == 0) {
        return;
    }
    base = (char *)&D_8007AFF0;
    p = base + 0x20000;
    b = (char *)gOvl11AreaGridState;
    *(s32 *)(p + 0x50E4) &= ~0x1000;
    *(s16 *)(p + 0x50FA) = 0;
    *(s16 *)(p + 0x50F8) = *(s16 *)(b + 8) * 400 - 2000;
    *(s16 *)(p + 0x50FC) = 1800 - *(s16 *)(b + 0xA) * 400;
    *(u16 *)(b + 4) = *(u16 *)(b + 8);
    *(u16 *)(b + 6) = *(u16 *)(b + 0xA);
}
