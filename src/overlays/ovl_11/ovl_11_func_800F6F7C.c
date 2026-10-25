#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_11_func_800F6F7C(s32 arg0, s16 arg1, s16 arg2) {
    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xB);
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 0);
    DrawSpriteFrameQuads(arg0, (s32)&gSharedSpriteSource, (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 4), (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 5), arg1, arg2, 0x1000, 0, 0);
}
