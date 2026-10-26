/*
 * map_texture_camera — reconstructed translation unit.
 *
 * Map-view camera phases, held-pad panning and seasonal 25-by-45 tile
 * retexturing. The refresh builds UV/CLUT coordinates in one shared scratch
 * record, then patches each supported textured grid entry in place.
 * Public camera operations are documented in map_texture_camera.h.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "screen_fade.h"
#include "map_texture_camera.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "map_cell_texture.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"

void Ovl11EnterMapCameraMode(void) {
    char *far_base = (char *)&D_8007AFF0;

    *(s32 *)(far_base + 0x2548C) = 3;
    *(s32 *)(far_base + 0x254A0) = 0x17;
    gOvl11MapCameraState = 0;
}

void Ovl11StepMapCameraVerticalOffset(void) {
    char *far_base = (char *)&D_8007AFF0;

    if (*(s32 *)(far_base + 0x254A0) == 0x17) {
        Ovl11MapCameraState *state = (Ovl11MapCameraState *)&gOvl11MapCameraState;

        state->verticalAccumulator += state->verticalStep;
        *(u16 *)(far_base + 0x253B6) += (u16)state->verticalAccumulator;
        state->verticalStep -= 4;
    }
}

void Ovl11RefreshMapTileTextures(void);
void Ovl11ResetSceneCameraOrbit(void);

void Ovl11UpdateMapCameraMode(void) {
    s32 upOffset;
    s32 downOffset;
    s32 leftOffset;
    s32 rightOffset;

    switch (((Ovl11MapCameraState *)&gOvl11MapCameraState)->phase) {
    case 0:
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_25394 = -0xB6E;
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_25398 = -0x7454;
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_253A0 = 0;
        ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x800000;
        ((Ovl11MapCameraState *)&gOvl11MapCameraState)->phase++;
        return;
    case 1:
        Ovl11LoadFixedSpriteImages();
        Ovl11RefreshMapTileTextures();
        BeginScreenFadeDecrease(10, 0, 2);
        ((Ovl11MapCameraState *)&gOvl11MapCameraState)->phase++;
        ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x800000;
        return;
    case 2:
        if (((u32 *)gPad0Input)[1] & 0x1000) {
            upOffset = ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panZ;
            if (upOffset < 400) {
                upOffset += 200;
            }
            ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panZ = upOffset;
        }
        if (((u32 *)gPad0Input)[1] & 0x4000) {
            downOffset = ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panZ;
            if (downOffset > -7200) {
                downOffset -= 200;
            }
            ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panZ = downOffset;
        }
        if (((u32 *)gPad0Input)[1] & 0x8000) {
            leftOffset = ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panX;
            if (leftOffset > -800) {
                leftOffset -= 200;
            }
            ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panX = leftOffset;
        }
        if (((u32 *)gPad0Input)[1] & 0x2000) {
            rightOffset = ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panX;
            if (rightOffset < 10000) {
                rightOffset += 200;
            }
            ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panX = rightOffset;
        }
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_253B4 = ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panX;
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_253B8 = ((Ovl11MapCameraState *)&gOvl11MapCameraState)->panZ;
        if (((u32 *)gPad0Input)[2] & 0x20) {
            BeginScreenFadeIncrease(10, 0, 2);
            ((Ovl11MapCameraState *)&gOvl11MapCameraState)->phase = 3;
            return;
        }
        return;
    case 3:
        if (IsScreenFadeComplete() == 1) {
            BeginScreenFadeDecrease(10, 0, 2);
            Ovl11ResetSceneCameraOrbit();
            Ovl11ApplyMapTileTextures();
            ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_2548C = 0;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x800000;
        }
        break;
    }
}

/* Unsupported cell kinds leave the output record unchanged. */
s32 Ovl11BuildMapTileTexture(Ovl11TextureCoordinates *coordinates, s32 cellKind) {
    s32 textureIndex;
    s32 vOffset;
    u16 packedCode;
    s32 result;
    u16 textureColumn;
    s32 clutY;
    s32 rightU;
    s32 bottomV = 0;
    s32 clutX;
    u32 leftU;
    s32 clutRow;
    s32 topV;
    u32 clutBits;
    s32 season = gCalendarSeason;

    switch (cellKind) {
    case 0x26:
        textureIndex = 0;
        break;
    case 0x25:
        textureIndex = 1;
        break;
    case 0x24:
        textureIndex = 2;
        break;
    case 0x31:
        textureIndex = 3;
        break;
    case 0x34:
        textureIndex = 4;
        break;
    case 0x23:
        textureIndex = 5;
        break;
    case 0x22:
        textureIndex = 6;
        break;
    case 0x35:
        textureIndex = 7;
        break;
    case 0x2A:
        textureIndex = 8;
        break;
    case 0x2B:
        textureIndex = 9;
        break;
    case 0x32:
        textureIndex = 0xA;
        break;
    case 0x29:
        textureIndex = 0xB;
        break;
    case 0x27:
        textureIndex = 0xC;
        break;
    case 0x28:
        textureIndex = 0xD;
        break;
    case 0x33:
    case 0x53:
    case 0x178:
        textureIndex = 0xE;
        break;
    case 0x54:
    case 0x179:
        textureIndex = 0xF;
        break;
    case 0x2F:
        textureIndex = 0x10;
        break;
    case 0x2C:
        textureIndex = 0x11;
        break;
    case 0x2D:
        textureIndex = 0x12;
        break;
    case 0x30:
        textureIndex = 0x13;
        break;
    case 0x2E:
        textureIndex = 0x14;
        break;
    case 0x3A:
        textureIndex = 0x15;
        if (season == 3) {
            textureIndex = 0x25;
        }
        break;
    case 0x40:
        textureIndex = 0x16;
        if (season == 2) {
            textureIndex = 0x17;
        }
        break;
    case 0x3E:
        textureIndex = 0x18;
        if (season == 3) {
            textureIndex = 0x26;
        }
        break;
    case 0x3F:
        textureIndex = 0x19;
        if (season == 3) {
            textureIndex = 0x24;
        }
        break;
    case 0x175:
        textureIndex = 0x1A;
        if (season == 3) {
            textureIndex = 0x23;
        }
        break;
    case 0x3B:
    case 0x3C:
        textureIndex = 0x1B;
        if (season == 3) {
            textureIndex = 0x27;
        }
        break;
    case 0x16C:
    case 0x16F:
        textureIndex = 0x1C;
        if (season == 3) {
            textureIndex = 0x28;
        }
        break;
    case 0x16D:
    case 0x170:
        textureIndex = 0x1E;
        if (season == 3) {
            textureIndex = 0x2A;
        }
        break;
    case 0x16E:
    case 0x171:
        textureIndex = 0x1D;
        if (season == 3) {
            textureIndex = 0x29;
        }
        break;
    case 0x3D:
        textureIndex = 0x1F;
        if (season == 3) {
            textureIndex = 0x2B;
        }
        break;
    case 0x172:
        textureIndex = 0x20;
        if (season == 3) {
            textureIndex = 0x2C;
        }
        break;
    case 0x173:
        textureIndex = 0x22;
        if (season == 3) {
            textureIndex = 0x2E;
        }
        break;
    case 0x174:
        textureIndex = 0x21;
        if (season == 3) {
            textureIndex = 0x2D;
        }
        break;
    case 0:
    default:
        return 0;
    }
    packedCode = gOvl11MapTileTextureCodes[textureIndex];
    result = 1;
    textureColumn = (u32) (packedCode & 0xE00) >> 9;
    rightU = textureColumn;
    rightU <<= 5;
    rightU |= 0x1F;
    vOffset = (packedCode & 0x1C0) / 2;
    bottomV = vOffset + 0x11F;
    coordinates->u0 = rightU;
    coordinates->v0 = bottomV;
    leftU = textureColumn << 5;
    coordinates->u1 = leftU;
    coordinates->u3 = leftU;
    topV = vOffset + 0x100;
    coordinates->v1 = bottomV;
    coordinates->u2 = rightU;
    coordinates->v2 = topV;
    coordinates->v3 = topV;
    clutBits = (u32) packedCode >> 1;
    clutRow = packedCode & 0x1F;
    clutX = (clutBits & 0x10) + 0x3C0;
    clutY = clutRow + 0x1E0;
    coordinates->clut = (clutX >> 4) | (clutY << 6);
    return result;
}

