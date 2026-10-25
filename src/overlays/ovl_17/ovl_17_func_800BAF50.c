#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_17_func_800BAF50(void) {
    u8 *base;

    SelectSpriteAnimation((ObjectState *)&D_800BDA74, 4);
    base = D_800BDA74 - 0x22C;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&D_800BDA74, base[0x230],
                  base[0x231], 0, 0);
}
