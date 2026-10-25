#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "cd_loader.h"
#include "sprite_upload.h"

void ovl_11_func_800BD238(s32 arg0) {
    s32 *p;
    s32 temp_a1;
    s32 var_s1;
    char *base;
    char *state;

    base = (char *)&D_8007AFF0;
    state = base + 0x20000;
    if ((*(s16 *)(state + 0x5476) == 1) || (*(s16 *)(state + 0x5476) == 3)) {
        DrawSync(0);
        ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
        var_s1 = 0xA20;
        p = &D_80122804[arg0];
        temp_a1 = p[0];
        CdReadDiscSectorsBlocking(0, temp_a1, p[1] - temp_a1, 0, gGraphicsHeapBase + 0x4290);
        if (arg0 != 0) {
            var_s1 = 0x838;
        }
        memcpy(base + 8, (void *)(gGraphicsHeapBase + 0x4290), var_s1);
        if (*(s16 *)(state + 0x5476) == 3) {
            UploadSpriteImagesAtOffsets((u8 *)(gGraphicsHeapBase + 0x4290 + var_s1), -0x100, 0, 0, 0);
            return;
        }
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290 + var_s1));
    }
}
