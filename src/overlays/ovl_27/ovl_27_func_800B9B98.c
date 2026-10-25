#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_27_func_800B9B98(void) {
    s32 var;
    s16 q;
    s16 r;

    var = (u16)D_800C4A30 + 1;
    q = (s16)var / 1800;
    D_800C4A30 = var - q * 1800;
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x8C, (SpriteSourceData *)D_800C4AE0, 0x33, 0xB6);
    if (D_800C4A30 >= 0x3C) {
        D_800C4A30 = 0;
        r = Rand(3) + 0x13;
        if (r == 0x13) {
            D_800C4A30 = 0x1E;
        }
        SelectSpriteAnimation((ObjectState *)D_800C4AE0, r);
    }
}
