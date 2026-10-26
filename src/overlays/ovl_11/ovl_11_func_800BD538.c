#include "common.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"
#include "ovl11_block_copy.h"

extern s32 gGraphicsHeapBase;
extern s32 D_8009A3F8;

void ovl_11_func_800BD538(void) {
    CdReadDiscSectorsBlocking(0, 0x0380B000, 0x5000, 0, gGraphicsHeapBase + 0x4290);
    Ovl11CopyWordBlock(&D_8009A3F8, gGraphicsHeapBase + 0x4290, 0x540U);
    UploadSpriteImages(gGraphicsHeapBase + 0x5790);
}
