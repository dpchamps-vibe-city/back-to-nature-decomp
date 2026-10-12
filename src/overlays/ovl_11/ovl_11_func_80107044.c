#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/stdlib.h"


void ovl_11_func_800DCC1C (s16 arg0, void *arg1);
s32 func_8001FABC (s16 arg0);
s32 func_800212A8 (s32 soundId, s32 lo, s32 hi);

typedef struct {
    char pad0[0x10];
    s32 flags;
    s32 unk14;
    s16 linkedSprite;
    char pad1A[0x2C - 0x1A];
} RotationSpriteView;

typedef struct {
    char pad0[0x938];
    RotationSpriteView sprites[50];
} RotationPoolView;

void ovl_11_func_80107044(Ovl11RotationState *arg0) {
    Ovl11RotationVector mirrored;
    s16 temp_s2;
    s16 var_s3;
    s32 temp_s4;
    s32 temp_s5;
    s32 var_v1;
    s32 angle;
    s32 direction;
    u16 component;
    u16 state;
    s32 opening;
    RotationPoolView *temp_v1;

    temp_s2 = arg0->unk14->sprite;
    temp_v1 = (RotationPoolView *)((struct Ovl11SpriteWork *)&D_8007AFF0)->header;
    temp_s4 = temp_v1->sprites[temp_s2].flags & 0x400;
    direction = arg0->unk2 - 1;
    temp_s5 = (u32) direction < 2U;
    if (temp_s4 != 0) {
        var_s3 = temp_v1->sprites[temp_s2].linkedSprite;
    } else {
        var_s3 = -1;
    }
    state = arg0->unk0;
    if (state == 2) {
        arg0->unk8 += 8;
    }
    if ((u16) arg0->unk0 == 4) {
        arg0->unk8 = (s32) (arg0->unk8 - 8);
    }
    if (temp_s5 != 0) {
        arg0->rotation.y -= arg0->unk8;
    } else {
        arg0->rotation.y += arg0->unk8;
    }
    ovl_11_func_800DCC1C(temp_s2, &arg0->rotation.x);
    if (temp_s4 != 0) {
        s32 y;

        component = (u16) arg0->rotation.x;
        mirrored.x = component;
        y = arg0->rotation.y;
        var_v1 = -y;
        mirrored.y = var_v1;
        component = (u16) arg0->rotation.z;
        mirrored.z = component;
        ovl_11_func_800DCC1C(var_s3, &mirrored);
    }
    if (arg0->unk0 == 2) {
        angle = abs(arg0->rotation.y);
        if (angle >= 0x420) {
            arg0->unk4 = 0xB;
            arg0->unk0 = 3;
            arg0->unk8 = 0;
            arg0->rotation.y = (u16) D_801274DC[arg0->unk2];
            return;
        }
    }
    if (arg0->unk0 == 4) {
        if (temp_s5 != 0) {
            if (arg0->rotation.y < 0) {
                return;
            }
        } else if (arg0->rotation.y > 0) {
            return;
        }
        func_8001FABC(0x4C);
        arg0->unk0 = 1U;
        arg0->unk8 = 0;
        arg0->rotation.y = 0U;
        ovl_11_func_800DCC1C(temp_s2, &arg0->rotation.x);
        if (temp_s4 != 0) {
            ovl_11_func_800DCC1C(var_s3, &arg0->rotation.x);
        }
    }
}
