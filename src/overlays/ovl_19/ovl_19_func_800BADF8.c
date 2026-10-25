#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_scene_camera_geometry.h"
#include "sprite_render.h"
#include "point_projection.h"

void func_800248E8(s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void ovl_19_func_800BADF8(s32 arg0, SpriteSourceData *arg1, s16 *arg2, s16 arg3) {
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a0;
    s32 var_s3;

    Ovl19AdjustPositionToSurface(arg2);
    D_800BD080[0] = arg2[0];
    D_800BD080[1] = arg2[1];
    D_800BD080[2] = arg2[2];
    temp_a2 = ProjectPointToScreen(D_800BD048, (SVECTOR *) D_800BD080);
    D_800BD048[1] += arg3;
    temp_v1 = (temp_a2 >> 3) * 4;
    temp_a1 = gActiveRenderContext->field_120;
    var_a0 = arg0 + temp_v1;
    var_s3 = temp_a1 + temp_v1;
    if ((arg1 == (SpriteSourceData *) gOvl19ActorSprites) ||
        (arg1 == (SpriteSourceData *) (gOvl19ActorSprites + 0x40)) ||
        (arg1 == (SpriteSourceData *) (gOvl19ActorSprites + 0x80))) {
        DrawSpriteAnimationQuadsAndUpdate(var_a0, arg1, (s16) D_800BD048[0], (s16) D_800BD048[1]);
        func_800248E8(var_s3, (s16) D_800BD048[0], (s16) D_800BD048[1], 0);
        return;
    }
    if (arg1 == (SpriteSourceData *) (gOvl19ActorSprites + 0x120)) {
        DrawSpriteAnimationQuadsAndUpdate(arg0, arg1, (s16) D_800BD048[0], (s16) D_800BD048[1]);
        return;
    }
    if ((arg2[1] != -0x64) && ((arg2[2] - arg2[0]) >= 0)) {
        temp_v0 = (temp_a2 >> 2) * 4;
        var_a0 = arg0 + temp_v0;
        var_s3 = temp_a1 + temp_v0;
    }
    DrawSpriteAnimationQuadsAndUpdate(var_a0, arg1, (s16) D_800BD048[0], (s16) D_800BD048[1]);
    func_800248E8(var_s3, (s16) D_800BD048[0], (s16) (D_800BD048[1] - arg3), 0);
}