/* Caller supplies in-range column/row and an existing, non-null grid entry.
 * The two header classes have separate arms to retain their original stores. */
s32 Ovl11ApplyMapTileTexture(s16 column, s16 row, Ovl11TextureCoordinates *coordinates) {
    s32 byte;
    s16 bit;
    Ovl11GridTextureEntry *entry;
    u32 header;
    u32 kind;
    s32 flags;
    s32 forward;

    byte = column / 8;
    bit = column % 8;
    flags = gOvl11MapColumnUvOrientation[byte] & (0x80 >> bit);
    forward = flags != 0;
    entry = ((Ovl11GridTextureTableView *)&D_8007AFF0)->entries[row * 45 + column];
    header = entry->header;
    if (header & 8) {
        kind = header & 0xFDFF0000;
        if (kind == 0x3D010000) {
            if (forward) {
                setUV4(entry, coordinates->u0, coordinates->v0, coordinates->u1, coordinates->v1,
                       coordinates->u2, coordinates->v2, coordinates->u3, coordinates->v3);
            } else {
                setUV4(entry, coordinates->u3, coordinates->v3, coordinates->u2, coordinates->v2,
                       coordinates->u1, coordinates->v1, coordinates->u0, coordinates->v0);
            }
            entry->tpage = 0x1F;
            entry->clut = coordinates->clut;
            return 1;
        }
        if (kind == 0x2D010000) {
            if (forward) {
                setUV4(entry, coordinates->u0, coordinates->v0, coordinates->u1, coordinates->v1,
                       coordinates->u2, coordinates->v2, coordinates->u3, coordinates->v3);
            } else {
                setUV4(entry, coordinates->u3, coordinates->v3, coordinates->u2, coordinates->v2,
                       coordinates->u1, coordinates->v1, coordinates->u0, coordinates->v0);
            }
            entry->tpage = 0x1F;
            entry->clut = coordinates->clut;
            return 1;
        }
    }
    return 0;
}

void Ovl11RefreshMapTileTextures(void) {
    s32 row;
    s32 column;

    for (row = 0; row < 25; row++) {
        for (column = 0; column < 45; column++) {
            if (Ovl11BuildMapTileTexture(&gOvl11MapTextureScratch,
                    ((Ovl11D93C8MapView *) D_8006C838)->cells[row][column].kind) == 1) {
                Ovl11ApplyMapTileTexture(column, row, &gOvl11MapTextureScratch);
            }
        }
    }
}
