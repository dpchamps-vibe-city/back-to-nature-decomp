#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_11_func_800FE068(u16 arg0, s32 arg1, s32 arg2) {
    s16 var_a1;
    s16 var_s2;
    s32 temp_v1;
    s32 var_a2;
    s32 var_s0;
    s32 var_s1;
    u16 *p;

    var_a1 = arg1;
    var_s2 = arg2;
    var_a2 = 0;
    p = D_80127214;
    var_s1 = (var_a1 << 0x10) + 0x80000;
    do {
        var_s0 = var_a2 + 1;
        if ((u32) (arg0 & 0xFFFF) >= (u16) p[var_s0]) {
            DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) D_8012CEB8, var_a2 & 0xFF, 0, (s16) (s32) var_a1, var_s2);
            temp_v1 = var_s1 >> 0x10;
            var_s1 += 0x80000;
            var_a1 = (s16) temp_v1;
        }
        var_a2 = var_s0;
    } while (var_s0 < 4);
}
