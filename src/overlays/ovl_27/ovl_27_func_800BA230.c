#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_upload.h"

s32 ovl_27_func_800BA230(void) {
    if (CdReadArchiveBytes(0, 0x2000, 0x23000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) != 0) {
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4FAC));
        memcpy(D_800C4C00, (void *)(gGraphicsHeapBase + 0x4290), 0xD1C);
        InitSpriteSourceData((SpriteSourceData *)D_800C4A80, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4AB0, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4AE0, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4B10, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4B40, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4B70, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4BA0, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4BD0, (SpriteDataHeader *)D_800C4C00);
        SelectSpriteAnimation((ObjectState *)D_800C4A80, 0);
        SelectSpriteAnimation((ObjectState *)&D_800C4AB0, 0xA);
        SelectSpriteAnimation((ObjectState *)&D_800C4AE0, 0x13);
        SelectSpriteAnimation((ObjectState *)&D_800C4B10, 0x16);
        SelectSpriteAnimation((ObjectState *)&D_800C4B40, 0x18);
        SelectSpriteAnimation((ObjectState *)&D_800C4B70, 0x1A);
        SelectSpriteAnimation((ObjectState *)&D_800C4BA0, 0x1C);
        SelectSpriteAnimation((ObjectState *)D_800C4BD0, 0x1D);
        return 1;
    }
    return 0;
}
