#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_11_func_800FE704(void) {
    s32 v = (((s16)D_80127212) / 30) & 0xFF;

    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, ((s32)(&D_8012CE88)), 1, v, 0x120,
                  0x30);
}
