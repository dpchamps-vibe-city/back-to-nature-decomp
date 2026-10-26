/* Map-cell sprite traversal and mode-dependent kind/value resolution.
 * Public contracts: ovl11_cell_sprite_dispatch.h. Private callbacks retain
 * their original linkage. Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"
#include "ovl11_cell_sprite_dispatch.h"
#include "ovl11_projected_record_sprites.h"

s32 Ovl11ResolveCellSpriteMode0(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset);
s32 Ovl11ResolveCellSpriteMode1(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset);
s32 Ovl11ResolveCellSpriteMode2(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset);
s32 Ovl11ResolveCellSpriteFallback(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset);
s32 Ovl11SetCellSpriteIfNonzero(s32 value, s32 frame, s32 textureIndex, s32 *sprite, s32 *texture);

/* Output [minX,minY,maxX,maxY]; the maxima are exclusive traversal bounds. */
void Ovl11ComputeCellSpriteBounds(s32 *bounds) {
    char *work = (char *)&D_8007AFF0;
    s32 corner;
    s32 centerZ;
    s32 centerX;
    s32 row;
    s32 worldX;
    s32 column;

    corner = 0;
    centerX = *(s16 *)(work + 0x253B4);
    centerZ = *(s16 *)(work + 0x253B8);
    do {
        if (corner != 0) {
            worldX = centerX + 0xE10;
            row = centerZ - 0xE10;
        } else {
            worldX = centerX - 0xE10;
            row = centerZ + 0xE10;
        }
        column = (worldX + 0x1130) / 400;
        row = (row - 0x640) / -400;
        if (column < 0) {
            column = 0;
        } else if (column >= 0x2E) {
            column = 0x2D;
        }
        if (row < 0) {
            row = 0;
        } else if (row >= 0x1A) {
            row = 0x19;
        }
        bounds[corner * 2] = column;
        bounds[corner * 2 + 1] = row;
        corner += 1;
    } while (corner < 2);
}

void Ovl11DrawVisibleCellSprites(void) {
    s32 bounds[4];
    s16 position[4];
    s32 projected[4];
    s32 sprite;
    s32 texture;
    s32 yOffset;
    s32 y;
    s32 x;

    if (!(((Ovl11D93C8StateView *)D_8006C838)->flags & 0x200)) {
        Ovl11ComputeCellSpriteBounds(bounds);
        if (((Ovl11D93C8StateView *)D_8006C838)->mode == 0) {
            gOvl11CellSpriteResolver = Ovl11ResolveCellSpriteMode0;
        } else if (((Ovl11D93C8StateView *)D_8006C838)->mode == 1) {
            gOvl11CellSpriteResolver = Ovl11ResolveCellSpriteMode1;
        } else if (((Ovl11D93C8StateView *)D_8006C838)->mode == 2) {
            gOvl11CellSpriteResolver = Ovl11ResolveCellSpriteMode2;
        } else {
            gOvl11CellSpriteResolver = Ovl11ResolveCellSpriteFallback;
        }
        for (y = bounds[1]; y < bounds[3]; y++) {
            for (x = bounds[0]; x < bounds[2]; x++) {
                position[0] = x * 400 - 4050;
                position[1] = -100;
                position[2] = 1250 - y * 400;
                if (Ovl11ProjectRecordSprite((u16 *)position, projected) != 0 &&
                    gOvl11CellSpriteResolver(((Ovl11D93C8MapView *)D_8006C838)->cells[y][x].kind,
                        ((Ovl11D93C8MapView *)D_8006C838)->cells[y][x].value,
                        &sprite, &texture, &yOffset) != 0) {
                    DrawSpriteFrameRects(gActiveRenderContext->field_120 + ((projected[2] >> 2) * 4),
                        (s32)((u8 *)D_8006C838 + 0x3BC8 + texture * 0x30),
                        (u8)sprite, 0, (s16)projected[0], (s16)(projected[1] + yOffset));
                }
            }
        }
    }
}

/* Each mode supplies a complete mapping, including the shared base kinds.
 * All outputs are initialized even when the kind is unsupported. */
