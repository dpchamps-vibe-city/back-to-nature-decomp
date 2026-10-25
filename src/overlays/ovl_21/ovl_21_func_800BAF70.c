#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_21_func_800BAF70(void) {
    u8 *base;

    SelectSpriteAnimation((ObjectState *)&D_800C0AFC, 0);
    base = D_800C0AFC - 0x6B4;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&D_800C0AFC, base[0x6B8],
                  base[0x6B9], (s16)(HWD0 / 2), (s16)(VWD0 / 2));
}
