#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

extern s16 D_800C4A36;
extern u8 D_800C4B70[];

void ovl_27_func_800B9EEC(void) {
    s32 var;
    s16 q;
    s32 arg0;

    var = (u16)D_800C4A36 + 1;
    q = (s16)var / 1800;
    D_800C4A36 = var - q * 1800;
    arg0 = gActiveRenderContext->field_D8 + 0x8C;
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B70, 0x17, 0x1E);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B70, 0x6, 0x6D);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B70, 0x24, 0xDF);
    if (D_800C4A36 >= 0x1E) {
        D_800C4A36 = 0;
        SelectSpriteAnimation((ObjectState *)D_800C4B70, Rand(2) + 0x1A);
    }
}
