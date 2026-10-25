#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_27_func_800B9324(void) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x88, ((s32)(&D_800C4BD0)), 0x1E, 0, 0, 0);
}
