#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "random.h"

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_800D425C(u16 *arg0);
s32 ovl_11_func_800D3D60(u16 arg0);
s32 ovl_11_func_800D3104(u16 *arg0, s32 arg1);

s32 ovl_11_func_800D4EA0(Ovl11SetFieldsView *arg0) {
    Ovl11PaddedVec3 position;
    s32 result;
    s32 slot;
    s16 state;
    u8 *far_base;

    result = 0;
    state = *(s16 *)((u8 *)arg0 + 0x28);
    switch (state) {
    case 0:
        ((Ovl11ProgressState *)D_8006C838)->flags |= 0x1000000;
        *(s16 *)((u8 *)arg0 + 0x28) = 1;
        *(s16 *)((u8 *)arg0 + 0x22) = 2;
        arg0->field_34 |= 0x800;
        position.field_0 = -0x1324;
        position.field_4 = 0;
        position.field_8 = -0x2198;
        ovl_11_func_800D05D0(arg0, position);
        *(s32 *)((u8 *)arg0 + 0x38) = -0x12C0;
        *(s32 *)((u8 *)arg0 + 0x3C) = -0x12C;
        *(s32 *)((u8 *)arg0 + 0x40) = -0x2EE0;
        arg0->field_30 = 1;
        far_base = (u8 *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == 1) {
            PlaySoundEffect(0x1C);
        }
        break;
    case 1:
        *(s16 *)((u8 *)arg0 + 0x22) = 2;
        ovl_11_func_800D425C((u16 *)arg0);
        if (!(arg0->field_34 & 0x2000)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 2;
        }
        *(s16 *)((u8 *)arg0 + 0x22) = 2;
        break;
    case 2:
        if (Rand(2) != 0) {
            *(s16 *)((u8 *)arg0 + 0x22) = 2;
        } else {
            *(s16 *)((u8 *)arg0 + 0x22) = 3;
        }
        result = 1;
        *(s16 *)((u8 *)arg0 + 0x16) = 0;
        arg0->field_34 &= ~0x800;
        slot = ovl_11_func_800D3D60(0);
        if (slot < 0) {
            slot = ovl_11_func_800D3D60(1);
            *(s16 *)((u8 *)arg0 + 0x16) = 1;
            if (slot < 0) {
                ovl_11_func_800D3104((u16 *)arg0, 0x13);
                break;
            }
        }
        *(u16 *)((u8 *)arg0 + 2) = slot;
        ovl_11_func_800D3104((u16 *)arg0, 0x14);
        break;
    }
    return result;
}
