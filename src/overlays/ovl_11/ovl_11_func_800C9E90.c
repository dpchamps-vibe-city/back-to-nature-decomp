#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_11_func_800C9E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SelectSpriteAnimation(((ObjectState *)(((s32)(&gSharedSpriteSource)))), 0x18);
    ((Recon_ovl_11_func_800C9E90_D_800A0728View *)(&gSharedSpriteSource))->unk5 = 0;
    DrawSpriteFrameRects(arg0, ((s32)(&gSharedSpriteSource)), 0x18, arg3 & 0xFF, ((s16)arg1), ((s16)arg2));
}
