/*
 * Synchronous single-ID path of the shared texture cache. Activity is a
 * per-pass retention mark: misses discard the highest-index inactive ID,
 * then fill the lowest-index free slot. The batch streamer and packet
 * builders share the records but are separate services.
 * Membership and boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_texture_cell_cache.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "texture_sprite_draw.h"
#include "ovl11_slot_decorations.h"

/* Clear occupancy and activity without erasing texture geometry. */
void Ovl11ResetTextureCellCache(void) {
    s32 i;

    for (i = 0; i <= 0x47; i++) {
        gOvl11TextureCache[i].active = 0;
        gOvl11TextureCache[i].id = 0;
    }
}

void Ovl11ClearTextureCellActivity(void) {
    s32 i;

    i = 0x47;
    do {
        gOvl11TextureCache[i].active = 0;
        i--;
    } while (i >= 0);
}

/* Uses the last piece of animation 0's frame. Unwritten geometry and activity
 * remain stale, unlike the batch installer's whole-record clear. */
void Ovl11LoadTextureCellRecord(u16 textureId) {
    SpriteSourceData sprite;
    SpriteSourceData *source;
    s32 i;
    s32 id;
    s32 baseX;
    s32 lastPieceOffset;
    s16 uploadY;
    s32 row;
    SpriteTableRef *frame;
    SpriteFramePiece *piece;
    SpriteTextureRect *texture;

    id = textureId & 0xFFFF;
    for (i = 0; i < 0x48; i++) {
        if (gOvl11TextureCache[i].id == 0) {
            break;
        }
    }
    if (i != 0x48) {
        CdReadDiscSectorsBlocking(0, (id << 11) + 0x03C19000, 0x800, 0, (s32)&gMessageWindowReadBuffer);
        source = &sprite;
        InitSpriteSourceData(source, (SpriteDataHeader *)&gMessageWindowReadBuffer);
        frame = (SpriteTableRef *)source->field_20 + *(u16 *)(source->field_2C + ((SpriteTableRef *)source->field_28)->offset);
        lastPieceOffset = frame->count * 12 - 12;
        piece = (SpriteFramePiece *)(source->field_24 + frame->offset);
        piece = (SpriteFramePiece *)((u8 *)piece + lastPieceOffset);
        texture = (SpriteTextureRect *)source->field_1C + piece->index;
        gOvl11TextureCache[i].id = id;
        gOvl11TextureCache[i].width = texture->width * 4;
        gOvl11TextureCache[i].height = texture->height;
        gOvl11TextureCache[i].x = (i % 8) * 8 + (baseX = (i / 24) * 64 + 0x140);
        gOvl11TextureCache[i].y = ((i % 24) / 8) * 64;
        gOvl11TextureCache[i].clutX = (i % 4) * 16 + baseX;
        gOvl11TextureCache[i].clutY = (row = (i % 24) / 4) + 0xC0;
        uploadY = row - 0x3E;
        UploadSpriteImagesAtOffsets((u8 *)&gMessageWindowReadBuffer + 0x100,
            (s16)((u16)gOvl11TextureCache[i].x - 0x140),
            gOvl11TextureCache[i].y,
            (s16)(gOvl11TextureCache[i].clutX - 0x140),
            uploadY);
    }
}

/* Hit-only retry path: ID zero submits the separate shared-sprite fallback. */
void Ovl11DrawCachedTextureCell(s32 *ot, u16 textureId, s16 x, s16 y) {
    s32 i;
    s32 id;

    id = textureId & 0xFFFF;
    if (id == 0) {
        Ovl11DrawEmptySlot((s32) (ot + 1), x, y);
        return;
    }
    for (i = 0; i < 0x48; i++) {
        if (gOvl11TextureCache[i].id == id) {
            gOvl11TextureCache[i].active = 1;
            Ovl11DrawTextureSpriteInCell(ot, i, x, y);
            return;
        }
    }
}

void Ovl11DrawTextureCell(s32 *ot, u16 textureId, s16 x, s16 y) {
    s32 i;
    u16 id;

    id = textureId & 0xFFFF;
    if (id == 0) {
        Ovl11DrawEmptySlot((s32)(ot + 1), x, y);
        return;
    }
    for (i = 0; i < 0x48; i++) {
        if (gOvl11TextureCache[i].id == id) {
            gOvl11TextureCache[i].active = 1;
            Ovl11DrawTextureSpriteInCell(ot, i, x, y);
            return;
        }
    }
    for (i = 0x47; i >= 0; i--) {
        if (gOvl11TextureCache[i].active == 0) {
            gOvl11TextureCache[i].id = 0;
            Ovl11LoadTextureCellRecord(id);
            Ovl11DrawCachedTextureCell(ot, id, x, y);
            return;
        }
    }
}

/* Retained original quad variant (no external retail caller). ID zero draws
 * nothing. Scale is divided by 4096 before multiplying the cached dimensions. */
void Ovl11DrawScaledTextureCell(s32 *ot, u16 textureId, s16 x, s16 y, s32 scale4096) {
    s32 i;
    u16 id;

    id = textureId & 0xFFFF;
    if (id != 0) {
        for (i = 0; i < 0x48; i++) {
            if (gOvl11TextureCache[i].id == id) {
                gOvl11TextureCache[i].active = 1;
                Ovl11DrawScaledTextureQuad(ot, i, x, y, scale4096);
                return;
            }
        }
        for (i = 0x47; i >= 0; i--) {
            if (gOvl11TextureCache[i].active == 0) {
                gOvl11TextureCache[i].id = 0;
                Ovl11LoadTextureCellRecord(id);
                Ovl11DrawScaledTextureCell(ot, id, x, y, scale4096);
                return;
            }
        }
    }
}
