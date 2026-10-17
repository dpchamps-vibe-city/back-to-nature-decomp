#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800F7230", ovl_11_func_800F7230);


/* PARKED by /auto_decompilation_loop on 2026-10-16T23:30:37.254Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800F7230.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"

u8 *func_80014CBC(s32, s32, s32, u8 *, s32, s32);
void func_80015704();
void func_80017200(u8 *, s16, s16, s16, s16);

s32 ovl_11_func_800F7230(s16 arg0, s16 arg1) {
    SpriteSourceData sprite;
    SpriteSourceData *src;
    s32 first;
    s32 count;
    s32 mode;
    s32 relative;
    s32 n;
    s32 i;
    s32 baseX;
    s32 offset;
    s32 row;
    s16 uploadY;
    u16 id;
    u8 *list;
    u8 *entries;
    SpriteTableRef *ref;
    SpriteTableRef *anim;
    u16 *frames;
    u16 *frame;
    SpriteFramePiece *piece;
    SpriteFramePiece *piece2;
    SpriteTextureRect *tex;
    SpriteTextureRect *tex2;
    Ovl11TextureRecord *record;

    first = arg0;
    if (func_80014CBC(0, (arg0 << 11) + 0x03C19000, 0x28000,
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
        n = 0;
        if (count > 0) {
        list = (u8 *)D_8006C838;
        list += 0x480A;
        for (; n < count; n++, list += 6) {
            switch (mode) {
            case 0:
                id = *(u16 *)(list - 0x180);
                break;
            case 1:
                if (n < 0x40) {
                    id = *(u16 *)(list - 0x300);
                } else {
                    id = D_80126E30[n - 0x40];
                }
                break;
            default:
                id = *(u16 *)list;
                break;
            }
            if (id != 0 && id >= first && id < first + 0x50) {
                tex = NULL;
                piece = NULL;
                relative = id - first;
                func_80015704(&sprite, (SpriteDataHeader *)((u8 *)D_8005E3B0 + ((relative << 11) + 0x4290)));
                src = &sprite;
                tex2 = NULL;
                piece2 = NULL;
                anim = (SpriteTableRef *)src->field_28;
                frames = (u16 *)src->field_2C;
                frame = (u16 *)((u8 *)frames + anim[0].offset);
                if (*frame <= 0xFFFD) {
                    ref = (SpriteTableRef *)src->field_20 + *frame;
                    entries = (u8 *)(src->field_24 + ref->offset);
                    offset = ref->count * 12 - 12;
                    piece = (SpriteFramePiece *)(entries + offset);
                    tex = (SpriteTextureRect *)src->field_1C + piece->index;
                }
                frame = (u16 *)((u8 *)frames + anim[1].offset);
                if (*frame <= 0xFFFD) {
                    ref = (SpriteTableRef *)src->field_20 + *frame;
                    entries = (u8 *)(src->field_24 + ref->offset);
                    offset = ref->count * 12 - 12;
                    piece2 = (SpriteFramePiece *)(entries + offset);
                    tex2 = (SpriteTextureRect *)src->field_1C + piece2->index;
                }
                for (i = 0; i < 0x48; i++) {
                    if (D_80129648_textures[i].id == 0) {
                        break;
                    }
                }
                if (i != 0x48) {
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
                    record = &D_80129648_textures[i];
                    record->width2 = tex2->width * 4;
                    record->height2 = tex2->height;
                    D_8012965C[i][0] = piece2->rest[1];
                    D_8012965E[i][0] = piece2->rest[2];
                }
                D_80129648_textures[i].clutX = (i % 4) * 16 + (s16)((i / 24) * 64 + 0x140);
                D_80129648_textures[i].clutY = (row = (i % 24) / 4) + 0xC0;
                uploadY = row - 0x3E;
                func_80017200((u8 *)D_8005E3B0 + ((relative << 11) + 0x4390),
                    (s16)((u16)D_80129648_textures[i].x - 0x140),
                    D_80129648_textures[i].y,
                    (s16)(D_80129648_textures[i].clutX - 0x140), uploadY);
                } else {
                    goto failed;
                }
            }
        }
        }
        return 0;
    }
    return 1;
}
#endif
