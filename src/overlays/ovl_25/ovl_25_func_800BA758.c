#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

extern s16 D_800BCD60[4];
extern s16 D_800BCDCC[4];

s32 func_8001DFD4(s32 *arg0, s16 *arg1);

void ovl_25_func_800BA758(SpriteSourceData *arg0, u16 *arg1) {
    s32 ret;
    s32 off;
    s32 base;

    D_800BCDCC[0] = arg1[0];
    D_800BCDCC[1] = arg1[1];
    D_800BCDCC[2] = arg1[2];
    D_800BCDCC[3] = arg1[3];
    ret = func_8001DFD4((s32 *)D_800BCD60, (s16 *)D_800BCDCC);
    off = ret + 0x400;
    base = gActiveRenderContext->field_120;
    DrawSpriteAnimationQuadsAndUpdate(base + (off >> 2) * 4, arg0, D_800BCD60[0], D_800BCD60[2]);
}
