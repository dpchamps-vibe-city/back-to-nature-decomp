#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

extern s16 D_800C4A34;
extern u8 D_800C4B40[];

void ovl_27_func_800B9DBC(void) {
    s32 var;
    s16 q;
    s32 arg0;

    var = (u16)D_800C4A34 + 1;
    q = (s16)var / 1800;
    D_800C4A34 = var - q * 1800;
    arg0 = gActiveRenderContext->field_D8 + 0x8C;
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B40, 0x3F, 0x95);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B40, 0xA, 0xBC);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B40, 9, 0xD6);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B40, 0x127, 0x34);
    DrawSpriteAnimationQuadsAndUpdate(arg0, (SpriteSourceData *)D_800C4B40, 0x126, 0x1E);
    if (D_800C4A34 >= 0x14) {
        D_800C4A34 = 0;
        SelectSpriteAnimation((ObjectState *)D_800C4B40, Rand(2) + 0x18);
    }
}
