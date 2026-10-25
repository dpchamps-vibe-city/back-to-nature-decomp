#include "common.h"
#include "game_types.h"

void func_80015840(ObjectState *obj, u8 arg1);

void func_8001585C(ObjectState *obj, u8 arg1);

void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2) {
    func_80015840(((ObjectState *)(((s32)(&gSharedSpriteSource)))), 11);
    func_8001585C(((ObjectState *)(((s32)(&gSharedSpriteSource)))), 2);
    func_80015EE8(arg0, ((s32)(&gSharedSpriteSource)), ((Recon_ovl_11_func_800F7010_D_800A0728View *)(&gSharedSpriteSource))->unk4, ((Recon_ovl_11_func_800F7010_D_800A0728View *)(&gSharedSpriteSource))->unk5, arg1, arg2);
}
