#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

extern s16 D_800C4A2E;
extern u8 D_800C4AB0[];

void ovl_27_func_800B9A8C(void) {
    s32 var;
    s16 q;
    s16 r;

    var = (u16)D_800C4A2E + 1;
    q = (s16)var / 1800;
    D_800C4A2E = var - q * 1800;
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x8C, (SpriteSourceData *)D_800C4AB0, 0x21, 0x92);
    if (D_800C4A2E >= 0x3C) {
        D_800C4A2E = 0;
        r = Rand(9) + 10;
        if (r == (r / 2) * 2) {
            r -= 1;
        }
        if (r < 10) {
            r = 10;
        }
        SelectSpriteAnimation((ObjectState *)D_800C4AB0, r);
    }
}
