#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"


extern u16 D_80126E50;
extern s16 D_80126E52;

void ovl_11_func_800F8A3C(s32 arg0) {
    s32 temp_v0_2;
    s32 var_v0;

    SelectSpriteAnimation((ObjectState *) &gSharedSpriteSource, 0xD);
    SelectSpriteAnimationFrame((ObjectState *) &gSharedSpriteSource, D_80126E50);
    DrawSpriteFrameRects(arg0, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 4)), (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 5)), 0x7D, 0x90);
    SelectSpriteAnimation((ObjectState *) &gSharedSpriteSource, 0xC);
    SelectSpriteAnimationFrame((ObjectState *) &gSharedSpriteSource, D_80126E50);
    DrawSpriteFrameRects(arg0, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 4)), (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 5)), 0x115, 0x90);
    D_80126E52++;
    if (D_80126E52 >= 9) {
        D_80126E52 = 0;
        temp_v0_2 = (s16) D_80126E50 + 1;
        var_v0 = temp_v0_2 / 4;
        D_80126E50 = D_80126E50 - (var_v0 * 4 - 1);
    }
}
