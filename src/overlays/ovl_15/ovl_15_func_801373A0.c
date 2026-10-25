#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"

void ovl_15_func_801373A0(void) {
    u32 i;

    DrawSync(0);
    ClearOTagR((u_long *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0xE, 0, 0x8E90, D_80137830, 1, 1);
    do {

    } while (CdReadArchiveBytes(0xE, 0, 0x8E90, D_80137830, 1, 0) == 0);
    UploadSpriteImages(D_8013A5A4);
    memcpy(D_801428F0, &D_8013A5A4[-0x2D74], 0x2D74);
    for (i = 0; i < 5; i++) {
        InitSpriteSourceData(&D_80145670[i], (SpriteDataHeader *) &D_801428F0[D_8013766C[i]]);
    }
}
