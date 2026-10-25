#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_23_func_800BB1B8(void) {
    u8 *base;

    SelectSpriteAnimation((ObjectState *)&D_800BFC30, 5);
    base = D_800BFC30 - 0x3B4;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&D_800BFC30, base[0x3B8],
                  base[0x3B9], 0, 0);
}
