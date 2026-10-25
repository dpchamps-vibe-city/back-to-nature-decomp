#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_11_func_800F7094(s32 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 var_s1;

    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xA);
    arg1 += 2;
    arg2 += 0x16;
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 0xA);
    DrawSpriteFrameRects(arg0, (s32)&gSharedSpriteSource, (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 4), (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 5), arg1, arg2);

    arg1 += 0xA;
    var_s1 = arg3 / 10;
    if (var_s1 != 0) {
        SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, var_s1 & 0xFF);
        DrawSpriteFrameRects(arg0, (s32)&gSharedSpriteSource, (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 4), (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 5), arg1, arg2);
    }

    arg1 += 0xA;
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, (arg3 % 10) & 0xFF);
    DrawSpriteFrameRects(arg0, (s32)&gSharedSpriteSource, (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 4), (s32)*(u8 *)((u8 *)&gSharedSpriteSource + 5), arg1, arg2);
}
