#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void func_800248E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_17_func_800BAEF0(void);
void ovl_17_func_800B9158(s32 arg0, s16 arg1);

void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1) {
    u8 *temp_s0;
    Recon_ovl_17_func_800BAD9C_AnimRef *temp_a3;
    s16 temp_s2;
    s16 temp_v0_2;
    s32 temp_s3;
    s32 temp_v0;
    s32 var_v1;
    s32 flag;

    temp_v0 = arg0->field_10 - arg1;
    var_v1 = temp_v0 >> 0xC;
    if (temp_v0 < 0) {
        var_v1 = (s32) (temp_v0 + 0xFFF) >> 0xC;
    }
    temp_v0_2 = 0xA0 - var_v1;
    temp_a3 = arg0->field_14;
    temp_s3 = gActiveRenderContext->field_D8 + (temp_a3->field_4 * 4);
    flag = arg0->field_0 & 1;
    temp_s0 = (u8 *) arg0 + 0x20;
    temp_s2 = temp_a3->field_0;
    if (!flag) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) temp_s0, 0, 0, 0, (s16) (s32) temp_a3->field_2);
    }
    DrawSpriteAnimationQuadsAndUpdate(temp_s3, (SpriteSourceData *) temp_s0, temp_v0_2, temp_s2);
    func_800248E8(temp_s3, temp_v0_2, temp_s2, 1);
    if (arg0->field_0 & 1) {
        ovl_17_func_800BAEF0();
        DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x10, (SpriteSourceData *) D_800BDAD4, (s16) (temp_v0_2 - 0x38), (s16) (temp_s2 - 0x38));
    }
    if (D_800BD864 == ((((D_800BD864 + 1) / 10) * 0xA) - 1)) {
        ovl_17_func_800B9158(arg0->field_10, temp_s2);
    }
}
