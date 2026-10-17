#include "common.h"
#include "game_types.h"

void func_80015868 (Struct_800154CC *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_80015840 (ObjectState *obj, s8 arg1);
void func_8001585C (ObjectState *obj, u8 arg1);
void ovl_19_func_800BADF8 (s32 arg0, SpriteSourceData *arg1, s16 *arg2, s16 arg3);

void ovl_19_func_800BAB4C(Ovl19Func800BAB4CArg *arg0) {
    ObjectState *temp_s0;
    s16 temp_v1;
    s32 var_a1;
    s32 var_s2;

    if (arg0->unk0 == 1) {
        func_80015868((Struct_800154CC *) &arg0->sprite, 0, 0, 0, arg0->unk6);
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
        func_80015840(temp_s0, var_a1);
    }
    if (var_s2 != -1) {
        func_8001585C(temp_s0, var_s2 & 0xFF);
    }
    ovl_19_func_800BADF8(D_8005E3C0->field_120, (SpriteSourceData *) temp_s0, arg0->position, 0);
}
