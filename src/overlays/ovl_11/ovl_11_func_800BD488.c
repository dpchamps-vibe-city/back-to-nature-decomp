#include "common.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"

extern s32 D_8012279C[];
extern s32 *D_80124FCC[];
extern s32 D_80098BF8;

void ovl_11_func_800BD488(s32 arg0) {
    s32 *p;
    s32 *q;
    s32 n;

    p = &D_8012279C[arg0];
    q = D_80124FCC[arg0 + 2];
    n = *(s32 *)((u8 *)q + (*q * 4) + 4);
    CdReadDiscSectorsBlocking(0, p[0], p[1] - p[0], 0, gGraphicsHeapBase + 0x4290);
    memcpy(&D_80098BF8, (void *)(gGraphicsHeapBase + 0x4290), n);
    n = n + 0x4290;
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + n));
}
