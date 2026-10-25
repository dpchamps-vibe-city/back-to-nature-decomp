#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_scene_camera_geometry.h"
#include "sprite_source.h"
#include "sprite_render.h"


void ovl_19_func_800BADF8 (s32 arg0, SpriteSourceData *arg1, s16 *arg2, s16 arg3);
void ovl_19_func_800BAB4C (Ovl19Func800BAB4CArg *arg0);
void ovl_19_func_800BAFAC (s32 arg0, ObjectState *arg1, s16 *arg2, s16 arg3);
s32 func_8001DFD4 (s32 *arg0, SVECTOR *arg1);
void func_800248E8 (s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void ovl_19_func_800BA834(s16 *arg0) {
    SVECTOR sp18;
    s16 *var_a1_2;
    s16 *var_s0;
    s16 *var_s2;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_4;
    s16 temp_v1_5;
    s16 var_a1;
    s16 var_s3;
    s16 var_s5;
    s16 var_v0;
    s16 var_v0_2;
    s16 var_s4;
    u16 temp_v1_3;

    var_s2 = gOvl19ActorPairState;
    if (arg0 == gOvl19ActorPairState) {
        var_s2 = arg0 + 0x24;
    }
    var_s3 = 0;
    temp_v1 = *arg0;
    var_s4 = 0;
    if (temp_v1 == 0) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) (arg0 + 0xC), 0, 0, 0x20, 1);
        var_a1 = 0;
    } else {
        if (temp_v1 == 1) {
            SetSpriteSourceTextureOffsets((Struct_800154CC *) (arg0 + 0xC), 0, 0, *(s16 *) ((u8 *) arg0 + 0xA), 0);
        }
        var_a1 = 0;
    }
    temp_v1_2 = *(s16 *) ((u8 *) arg0 + 2);
    var_s5 = -1;
    switch (temp_v1_2) {
    case 0:
        var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 5;
        break;
    case 1:
        var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 9;
        break;
    case 2:
        var_s5 = arg0[2];
        var_a1 = 0;
        break;
    case 4:
        var_s3 = -0xA;
        var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 0x11;
        break;
    case 6:
        if ((gOvl19StepStartTicks - 0x1E) < arg0[3]) {
            var_s3 = -0xA;
            var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 0x11;
        } else {
            var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 1;
        }
        break;
    case 7:
    case 8:
        if ((gOvl19StepStartTicks - 0x1E) >= arg0[3]) {
            temp_v1_3 = *(u16 *) ((u8 *) var_s2 + 4);
            var_a1 = temp_v1_3 + 1;
            arg0[2] = temp_v1_3;
        } else {
            var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 0xD;
        }
        var_s4 = 1;
        break;
    case 9:
        temp_v1_4 = arg0[3];
        if (temp_v1_4 >= (gOvl19DoubleThresholdTicks - 0x1E)) {
            var_s3 = -0xA;
            var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 0x11;
        } else if (temp_v1_4 >= (gOvl19DoubleThresholdTicks - 0x3C)) {
            var_s3 = -0xA;
            var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 0x11;
        } else {
            var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 1;
        }
        break;
    case 10:
        temp_v1_5 = arg0[3];
        if (temp_v1_5 >= (D_800BF652 - 0x1E)) {
            var_s3 = -0xA;
            var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 0x11;
        } else {
            if (temp_v1_5 >= (D_800BF652 - 0x2D)) {
                var_s4 = 1;
                var_a1 = (*(u16 *) ((u8 *) var_s2 + 4)) + 0xD;
            } else {
                var_s4 = 1;
                var_a1 = (*(u16 *) ((u8 *) var_s2 + 4)) + 1;
            }
            arg0[2] = var_s2[2];
        }
        break;
    case 3:
    case 5:
    case 11:
        var_a1 = (*(u16 *) ((u8 *) arg0 + 4)) + 1;
        break;
    }
    var_s0 = arg0 + 0xC;
    if ((*(u8 *) ((u8 *) arg0 + 0x1C)) != var_a1) {
        SelectSpriteAnimation((ObjectState *) var_s0, var_a1 & 0xFF);
    }
    if (var_s5 != -1) {
        SelectSpriteAnimationFrame((ObjectState *) var_s0, var_s5 & 0xFF);
    }
    ovl_19_func_800BADF8(gActiveRenderContext->field_120, (SpriteSourceData *) var_s0, (arg0 + 8), var_s3);
    if (var_s4 == 1) {
        var_a1_2 = gOvl19SceneWork;
        if ((*(u8 *) ((u8 *) var_a1_2 + 0x1D4)) != 0x24) {
            SelectSpriteAnimation((ObjectState *) (var_a1_2 + 0xE8), 0x24);
        }
        sp18 = *(SVECTOR *) (arg0 + 8);
        ovl_19_func_800BADF8(gActiveRenderContext->field_D8 + 8, (SpriteSourceData *) (var_a1_2 + 0xE8), (s16 *) &sp18, -0x18);
    }
}
