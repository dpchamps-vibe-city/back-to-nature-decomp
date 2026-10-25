#include "common.h"
#include "game_types.h"
#include "cd_loader.h"

void func_80015704();
void func_80017200(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);

void ovl_11_func_800F6A24(u16 arg0) {
    SpriteSourceData sprite;
    SpriteSourceData *src;
    s32 i;
    s32 id;
    s32 baseX;
    s32 offset;
    s16 uploadY;
    s32 row;
    SpriteTableRef *ref;
    SpriteFramePiece *entry;
    SpriteTextureRect *tex;

    id = arg0 & 0xFFFF;
    for (i = 0; i < 0x48; i++) {
        if (D_80129648_textures[i].id == 0) {
            break;
        }
    }
    if (i != 0x48) {
        CdReadDiscSectorsBlocking(0, (id << 11) + 0x03C19000, 0x800, 0, (s32)&gMessageWindowReadBuffer);
        src = &sprite;
        func_80015704(src, (SpriteDataHeader *)&gMessageWindowReadBuffer);
        ref = (SpriteTableRef *)src->field_20 + *(u16 *)(src->field_2C + ((SpriteTableRef *)src->field_28)->offset);
        offset = ref->count * 12 - 12;
        entry = (SpriteFramePiece *)(src->field_24 + ref->offset);
        entry = (SpriteFramePiece *)((u8 *)entry + offset);
        tex = (SpriteTextureRect *)src->field_1C + entry->index;
        D_80129648_textures[i].id = id;
        D_80129648_textures[i].width = tex->width * 4;
        D_80129648_textures[i].height = tex->height;
        D_80129648_textures[i].x = (i % 8) * 8 + (baseX = (i / 24) * 64 + 0x140);
        D_80129648_textures[i].y = ((i % 24) / 8) * 64;
        D_80129648_textures[i].clutX = (i % 4) * 16 + baseX;
        D_80129648_textures[i].clutY = (row = (i % 24) / 4) + 0xC0;
        uploadY = row - 0x3E;
        func_80017200((u8 *)&gMessageWindowReadBuffer + 0x100,
            (s16)((u16)D_80129648_textures[i].x - 0x140),
            D_80129648_textures[i].y,
            (s16)(D_80129648_textures[i].clutX - 0x140),
            uploadY);
    }
}
