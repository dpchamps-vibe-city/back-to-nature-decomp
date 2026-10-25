#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sound_commands.h"
#include "sprite_source.h"
#include "sprite_upload.h"

void ovl_11_func_800FB908(void);

void ovl_11_func_800FBE88(void) {
    u32 i;

    PlaySoundEffect(3);
    ovl_11_func_800FB908();
    D_8012720C = 1;
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0xE, 0, 0x8E90, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0xE, 0, 0x8E90, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x7004));
    memcpy(D_8012A048, (u8 *)(gGraphicsHeapBase + 0x4290), 0x2D74);
    for (i = 0; i < 5; i++) {
        InitSpriteSourceData((SpriteSourceData *)(D_8012CDF8 + i * 0x30),
                      (SpriteDataHeader *)(D_8012A048 + D_801272F4[i]));
    }
}
