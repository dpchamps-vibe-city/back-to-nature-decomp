#include "common.h"
#include "screen_primitives.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_11_func_800F7094(s32 arg0, s16 arg1, s16 arg2, s32 arg3);

void ovl_11_func_800FD968(s32 *arg0, s16 arg1) {
    s32 temp_s2;
    s16 temp_v0_2;
    s16 var_a1;
    s16 var_a3;
    s16 y0;
    s32 var_v0;
    s32 var_v0_2;
    s32 temp_s5;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s1;
    u32 *temp_s4;
    u32 *temp_s6;

    temp_v0 = gActiveRenderContext->field_D8;
    var_s1 = 0;
    temp_s4 = (u32 *)(temp_v0 + 0x60);
    temp_s6 = (u32 *)(temp_v0 + 0x5C);
    temp_s5 = temp_v0 + 0x58;
    do {
        var_a1 = 0x28;
        if (var_s1 % 3 != 0) {
            var_a1 = 0xA8;
        }
        var_v0 = arg1;
        temp_v1 = var_s1 & 2;
        temp_s2 = arg1 + 8;
        if (temp_v1 != 0) {
            var_v0 = temp_s2;
        }
        y0 = var_v0;
        var_a3 = 0xA8;
        if (temp_v1 != 0) {
            var_a3 = 0x28;
        }
        var_v0_2 = arg1;
        if (var_s1 % 3 != 0) {
            var_v0_2 = temp_s2;
        }
        DrawScreenLine(temp_s4, var_a1, y0, var_a3, var_v0_2, 0xE0B888, 0);
        var_s1 += 1;
    } while (var_s1 < 4);
    temp_v0_2 = arg1 + 1;
    DrawScreenFlatRect((s32 *)temp_s4, 0x29, temp_v0_2, 0x80, 7, 0xC8A060, 0);
    DrawScreenLine(temp_s6, 0x68, arg1, 0x68, temp_s2, 0xE0B888, 0);
    var_a3 = ((arg0[1] % 10000) * 64) / 10000;
    DrawScreenGradientRect((s32 *)temp_s6, 0x69 - var_a3, temp_v0_2, var_a3, 7, 0xE8A0B8, 0xD14070, 0xE8A0B8, 0xD14070, 0);
    if (arg0[1] / 10000 != 0) {
        DrawSpriteFrameRects(temp_s5, (s32)D_8012CE88, 0xA, 0, 0x28, arg1 - 0x18);
        ovl_11_func_800F7094(temp_s5, 0x40, arg1 - 0x20, arg0[1] / 10000 - (arg0[1] / 1000000) * 100);
    }
    DrawScreenGradientRect((s32 *)temp_s6, 0x69, temp_v0_2, ((arg0[0] % 10000) * 64) / 10000, 7, 0x4D70A0, 0xA6B8D0, 0x4D70A0, 0xA6B8D0, 0);
    if (arg0[0] / 10000 != 0) {
        DrawSpriteFrameRects(temp_s5, (s32)D_8012CE88, 0xA, 0, 0x68, arg1 - 0x18);
        ovl_11_func_800F7094(temp_s5, 0x80, arg1 - 0x20, arg0[0] / 10000 - (arg0[0] / 1000000) * 100);
    }
}
