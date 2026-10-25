#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void func_80024A4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4) {
    s16 arg3_16;

    arg3_16 = (s16)arg3;

    if (((SpriteSourceData *)&gSharedSpriteSource)->field_14 != (s32)&gSharedSpriteData) {
        ((void (*)(SpriteSourceData *, SpriteDataHeader *))InitSpriteSourceData)(
            (SpriteSourceData *)&gSharedSpriteSource, (SpriteDataHeader *)&gSharedSpriteData);
    }

    DrawSpriteFrameRects(arg0, (s32)&gSharedSpriteSource, arg1 & 0xFF, arg2 & 0xFF, arg3_16, arg4);
}
