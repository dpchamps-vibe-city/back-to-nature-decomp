/*
 * Batch sprite-texture streaming into the shared 72-slot texture cache.
 *
 * Each CD read covers 80 consecutive 0x800-byte sprite records. Installation
 * selects IDs from the work-area request lists, resolves up to two frames per
 * sprite, and uploads their image/CLUT data to the cache slot's VRAM region.
 * The halfword cursor encodes both the next batch ID and read/poll phase.
 * See ovl11_texture_stream.h for the public service contract.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "ovl11_texture_stream.h"

void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80017200(u8 *, s16, s16, s16, s16);

/* Internal helpers retain their existing linkage and ABI. The start result is
 * the loader's buffer pointer returned as a word; the stream ignores it.
 * Preserve the low-halfword sign extension before scaling the ID. */
s32 Ovl11StartTextureBatchRead(s32 firstId) {
    return CdReadArchiveBytes(0, (firstId << 0x10 >> 5) + 0x3C19000,
        0x28000, gGraphicsHeapBase + 0x4290, 1, 1);
}

/* 0 = installed, 1 = read still pending, -1 = no free cache record.
 * There is no rollback on exhaustion: earlier records stay installed.
 *
 * Keep the empty loop between source initialization and the second-frame
 * temporaries, indexed request traversal, and frames-before-anim declaration
 * order. Together they preserve the call-boundary lifetimes and spill layout
 * of this loop under the retail compiler. */
s32 Ovl11InstallTextureBatch(s16 firstId, s16 requestMode) {
    SpriteSourceData sprite;
    s32 first;
    s32 count;
    SpriteSourceData *src;
    s32 mode;
    s32 requestIndex;
    s32 slot;
    s32 row;
    s16 uploadY;
    u16 id;
    SpriteFramePiece *entries;
    SpriteTableRef *ref;
    u16 *frames;
    SpriteTableRef *anim;
    u16 *frame;
    SpriteFramePiece *piece;
    SpriteFramePiece *piece2;
    SpriteTextureRect *tex;
    SpriteTextureRect *tex2;

    first = firstId;
    if (CdReadArchiveBytes(0, (firstId << 11) + 0x03C19000, 0x28000,
            (u8 *)gGraphicsHeapBase + 0x4290, 1, 0) != 0) {
        if (requestMode >= 0) {
            mode = 3;
            if ((u16)requestMode < 4U) {
                mode = requestMode;
            }
            goto selected;
failed:
            return -1;
        } else {
            mode = 0;
        }
selected:
        count = gOvl11TextureRequestCounts[mode];
        for (requestIndex = 0; requestIndex < count; requestIndex++) {
            switch (mode) {
            case 0:
                id = ((Ovl11TextureRequests *)D_8006C838)->lists[1][requestIndex][0];
                break;
            case 1:
                if (requestIndex < 0x40) {
                    id = ((Ovl11TextureRequests *)D_8006C838)->lists[0][requestIndex][0];
                } else {
                    id = gOvl11ExtraTextureRequests[requestIndex - 0x40];
                }
                break;
            default:
                id = ((Ovl11TextureRequests *)D_8006C838)->lists[2][requestIndex][0];
                break;
            }
            if (id == 0 || id < first || id >= first + 0x50) {
                continue;
            }
            tex = NULL;
            piece = NULL;
            func_80015704(&sprite, (SpriteDataHeader *)((u8 *)gGraphicsHeapBase
                + (((id - first) << 11) + 0x4290)));
            do { } while (0);
            src = &sprite;
            tex2 = NULL;
            piece2 = NULL;
            anim = (SpriteTableRef *)src->field_28;
            frames = (u16 *)src->field_2C;
            frame = (u16 *)((u8 *)frames + anim[0].offset);
            if (*frame <= 0xFFFD) {
                ref = (SpriteTableRef *)src->field_20 + *frame;
                entries = (SpriteFramePiece *)(src->field_24 + ref->offset);
                piece = &entries[ref->count - 1];
                tex = (SpriteTextureRect *)src->field_1C + piece->index;
            }
            frame = (u16 *)((u8 *)frames + anim[1].offset);
            if (*frame <= 0xFFFD) {
                ref = (SpriteTableRef *)src->field_20 + *frame;
                entries = (SpriteFramePiece *)(src->field_24 + ref->offset);
                piece2 = &entries[ref->count - 1];
                tex2 = (SpriteTextureRect *)src->field_1C + piece2->index;
            }
            for (slot = 0; slot < 0x48; slot++) {
                if (gOvl11TextureCache[slot].id == 0) {
                    break;
                }
            }
            if (slot == 0x48) {
                goto failed;
            }
            memset(&gOvl11TextureCache[slot], 0, 0x22);
            gOvl11TextureCache[slot].id = id;
            gOvl11TextureCache[slot].x = (slot % 8) * 8 + (s16)((slot / 24) * 64 + 0x140);
            gOvl11TextureCache[slot].y = ((slot % 24) / 8) * 64;
            if (tex != NULL) {
                gOvl11TextureCache[slot].width = tex->width * 4;
                gOvl11TextureCache[slot].height = tex->height;
                gOvl11TextureCache[slot].pieceX = piece->rest[1];
                gOvl11TextureCache[slot].pieceY = piece->rest[2];
            }
            if (piece2 != NULL) {
                gOvl11TextureCache[slot].x2 = (slot % 8) * 8 + (s16)((slot / 24) * 64 + 0x140);
                gOvl11TextureCache[slot].y2 = ((slot % 24) / 8) * 64 + 0x20;
            }
            if (tex2 != NULL) {
                gOvl11TextureCache[slot].width2 = tex2->width * 4;
                gOvl11TextureCache[slot].height2 = tex2->height;
                gOvl11TextureCache[slot].pieceX2 = piece2->rest[1];
                gOvl11TextureCache[slot].pieceY2 = piece2->rest[2];
            }
            gOvl11TextureCache[slot].clutX = (slot % 4) * 16 + ((slot / 24) * 64 + 0x140);
            gOvl11TextureCache[slot].clutY = (row = (slot % 24) / 4) + 0xC0;
            uploadY = row - 0x3E;
            func_80017200((u8 *)gGraphicsHeapBase + (((id - first) << 11) + 0x4390),
                (s16)((u16)gOvl11TextureCache[slot].x - 0x140),
                gOvl11TextureCache[slot].y,
                (s16)(gOvl11TextureCache[slot].clutX - 0x140), uploadY);
        }
        return 0;
    }
    return 1;
}

s32 Ovl11ServiceTextureStream(s16 resetOrStep) {
    s32 stepResult;

    if (resetOrStep == -1) {
        gOvl11TextureStreamCursor = 0;
        return 0;
    } else {
        stepResult = Ovl11StepTextureStream(1);
        return ((s16)stepResult);
    }
}

s32 Ovl11StepTextureStream(s32 requestMode) {
    s32 result;

    /* Below 0x400 the cursor is a first ID; adding 0x400 marks an active
     * read. A successful install removes that marker and advances by 80. */
    if (gOvl11TextureStreamCursor < 0x400) {
        Ovl11StartTextureBatchRead(gOvl11TextureStreamCursor);
        gOvl11TextureStreamCursor += 0x400;
        return 1;
    }
    if (Ovl11InstallTextureBatch(gOvl11TextureStreamCursor - 0x400, requestMode) == 0) {
        gOvl11TextureStreamCursor -= 0x3B0;
        result = gOvl11TextureStreamCursor < 0x13F;
    } else {
        result = 1;
    }
    return result;
}
