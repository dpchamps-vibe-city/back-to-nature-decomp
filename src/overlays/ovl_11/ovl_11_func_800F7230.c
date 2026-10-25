#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "cd_loader.h"

void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80017200(u8 *, s16, s16, s16, s16);

/* Finishes the CD load of the 0x50 sprite blocks starting at id arg0 (one
 * 0x800-byte block per id, read into D_8005E3B0 + 0x4290). For every id in
 * the request list chosen by arg1 that falls in the loaded range, parses the
 * block, takes the last piece of its two animation frames, claims a free
 * D_80129648 texture record, lays out its VRAM and CLUT positions and
 * uploads it. Returns 1 when func_80014CBC yields nothing, -1 when the 0x48
 * records are full and 0 when done; the caller 800F77F0 advances only on 0.
 *
 * Three constructs are here for the compiler, not for behaviour:
 *
 * - The empty "do { } while (0);" after func_80015704 puts loop notes
 *   between the call and the frame lookups. sched1 treats them as a barrier,
 *   so "tex2 = NULL; piece2 = NULL;" stay below the call. Floated above it,
 *   piece2 would live across the call, lose its allocation priority to tex
 *   and n, and s6/s7/fp would rotate. The LOOP_END note also ends cse1's
 *   block there. GCSE then inserts the second call's "id - first" at the end
 *   of the post-call block, where cse2 turns it into the copy from s0 that
 *   the target stores to 84(sp). This is probably a compiled-out debug macro
 *   in the original.
 *
 * - The request lists are indexed by n, not walked by a pointer. loop.c
 *   combines the three list addresses into one pointer stepped by 6. That
 *   pointer has one increment, too few references for caller-save, so it is
 *   spilled to 88(sp) after the GCSE temporary, as in the target.
 *
 * - frames is declared before anim. The two tie in global-alloc priority and
 *   the lower register number is allocated first, which puts frames in a3. */
s32 ovl_11_func_800F7230(s16 arg0, s16 arg1) {
    SpriteSourceData sprite;
    s32 first;
    s32 count;
    SpriteSourceData *src;
    s32 mode;
    s32 n;
    s32 i;
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

    first = arg0;
    if (CdReadArchiveBytes(0, (arg0 << 11) + 0x03C19000, 0x28000,
            (u8 *)D_8005E3B0 + 0x4290, 1, 0) != 0) {
        if (arg1 >= 0) {
            mode = 3;
            if ((u16)arg1 < 4U) {
                mode = arg1;
            }
            goto selected;
failed:
            return -1;
        } else {
            mode = 0;
        }
selected:
        count = D_80126F74[mode];
        for (n = 0; n < count; n++) {
            switch (mode) {
            case 0:
                id = ((Ovl11TextureRequests *)D_8006C838)->lists[1][n][0];
                break;
            case 1:
                if (n < 0x40) {
                    id = ((Ovl11TextureRequests *)D_8006C838)->lists[0][n][0];
                } else {
                    id = D_80126E30[n - 0x40];
                }
                break;
            default:
                id = ((Ovl11TextureRequests *)D_8006C838)->lists[2][n][0];
                break;
            }
            if (id == 0 || id < first || id >= first + 0x50) {
                continue;
            }
            tex = NULL;
            piece = NULL;
            func_80015704(&sprite, (SpriteDataHeader *)((u8 *)D_8005E3B0
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
            for (i = 0; i < 0x48; i++) {
                if (D_80129648_textures[i].id == 0) {
                    break;
                }
            }
            if (i == 0x48) {
                goto failed;
            }
            memset(&D_80129648_textures[i], 0, 0x22);
            D_80129648_textures[i].id = id;
            D_80129648_textures[i].x = (i % 8) * 8 + (s16)((i / 24) * 64 + 0x140);
            D_80129648_textures[i].y = ((i % 24) / 8) * 64;
            if (tex != NULL) {
                D_80129648_textures[i].width = tex->width * 4;
                D_80129648_textures[i].height = tex->height;
                D_80129648_textures[i].pieceX = piece->rest[1];
                D_80129648_textures[i].pieceY = piece->rest[2];
            }
            if (piece2 != NULL) {
                D_80129648_textures[i].x2 = (i % 8) * 8 + (s16)((i / 24) * 64 + 0x140);
                D_80129648_textures[i].y2 = ((i % 24) / 8) * 64 + 0x20;
            }
            if (tex2 != NULL) {
                D_80129648_textures[i].width2 = tex2->width * 4;
                D_80129648_textures[i].height2 = tex2->height;
                D_80129648_textures[i].pieceX2 = piece2->rest[1];
                D_80129648_textures[i].pieceY2 = piece2->rest[2];
            }
            D_80129648_textures[i].clutX = (i % 4) * 16 + ((i / 24) * 64 + 0x140);
            D_80129648_textures[i].clutY = (row = (i % 24) / 4) + 0xC0;
            uploadY = row - 0x3E;
            func_80017200((u8 *)D_8005E3B0 + (((id - first) << 11) + 0x4390),
                (s16)((u16)D_80129648_textures[i].x - 0x140),
                D_80129648_textures[i].y,
                (s16)(D_80129648_textures[i].clutX - 0x140), uploadY);
        }
        return 0;
    }
    return 1;
}
