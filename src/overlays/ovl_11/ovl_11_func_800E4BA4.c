#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"

extern s16 D_80129410;
extern s16 D_80129412;

s32 ovl_11_func_800E4B6C(s32 arg0, s32 arg1);

void ovl_11_func_800E4BA4(s32 arg0) {
    s16 *var_s0;
    s32 var_s1;

    var_s1 = 2;
    var_s0 = (s16 *)(arg0 + 4);
    do {
        if (*var_s0 & 0x8000) {
            UploadSpriteImagesKeepingWideType2Positions((u8 *)ovl_11_func_800E4B6C(arg0, var_s1 - 2), D_80129410, D_80129412, D_80129410, (s16)(s32)D_80129412);
        }
        var_s1 += 1;
        var_s0 += 1;
    } while (var_s1 < 0x1A);
}
