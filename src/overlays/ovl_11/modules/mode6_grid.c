/* Mode-6 seven-by-seven surface grid. Build installs bit-3 polygon pointers
 * at 400-unit cell centers; lookup uses those same cells, while refresh maps
 * the seven-by-seven logical map onto the private UV/CLUT table.
 * Pointer rows retain the shared engine's 45-entry stride. The builder's
 * scratch counters/position are private to this scan, not reentrant state.
 * Original ovl_11:8011DF4C–8011E574. Public contracts: ovl11_mode6_grid.h,
 * ovl11_grid_position.h and mode6_texture_refresh.h. */

#include "common.h"
#include "game_types.h"
#include "mode6_texture_refresh.h"
#include "ovl11_mode6_grid.h"
#include "ovl11_grid_position.h"
#include "ovl11_surface_height.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/* Last polygon-query record, returned through the original word ABI. */
s32 GetVal8005E524(void);

s32 Ovl11BuildMode6SurfaceGrid(s32 resume) {
    s32 *entry;
    u8 *base;
    s32 **grid;
    s32 **cell;

    if ((resume << 16) == 0) {
        gOvl11Mode6GridScanRow = 0;
        gOvl11Mode6GridProbePosition[0] = -1620;
        gOvl11Mode6GridProbePosition[1] = 0;
        gOvl11Mode6GridProbePosition[2] = 1020;
    }
    while (gOvl11Mode6GridScanRow < 7) {
        base = D_8009AFF0;
        gOvl11Mode6GridProbePosition[0] = -1420;
        for (gOvl11Mode6GridScanColumn = 0; gOvl11Mode6GridScanColumn < 7;) {
            grid = (s32 **)(base + 0x3608);
            Ovl11ResolveSurfaceHeight(gOvl11Mode6GridProbePosition, 0xC000);
            entry = (s32 *)GetVal8005E524();
            if (*entry & 8) {
                cell = grid;
                cell += gOvl11Mode6GridScanRow * 45 + gOvl11Mode6GridScanColumn;
                *cell = entry;
            }
            gOvl11Mode6GridScanColumn++;
            gOvl11Mode6GridProbePosition[0] += 400;
        }
        gOvl11Mode6GridScanRow++;
        gOvl11Mode6GridProbePosition[2] -= 400;
    }
    return 1;
}

/* Maps a world position to a cell of the 7x7, 400-unit field grid (x biased
 * by 0x654, z mirrored about 0x4C4) and stores the column/row. Returns 2 when
 * the position is below the grid (the cell index is stepped down by one), 3
 * when it is past it, and otherwise 1 if the far-state mode at +0x25476 is 6
 * and the cell's entry in the 45-wide pointer table at +0x23608 has bit 3
 * clear, else 0. ovl_11_func_800D7EF8 is the 45x25 counterpart for mode 1. */
s32 Ovl11LookupMode6GridPosition(s32 *position, u16 *colOut, u16 *rowOut) {
    VECTOR copy;
    u8 *base;
    u8 *p;
    u8 *tbl;
    s32 *cell;
    s32 idx;
    s16 x;
    s16 z;
    s16 col;
    s16 row;

    /* Preserve the existing aggregate binding while sharing the XYZ pointer
     * interface with the mode-1 handler; only X/Z affect the lookup. */
    copy = *(VECTOR *)position;
    *colOut = position[0] + 0x654;
    *rowOut = 0x4C4 - position[2];
    z = *rowOut;
    x = *colOut;
    *colOut = (s16)*colOut / 400;
    *rowOut = (s16)*rowOut / 400;
    if (x < 0) {
        *colOut -= 1;
        return 2;
    }
    col = *colOut;
    if (col >= 7) {
        return 3;
    }
    if (z < 0) {
        *rowOut -= 1;
        return 2;
    }
    row = *rowOut;
    if (row >= 7) {
        return 3;
    }
    base = (u8 *)&D_8007AFF0;
    idx = (row * 0x2D + col) * 4;
    p = base + 0x20000;
    tbl = p + 0x3608;
    cell = *(s32 **)(tbl + idx);
    if (*(s16 *)(p + 0x5476) == 6) {
        if ((*cell & 8) == 0) {
            return 1;
        }
    } else {
        return 0;
    }
    return 0;
}

s32 Ovl11SetMode6CellTexture(s16 column, s16 row, s16 textureIndex) {
    u32 header;
    u32 kind;
    Ovl11GridTextureEntry *entry;
    Ovl11TextureCoordinates *coords;
    Ovl11TextureStateView *state;

    state = (Ovl11TextureStateView *)&D_8007AFF0;
    if (state->mode != 6) {
        return 0;
    }
    entry = state->entries[row * 45 + column];
    header = entry->header;
    if (header & 8) {
        coords = &((Ovl11TextureCoordinates *)gOvl11Mode6TextureCoords)[textureIndex];
        kind = header & 0xFDFF0000;
        if (kind == 0x3D010000) {
            if (row != 0) {
                setUV4(entry, coords->u0, coords->v0, coords->u1, coords->v1,
                       coords->u2, coords->v2, coords->u3, coords->v3);
            } else {
                setUV4(entry, coords->u3, coords->v3, coords->u2, coords->v2,
                       coords->u1, coords->v1, coords->u0, coords->v0);
            }
            if (textureIndex >= 5) {
                entry->tpage = 0x1D;
            } else {
                entry->tpage = 9;
            }
            entry->clut = coords->clut;
            return 0;
        }
        if (kind == 0x2D010000) {
            if (row != 0) {
                setUV4(entry, coords->u0, coords->v0, coords->u1, coords->v1,
                       coords->u2, coords->v2, coords->u3, coords->v3);
            } else {
                setUV4(entry, coords->u3, coords->v3, coords->u2, coords->v2,
                       coords->u1, coords->v1, coords->u0, coords->v0);
            }
            if (textureIndex >= 5) {
                entry->tpage = 0x1D;
            } else {
                entry->tpage = 9;
            }
            entry->clut = coords->clut;
            return 0;
        }
    }
    return 1;
}

void Ovl11RefreshMode6CellTextures(void) {
    Ovl11D124Entry *entry;
    s16 textureIndex;
    u32 row;
    u16 textureCode;
    u32 column;
    u8 variant;

    entry = &gAlternateAreaGrid[0][0];
    row = 0;
    do {
        column = 0;
        do {
            textureCode = (u16) entry->unk2;
            textureIndex = 0;
            switch (textureCode) {
            case 0x167:
                textureIndex = 0;
                break;
            case 0x168:
                textureIndex = 1;
                break;
            case 0x169:
                textureIndex = 2;
                break;
            case 0x16A:
                textureIndex = 3;
                break;
            case 0x16B:
                textureIndex = 4;
                break;
            }
            if ((u16) entry->unk0 == 0x36) {
                if (entry->unk4 != 0) {
                    variant = entry->unk4;
                    textureIndex = ((variant - 1) * 8) | 5;
                    textureIndex += (u16) entry->unk6 & 0xF;
                }
            }
            Ovl11SetMode6CellTexture((s16) column, (s16) row, textureIndex);
            column++;
            entry++;
        } while (column < 7U);
        row++;
    } while (row < 7U);
}
