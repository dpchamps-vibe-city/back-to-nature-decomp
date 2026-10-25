#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_11_func_800FE640(void) {
    s32 temp_s0;
    s32 temp_s1;

    temp_s1 = gActiveRenderContext->field_D8 + 0x68;
    temp_s0 = ((s16) D_80127212 / 30) & 0xFF;
    DrawSpriteFrameRects(temp_s1, (s32) D_8012CE88, 0, temp_s0, 0x20, 0x18);
    DrawSpriteFrameRects(temp_s1, (s32) D_8012CE88, 2, temp_s0, 0x108, 0x18);
}
