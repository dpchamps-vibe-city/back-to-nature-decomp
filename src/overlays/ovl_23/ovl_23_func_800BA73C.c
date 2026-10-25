#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

s32 ovl_23_func_800BB040(s32 arg0);
s32 ovl_23_func_800BAFFC(s32 arg0);
void DrawSpriteAnimationQuadsAndUpdate();
void DrawSpriteFrameRects();
void func_800248E8();
void ovl_23_func_800B94D0();

void ovl_23_func_800BA73C(Ovl23BA73CArg *arg0) {
    ObjectState *temp_s1;
    s16 temp_v0;
    s16 var_a1;
    s32 temp_s0;
    s32 temp_s3;
    s32 temp_s4;
    s32 var_s5;

    temp_v0 = arg0->unk4 - 1;
    var_s5 = -1;
    switch (temp_v0) {
    case 0:
        var_a1 = arg0->unk6 + 0xD;
        break;
    case 1:
        var_a1 = arg0->unk6 + 0x15;
        break;
    case 2:
        var_a1 = arg0->unk6 + 1;
        break;
    case 3:
        if (arg0->unk8 >= 0x1F) {
            var_a1 = 0x29;
            var_s5 = arg0->unk6 / 2;
        } else {
            var_a1 = arg0->unk6 / 2 + 9;
        }
        break;
    case 4:
        var_a1 = arg0->unk6 / 2 + 0x1D;
        break;
    case 5:
        var_a1 = arg0->unk6 / 2 + 0x21;
        break;
    case 6:
        var_a1 = 0x29;
        var_s5 = arg0->unk6 / 2;
        break;
    case 7:
        if (arg0->unk8 >= 0x1F) {
            var_a1 = 0x29;
            var_s5 = arg0->unk6 / 2;
        } else {
            var_a1 = arg0->unk6 / 2 + 0x2A;
        }
        break;
    case 8:
        var_a1 = 0x32;
        var_s5 = arg0->unk6 / 2;
        break;
    case 9:
        if (arg0->unk8 >= 0x1F) {
            var_a1 = 0x32;
            var_s5 = arg0->unk6 / 2;
        } else {
            var_a1 = arg0->unk6 / 2 + 0x33;
        }
        break;
    default:
        return;
    }
    temp_s1 = (ObjectState *) ((u8 *) arg0 + 0x1C);
    if (arg0->unk20 != var_a1) {
        SelectSpriteAnimation(temp_s1, var_a1 & 0xFF);
    }
    if (var_s5 != -1) {
        SelectSpriteAnimationFrame(temp_s1, var_s5 & 0xFF);
    }
    temp_s3 = ovl_23_func_800BB040(arg0->unk14);
    temp_s4 = (s16) (arg0->unk18 / 0x1000);
    SetSpriteSourceTextureOffsets((Struct_800154CC *) temp_s1, 0, 0, arg0->unk4C, arg0->unk4E);
    temp_s0 = ovl_23_func_800BAFFC(arg0->unk18);
    if (var_s5 == -1) {
        DrawSpriteAnimationQuadsAndUpdate(temp_s0, (SpriteSourceData *) temp_s1, temp_s3, temp_s4);
    } else {
        DrawSpriteFrameRects(temp_s0, (s32) temp_s1, arg0->unk20, arg0->unk21, temp_s3, temp_s4);
    }
    func_800248E8(temp_s0, temp_s3, temp_s4, 0);
    if ((((Ovl23D87CViewBAA60 *) Ovl23_D_800BF87C_draw)->unk4 != 1) &&
        (((Ovl23D87CViewBAA60 *) Ovl23_D_800BF87C_draw)->unk14 ==
         (((((Ovl23D87CViewBAA60 *) Ovl23_D_800BF87C_draw)->unk14 + 1) / 30) * 30) - 1)) {
        ovl_23_func_800B94D0(arg0->unk14, temp_s4, arg0->unk6);
    }
}
