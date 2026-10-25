#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"

void ovl_11_func_8011EE98(s16 arg0) {
    s32 temp_s0;
    struct struct_8012DA80 *temp_s2;
    struct struct_8012DA80 *temp_s3;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    arg0 = arg0 < 0 ? 0 : (arg0 > 3 ? 3 : arg0);
    temp_s0 = (arg0 * 0xE800) + 0x03D17000;
    CdReadArchiveBytes(0, temp_s0, 0xE800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, temp_s0, 0xE800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4690));
    D_8012D810 = *(Ov11SpriteDataBlock *)(gGraphicsHeapBase + 0x4290);
    InitSpriteSourceData((SpriteSourceData *)D_8012DA80, (SpriteDataHeader *)&D_8012D810);
    temp_s3 = D_8012DA80 + 1;
    InitSpriteSourceData((SpriteSourceData *)temp_s3, (SpriteDataHeader *)&D_8012D810);
    temp_s2 = D_8012DA80 + 2;
    InitSpriteSourceData((SpriteSourceData *)temp_s2, (SpriteDataHeader *)&D_8012D810);
    SelectSpriteAnimation((ObjectState *)D_8012DA80, 0);
    SelectSpriteAnimation((ObjectState *)temp_s3, 1);
    SelectSpriteAnimation((ObjectState *)temp_s2, 2);
    SelectSpriteAnimationFrame((ObjectState *)temp_s2, 1);
}
