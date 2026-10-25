#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_27_func_800B998C(void) {
    s32 var;
    s16 q;
    s16 r;

    var = (u16)D_800C4A2C + 1;
    q = (s16)var / 1800;
    D_800C4A2C = var - q * 1800;
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x8C, (SpriteSourceData *)D_800C4A80, 0x11A, 0xAD);
    if (D_800C4A2C >= 0x5A) {
        D_800C4A2C = 0;
        r = Rand(0xA);
        if (r == (r / 2) * 2) {
            r -= 1;
        }
        if (r < 0) {
            r = 0;
        }
        SelectSpriteAnimation((ObjectState *)D_800C4A80, r);
    }
}
