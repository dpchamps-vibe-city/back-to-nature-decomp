/*
 * map_cell_texture — reconstructed translation unit.
 *
 * Mode-1 map grid construction and texture pass: build 25-by-45 cell pointers
 * from collision probes, then derive and apply the private UV/CLUT records.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "map_cell_texture.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 Ovl11SetMapCellTexture(s16 column, s16 row, s16 textureIndex) {
    s16 selectedTexture;
    u32 header;
    u32 kind;
    Ovl11GridTextureEntry *entry;
    Ovl11TextureCoordinates *coordinates;
    Ovl11TextureStateView *state;

    selectedTexture = textureIndex;
    state = (Ovl11TextureStateView *)&D_8007AFF0;
    if (state->mode != 1) {
        return 0;
    }
    entry = state->entries[row * 45 + column];
    if (((struct struct_8006C838_button *)D_8006C838)->field_44BA == 3) {
        selectedTexture = 0x1D;
    }
    header = entry->header;
    if (header & 8) {
        coordinates = &((Ovl11TextureCoordinates *)gOvl11MapCellUvRecords)[selectedTexture];
        kind = header & 0xFDFF0000;
        if (kind == 0x3D010000) {
            if (gOvl11MapColumnMirror[column] != 0) {
                setUV4(entry, coordinates->u0, coordinates->v0,
                       coordinates->u1, coordinates->v1, coordinates->u2,
                       coordinates->v2, coordinates->u3, coordinates->v3);
            } else {
                setUV4(entry, coordinates->u3, coordinates->v3,
                       coordinates->u2, coordinates->v2, coordinates->u1,
                       coordinates->v1, coordinates->u0, coordinates->v0);
            }
            entry->clut = coordinates->clut;
            if (selectedTexture == 0x1D) {
                entry->tpage = 7;
                return 0;
            }
            if ((selectedTexture >= 5) && (selectedTexture != 0x1D)) {
                entry->tpage = 0x1D;
                if (((struct struct_8006C838_button *)D_8006C838)->field_44BA == 2) {
                    entry->clut = (coordinates->clut & 0xFFC0) | 0x35;
                }
            } else {
                entry->tpage = 8;
            }
            return 0;
        }
        if (kind == 0x2D010000) {
            if (gOvl11MapColumnMirror[column] != 0) {
                setUV4(entry, coordinates->u0, coordinates->v0,
                       coordinates->u1, coordinates->v1, coordinates->u2,
                       coordinates->v2, coordinates->u3, coordinates->v3);
            } else {
                setUV4(entry, coordinates->u3, coordinates->v3,
                       coordinates->u2, coordinates->v2, coordinates->u1,
                       coordinates->v1, coordinates->u0, coordinates->v0);
            }
            entry->clut = coordinates->clut;
            if (selectedTexture == 0x1D) {
                entry->tpage = 7;
                return 0;
            }
            if (selectedTexture >= 5) {
                entry->tpage = 0x1D;
                if (((struct struct_8006C838_button *)D_8006C838)->field_44BA == 2) {
                    entry->clut = (coordinates->clut & 0xFFC0) | 0x35;
                }
            } else {
                entry->tpage = 8;
            }
            return 0;
        }
    }
    return 1;
}

void Ovl11ApplyMapTileTextures(void) {
    Ovl11D124Entry *entry;
    s16 textureIndex;
    u32 row;
    u16 textureCode;
    u32 column;
    u8 variant;

    entry = &gMainAreaGrid[0][0];
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
            Ovl11SetMapCellTexture((s16) column, (s16) row, textureIndex);
            column++;
            entry++;
        } while (column < 45U);
        row++;
    } while (row < 25U);
}

s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);
/* Overlay alias of the matched word-returning GetVal8005E524. */
s32 GetVal8005E524(void);

s32 Ovl11BuildMapGrid(s32 resumeWord) {
    s32 *entry;
    u8 *base;
    s32 **grid;
    s32 **cell;

    if ((resumeWord << 16) == 0) {
        gOvl11MapGridScanRow = 0;
        gOvl11MapGridProbePosition[1] = 0;
        gOvl11MapGridProbePosition[2] = 1500;
    }
    while (gOvl11MapGridScanRow < 25) {
        base = D_8009AFF0;
        gOvl11MapGridProbePosition[0] = -4200;
        for (gOvl11MapGridScanColumn = 0; gOvl11MapGridScanColumn < 45;) {
            grid = (s32 **)(base + 0x3608);
            ovl_11_func_800BDB88(gOvl11MapGridProbePosition, 0xC000);
            entry = (s32 *)GetVal8005E524();
            if (*entry & 8) {
                cell = grid;
                cell += gOvl11MapGridScanRow * 45 + gOvl11MapGridScanColumn;
                *cell = entry;
            }
            gOvl11MapGridScanColumn++;
            gOvl11MapGridProbePosition[0] += 400;
        }
        gOvl11MapGridScanRow++;
        gOvl11MapGridProbePosition[2] -= 400;
    }
    return 1;
}