s32 Ovl11ResolveCellSpriteMode0(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset) {
    s32 result;

    result = 1;
    *sprite = 0;
    *texture = 0;
    *yOffset = 0;
    switch (kind) {
    case 0x3F: *texture = 5; break;
    case 0x175: *sprite = 1; *texture = 5; break;
    case 0x40: *texture = 6; break;
    case 0x3A: *texture = 0; break;
    case 0x3E: *texture = 4; break;
    case 0x16D: *texture = 1; *yOffset = -10; break;
    case 0x170: *texture = 2; *yOffset = -10; break;
    case 0x173: *texture = 3; *yOffset = -15; break;
    case 0x64: *texture = 15; break;
    case 0x22: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 7, sprite, texture); break;
    case 0x23: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 8, sprite, texture); break;
    case 0x24: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 9, sprite, texture); break;
    case 0x25: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 10, sprite, texture); break;
    case 0x26: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 11, sprite, texture); break;
    case 0x31: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 12, sprite, texture); break;
    case 0x34: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 13, sprite, texture); break;
    default: result = 0; break;
    }
    return result;
}

s32 Ovl11ResolveCellSpriteMode1(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset) {
    s32 result;

    result = 1;
    *sprite = 0;
    *texture = 0;
    *yOffset = 0;
    switch (kind) {
    case 0x3F: *texture = 5; break;
    case 0x175: *sprite = 1; *texture = 5; break;
    case 0x40: *texture = 6; break;
    case 0x3A: *texture = 0; break;
    case 0x3E: *texture = 4; break;
    case 0x16D: *texture = 1; *yOffset = -10; break;
    case 0x170: *texture = 2; *yOffset = -10; break;
    case 0x173: *texture = 3; *yOffset = -15; break;
    case 0x27: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 12, sprite, texture); break;
    case 0x28: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 13, sprite, texture); break;
    case 0x29: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 14, sprite, texture); break;
    case 0x2A: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 15, sprite, texture); break;
    case 0x2B: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 16, sprite, texture); break;
    case 0x32: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 17, sprite, texture); break;
    case 0x35: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 18, sprite, texture); break;
    case 0x22: result = Ovl11SetCellSpriteIfNonzero(value, 0, 7, sprite, texture); break;
    case 0x23: result = Ovl11SetCellSpriteIfNonzero(value, 0, 8, sprite, texture); break;
    case 0x24: result = Ovl11SetCellSpriteIfNonzero(value, 0, 9, sprite, texture); break;
    case 0x25: result = Ovl11SetCellSpriteIfNonzero(value, 0, 10, sprite, texture); break;
    case 0x26: result = Ovl11SetCellSpriteIfNonzero(value, 0, 11, sprite, texture); break;
    case 0x31: result = Ovl11SetCellSpriteIfNonzero(value, 0, 40, sprite, texture); break;
    case 0x34: result = Ovl11SetCellSpriteIfNonzero(value, 0, 41, sprite, texture); break;
    default: result = 0; break;
    }
    return result;
}

s32 Ovl11ResolveCellSpriteMode2(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset) {
    s32 result;

    result = 1;
    *sprite = 0;
    *texture = 0;
    *yOffset = 0;
    switch (kind) {
    case 0x3F: *texture = 5; break;
    case 0x175: *sprite = 1; *texture = 5; break;
    case 0x40: *texture = 0x27; break;
    case 0x3A: *texture = 0; break;
    case 0x3E: *texture = 4; break;
    case 0x16D: *texture = 1; *yOffset = -10; break;
    case 0x170: *texture = 2; *yOffset = -10; break;
    case 0x173: *texture = 3; *yOffset = -15; break;
    case 0x2C: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 12, sprite, texture); break;
    case 0x2D: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 13, sprite, texture); break;
    case 0x2E: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 14, sprite, texture); break;
    case 0x2F: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 15, sprite, texture); break;
    case 0x30: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 16, sprite, texture); break;
    case 0x179: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 25, sprite, texture); break;
    case 0x178: result = Ovl11SetCellSpriteIfNonzero(value, value + 2, 17, sprite, texture); break;
    case 0x27: result = Ovl11SetCellSpriteIfNonzero(value, 0, 7, sprite, texture); break;
    case 0x28: result = Ovl11SetCellSpriteIfNonzero(value, 0, 8, sprite, texture); break;
    case 0x29: result = Ovl11SetCellSpriteIfNonzero(value, 0, 9, sprite, texture); break;
    case 0x2A: result = Ovl11SetCellSpriteIfNonzero(value, 0, 10, sprite, texture); break;
    case 0x2B: result = Ovl11SetCellSpriteIfNonzero(value, 0, 11, sprite, texture); break;
    case 0x32: result = Ovl11SetCellSpriteIfNonzero(value, 0, 37, sprite, texture); break;
    case 0x35: result = Ovl11SetCellSpriteIfNonzero(value, 0, 38, sprite, texture); break;
    default: result = 0; break;
    }
    return result;
}

