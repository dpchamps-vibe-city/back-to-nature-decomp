#include "common.h"
#include "game_types.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"

void ovl_11_func_800BDA20(void) {
    CdReadDiscSectorsBlocking(0, 0x3D51000, 0x6800, 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages(((void *)(gGraphicsHeapBase + 0x4290)));
}
