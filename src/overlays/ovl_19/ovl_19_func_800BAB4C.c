#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_19_func_800BADF8 (s32 arg0, SpriteSourceData *arg1, s16 *arg2, s16 arg3);

void ovl_19_func_800BAB4C(Ovl19Func800BAB4CArg *arg0) {
    ObjectState *temp_s0;
    s16 temp_v1;
    s32 var_a1;
    s32 var_s2;

    if (arg0->unk0 == 1) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) &arg0->sprite, 0, 0, 0, arg0->unk6);
    }
    var_a1 = 0;
    var_s2 = -1;
    temp_v1 = arg0->unk2;
    switch (temp_v1) {
    case 0:
        var_a1 = 0;
        break;
    case 1:
        var_a1 = 1;
        break;
    case 3:
        var_a1 = 3;
        break;
    case 2:
    case 4:
        var_a1 = 2;
        break;
    }
    temp_s0 = (ObjectState *) &arg0->sprite;
    if (arg0->sprite.field_4 != var_a1) {
        SelectSpriteAnimation(temp_s0, var_a1);
    }
    if (var_s2 != -1) {
        SelectSpriteAnimationFrame(temp_s0, var_s2 & 0xFF);
    }
    ovl_19_func_800BADF8(gActiveRenderContext->field_120, (SpriteSourceData *) temp_s0, arg0->position, 0);
}