s32 Ovl11ResolveCellSpriteFallback(s32 kind, s32 value, s32 *sprite, s32 *texture, s32 *yOffset) {
    s32 result;

    result = 1;
    *sprite = 0;
    *texture = 0;
    *yOffset = 0;
    switch (kind) {
    case 0x3F: *texture = 5; break;
    case 0x175: *sprite = 1; *texture = 5; break;
    case 0x3A: *texture = 0; break;
    case 0x3E: *texture = 4; break;
    case 0x16D: *texture = 1; *yOffset = -10; break;
    case 0x170: *texture = 2; *yOffset = -10; break;
    case 0x173: *texture = 3; *yOffset = -15; break;
    default: result = 0; break;
    }
    return result;
}

/* A zero value leaves both outputs untouched; callers initialize them first. */
s32 Ovl11SetCellSpriteIfNonzero(s32 value, s32 frame, s32 textureIndex, s32 *sprite, s32 *texture) {
    if (value == 0) {
        return 0;
    }
    *sprite = frame;
    *texture = textureIndex;
    return 1;
}

void Ovl11DrawFixedCellSpriteGrid(void) {
    s16 position[4];
    s32 projected[4];
    s32 sprite;
    s32 texture;
    s32 y;
    s32 x;
    u16 kind;
    u8 value;
    u8 *map;

    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            position[0] = x * 400 - 1250;
            position[1] = -100;
            position[2] = 850 - y * 400;
            if (Ovl11ProjectRecordSprite((u16 *)position, projected) != 0) {
                sprite = 0;
                texture = 0;
                map = (u8 *)D_8006C838;
                kind = ((Ovl11D9F1CMapView *)map)->cells[y * 7 + x].kind;
                value = ((Ovl11D9F1CMapView *)(map + (y * 7 + x) * 8))->cells[0].value;
                switch (kind) {
                case 0x3F: texture = 3; break;
                case 0x175: sprite = 1; texture = 3; break;
                case 0x3A: texture = 0; break;
                case 0x3E: texture = 2; break;
                case 0x22: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 4, &sprite, &texture)) continue; break;
                case 0x23: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 5, &sprite, &texture)) continue; break;
                case 0x24: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 6, &sprite, &texture)) continue; break;
                case 0x25: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 7, &sprite, &texture)) continue; break;
                case 0x26: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 8, &sprite, &texture)) continue; break;
                case 0x27: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 9, &sprite, &texture)) continue; break;
                case 0x28: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 10, &sprite, &texture)) continue; break;
                case 0x29: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 11, &sprite, &texture)) continue; break;
                case 0x2A: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 12, &sprite, &texture)) continue; break;
                case 0x2B: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 13, &sprite, &texture)) continue; break;
                case 0x2C: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 14, &sprite, &texture)) continue; break;
                case 0x2D: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 15, &sprite, &texture)) continue; break;
                case 0x2E: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 16, &sprite, &texture)) continue; break;
                case 0x2F: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 17, &sprite, &texture)) continue; break;
                case 0x30: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 18, &sprite, &texture)) continue; break;
                case 0x31: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 20, &sprite, &texture)) continue; break;
                case 0x32: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 21, &sprite, &texture)) continue; break;
                case 0x178: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 22, &sprite, &texture)) continue; break;
                case 0x179: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 27, &sprite, &texture)) continue; break;
                case 0x34: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 23, &sprite, &texture)) continue; break;
                case 0x35: if (!Ovl11SetCellSpriteIfNonzero(value, value + 2, 24, &sprite, &texture)) continue; break;
                default: continue;
                }
                DrawSpriteFrameRects(gActiveRenderContext->field_120 + ((projected[2] >> 2) * 4),
                    (s32)(&gSpriteSourceBankBase + texture), (u8)sprite, 0,
                    (s16)projected[0], (s16)projected[1]);
            }
        }
    }
}
