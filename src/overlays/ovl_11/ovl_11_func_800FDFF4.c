#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_11_func_800FDFF4(u16 arg0, s16 arg1, s16 arg2) {
    if (arg0 >= 5) {
        arg0 = 4;
    }
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, ((s32)(&D_8012CEB8)), D_8012737C[arg0 * 2], 0, arg1, arg2);
}
