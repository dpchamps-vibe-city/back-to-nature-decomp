#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

extern s16 D_800C4A32;
extern u8 D_800C4B10[];

void ovl_27_func_800B9CB4(void) {
    s32 var;
    s16 q;
    s32 arg0;

    var = (u16)D_800C4A32 + 1;
    q = (s16)var / 1800;
    D_800C4A32 = var - q * 1800;
    arg0 = gActiveRenderContext->field_D8 + 0x8C;
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B10, 0x127, 0xCD);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B10, 0x102, 0x8A);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B10, 0x118, 0x79);
    if (D_800C4A32 >= 0xF) {
        D_800C4A32 = 0;
        SelectSpriteAnimation((ObjectState *)D_800C4B10, Rand(2) + 0x16);
    }
}
