#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void func_800248E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_17_func_800BAEF0(void);

void ovl_17_func_800BAD9C(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1) {
    Struct_800154CC *temp_s1;
    Recon_ovl_17_func_800BAD9C_AnimRef *temp_a3;
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_s4;
    s32 temp_v0 = arg0->field_10 - arg1;
    s32 var_v1;
    s32 flag;
    u8 *base;
    u8 *p;

    var_v1 = temp_v0 >> 0xC;
    if (temp_v0 < 0) {
        var_v1 = (temp_v0 + 0xFFF) >> 0xC;
    }
    temp_s3 = (s16)(0xA0 - var_v1);
    temp_a3 = arg0->field_14;
    temp_s4 = gActiveRenderContext->field_D8 + (temp_a3->field_4 * 4);
    flag = arg0->field_0 & 1;
    temp_s1 = (Struct_800154CC *)((u8 *)arg0 + 0x20);
    temp_s2 = temp_a3->field_0;
    if (!flag) {
        SetSpriteSourceTextureOffsets(temp_s1, 0, 0, 0, temp_a3->field_2);
    }
    DrawSpriteFrameRects(temp_s4, (s32)temp_s1, (s32)arg0->field_24, (s32)arg0->field_25,
                  temp_s3, temp_s2);
    func_800248E8(temp_s4, temp_s3, temp_s2, 1);
    if (arg0->field_0 & 1) {
        ovl_17_func_800BAEF0();
        p = D_800BDAD4;
        base = p - 0x28C;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x10, (s32)p,
                      (s32)base[0x290], (s32)base[0x291],
                      (s16)(temp_s3 - 0x38), (s16)(temp_s2 - 0x38));
    }
}
