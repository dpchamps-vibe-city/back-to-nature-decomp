#include "common.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"

s32 ovl_27_func_800BA7B0(void) {
    if (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0) {
        return 0;
    }
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    return 1;
}
