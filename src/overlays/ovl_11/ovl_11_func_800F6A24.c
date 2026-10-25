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
        if (gOvl11TextureCache[i].id == 0) {
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
        gOvl11TextureCache[i].id = id;
        gOvl11TextureCache[i].width = tex->width * 4;
        gOvl11TextureCache[i].height = tex->height;
        gOvl11TextureCache[i].x = (i % 8) * 8 + (baseX = (i / 24) * 64 + 0x140);
        gOvl11TextureCache[i].y = ((i % 24) / 8) * 64;
        gOvl11TextureCache[i].clutX = (i % 4) * 16 + baseX;
        gOvl11TextureCache[i].clutY = (row = (i % 24) / 4) + 0xC0;
        uploadY = row - 0x3E;
        func_80017200((u8 *)&gMessageWindowReadBuffer + 0x100,
            (s16)((u16)gOvl11TextureCache[i].x - 0x140),
            gOvl11TextureCache[i].y,
            (s16)(gOvl11TextureCache[i].clutX - 0x140),
            uploadY);
    }
}
