#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"
#include "sprite_upload.h"

void ovl_17_func_800BA21C(void) {
    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {

    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}
