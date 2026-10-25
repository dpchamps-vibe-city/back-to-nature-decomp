#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_11_func_800FC8C8(void) {
    Ovl11D80127328Entry *var_s0;
    u8 *base;
    s16 var_s6;
    s32 temp_v1;
    s32 var_a1;
    s32 var_s1;
    u32 var_s2;

    var_s2 = 0;
    var_a1 = 0x8C;
    var_s6 = 0xC0;
    base = (u8 *) &D_8006C838;
    var_s0 = D_80127328;
    var_s1 = 0xA40000;
    do {
        if ((*(s32 *) (base + 0x44F8)) & var_s0->unk0) {
            DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) D_8012CE88, (s32) var_s0->unk4, 0, (s16) var_a1, (s16) var_s6);
            temp_v1 = (s16) (var_s1 >> 0x10);
            var_s1 += 0x180000;
            var_a1 = temp_v1;
        }
        var_s2 += 1;
        var_s0 += 1;
    } while (var_s2 < 6U);
}
