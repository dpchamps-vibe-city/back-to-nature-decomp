#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_11_func_80107E2C(u8 *arg0, s32 arg1, s16 arg2, s16 arg3) {
    s16 temp_v0;
    s16 frame;

    if (*(s16 *)(arg0 + 2) != 0) {
        temp_v0 = GetSpriteAnimationFrameCount((SpriteSourceData *)&gSharedSpriteSource, *(s16 *)arg0);
        frame = (s16)(*(s16 *)(arg0 + 2) / 10) % temp_v0;
        DrawSpriteFrameRects(arg1, (s32)&gSharedSpriteSource, arg0[0],
                     (temp_v0 - frame - 1) & 0xFF, arg2, arg3);
    }
}
