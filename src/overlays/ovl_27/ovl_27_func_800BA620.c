#include "common.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"

s32 ovl_27_func_800BA620(void) {
    if (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0) {
        return 0;
    }
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    return 1;
}
