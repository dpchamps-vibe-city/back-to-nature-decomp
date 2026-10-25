#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);
/* This caller passes only the two sprite-initialization arguments, as in
 * ovl_28_func_800B8B70; the matched callee's trailing parameters are unused. */
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80015840(ObjectState *obj, u8 arg1);

s32 ovl_27_func_800BA230(void) {
    if (CdReadArchiveBytes(0, 0x2000, 0x23000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) != 0) {
        func_8001719C((u8 *)(gGraphicsHeapBase + 0x4FAC));
        memcpy(D_800C4C00, (void *)(gGraphicsHeapBase + 0x4290), 0xD1C);
        func_80015704((SpriteSourceData *)D_800C4A80, (SpriteDataHeader *)D_800C4C00);
        func_80015704((SpriteSourceData *)D_800C4AB0, (SpriteDataHeader *)D_800C4C00);
        func_80015704((SpriteSourceData *)D_800C4AE0, (SpriteDataHeader *)D_800C4C00);
        func_80015704((SpriteSourceData *)D_800C4B10, (SpriteDataHeader *)D_800C4C00);
        func_80015704((SpriteSourceData *)D_800C4B40, (SpriteDataHeader *)D_800C4C00);
        func_80015704((SpriteSourceData *)D_800C4B70, (SpriteDataHeader *)D_800C4C00);
        func_80015704((SpriteSourceData *)D_800C4BA0, (SpriteDataHeader *)D_800C4C00);
        func_80015704((SpriteSourceData *)D_800C4BD0, (SpriteDataHeader *)D_800C4C00);
        func_80015840((ObjectState *)D_800C4A80, 0);
        func_80015840((ObjectState *)&D_800C4AB0, 0xA);
        func_80015840((ObjectState *)&D_800C4AE0, 0x13);
        func_80015840((ObjectState *)&D_800C4B10, 0x16);
        func_80015840((ObjectState *)&D_800C4B40, 0x18);
        func_80015840((ObjectState *)&D_800C4B70, 0x1A);
        func_80015840((ObjectState *)&D_800C4BA0, 0x1C);
        func_80015840((ObjectState *)D_800C4BD0, 0x1D);
        return 1;
    }
    return 0;
}
