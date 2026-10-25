#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2) {
    SelectSpriteAnimation(((ObjectState *)(((s32)(&gSharedSpriteSource)))), 11);
    SelectSpriteAnimationFrame(((ObjectState *)(((s32)(&gSharedSpriteSource)))), 2);
    DrawSpriteFrameRects(arg0, ((s32)(&gSharedSpriteSource)), ((Recon_ovl_11_func_800F7010_D_800A0728View *)(&gSharedSpriteSource))->unk4, ((Recon_ovl_11_func_800F7010_D_800A0728View *)(&gSharedSpriteSource))->unk5, arg1, arg2);
}
