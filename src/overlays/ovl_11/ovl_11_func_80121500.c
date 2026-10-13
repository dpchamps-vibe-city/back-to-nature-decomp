#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D5868(s16 arg0);
s32 ovl_11_func_800D61D8(s16 arg0);

#define EFFECT_STATE (((struct struct_8006C838_button *)D_8006C838)->field_44BA)
#define EFFECT_AREA (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476)

void ovl_11_func_80121500(Ovl11SpriteEffect *arg0, Ovl11EffectCoordinate arg1, Ovl11EffectCoordinate arg2, Ovl11EffectCoordinate arg3, s32 arg4, u16 arg5) {
    s32 var_a3;
    s32 temp_v0;
    s32 var_s1;
    s32 var_t0;
    s32 var_v1;

    var_t0 = 0;
    var_s1 = 0;
    if (EFFECT_STATE == 3) {
        var_t0 = (EFFECT_AREA != 1) * 2;
    }
    if (EFFECT_AREA != 1) {
        var_s1 = -1;
    }
    if (EFFECT_AREA == 6) {
        var_s1 = 0x16;
    }
    switch (arg5) {
    case 0x3A:
        var_v1 = 0;
        var_a3 = var_t0 + 1;
        break;
    case 0x3B:
    case 0x16C:
    case 0x16D:
    case 0x16E:
        var_v1 = 1;
        var_a3 = var_t0 + 1;
        break;
    case 0x3C:
    case 0x16F:
    case 0x170:
    case 0x171:
        var_v1 = 2;
        var_a3 = var_t0 + 1;
        break;
    case 0x3D:
    case 0x172:
    case 0x173:
    case 0x174:
        var_v1 = 3;
        var_a3 = var_t0 + 1;
        break;
    case 0x3E:
        var_v1 = 4;
        var_a3 = var_t0 + 1;
        break;
    case 0x3F:
    case 0x175:
        var_v1 = 5;
        var_a3 = 1;
        break;
    case 0x36:
        var_a3 = 1;
        switch (EFFECT_STATE) {
        case 0:
            var_v1 = 14;
            break;
        case 1:
        case 2:
            var_v1 = 19;
            break;
        default:
            return;
        }
        break;
    case 0x40:
        var_a3 = 3;
        if (EFFECT_STATE != 2) {
            var_a3 = 1;
        }
        var_v1 = var_s1 + 6;
        break;
    default:
        if (ovl_11_func_800D5868((s16)arg5) == 1 &&
            (temp_v0 = ovl_11_func_800D61D8((s16)arg5), var_a3 = 1, EFFECT_STATE != temp_v0) &&
            EFFECT_AREA != 6) {
            var_a3 = 1;
            var_v1 = var_s1 + 6;
            if (temp_v0 != 4) {
                var_a3 = 3;
            }
        } else {
            var_a3 = 1;
            var_v1 = var_s1 + 6;
        }
        break;
    }
    arg0->unk14 = &D_80070400 + var_v1;
    arg0->unk0 = var_a3;
    arg0->unk2 = 0;
    arg0->unk4 = *(s32 *)&arg1;
    arg0->unk8 = *(s32 *)&arg2;
    arg0->unkC = *(s32 *)&arg3;
}
