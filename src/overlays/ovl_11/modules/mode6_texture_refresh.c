/*
 * mode6_texture_refresh — reconstructed translation unit.
 *
 * Mode-6 seven-by-seven cell texture refresh: derive texture indices from map
 * cell tags and apply the private UV/CLUT records to existing textured
 * entries.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "mode6_texture_refresh.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

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

    entry = &D_80074124[0][0];
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
