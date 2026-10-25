#include "common.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"

void ovl_11_func_800DA588(s32 arg0);

extern s32 D_801227CC[];
extern s32 *D_80125528[];
extern u8 D_800957F8[];

void ovl_11_func_800BD5A8(s32 arg0) {
    s32 *p;
    s32 *q;
    s32 n;

    if (arg0 != -1) {
        p = &D_801227CC[arg0];
        q = D_80125528[arg0];
        n = *(s32 *)((u8 *)q + (*q * 4) + 4);
        CdReadDiscSectorsBlocking(0, p[0], p[1] - p[0], 0, gGraphicsHeapBase + 0x4290);
        memcpy(&D_800957F8, (void *)(gGraphicsHeapBase + 0x4290), n);
        n = n + 0x4290;
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + n));
        ovl_11_func_800DA588(arg0);
    }
}
