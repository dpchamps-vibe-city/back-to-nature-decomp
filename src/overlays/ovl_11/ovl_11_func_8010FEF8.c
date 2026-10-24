#include "common.h"
#include "game_types.h"

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_80110544(Ovl11E644Object *arg0, s16 arg1);
s32 ovl_11_func_800F413C(Ovl11F413CArg0View *arg0, Ovl11F413CArg1View *arg1);
s32 ovl_11_func_80110520(s32 arg0, s16 arg1);
s32 ovl_11_func_80110188(Ovl11StateArg0 *arg0, s16 arg1, s32 *arg2, s16 arg3);
u16 ovl_11_func_80110118(Ovl11ActionCompletionView *arg0);
s32 ovl_11_func_800F44B4(u16 arg0, s32 *arg1, s16 *arg2, s32 arg3);
s32 ovl_11_func_80110944(s16 arg0, u16 arg1);

s32 ovl_11_func_8010FEF8(Ovl11SetFieldsView *arg0, s32 arg1) {
    Ovl11PaddedVec3 sp18;
    s16 sp28;
    s16 temp_v0;
    s32 temp_v0_2;
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    switch (temp_v1) {
    case 0:
        temp_v0 = ovl_11_func_80110944(arg0->field_30, 8);
        (*(s16 *) ((u8 *) arg0 + 0xBA)) = temp_v0;
        if (temp_v0 == -1) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
            return 1;
        }
        temp_v0_2 = ovl_11_func_800F44B4(0x123, &sp18.field_0, &sp28, temp_v0);
        if ((temp_v0_2 == 1) && (sp28 == arg0->field_30)) {
            ovl_11_func_800D05D0(arg0, sp18);
            (*(s16 *) ((u8 *) arg0 + 0x28)) = temp_v0_2;
        }
        break;
    case 1:
        if (ovl_11_func_80110544((Ovl11E644Object *) arg0, 0xE6) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 2;
        } else if (!(*(u16 *) (((struct Ovl11FEF8State *) D_8006C838)->records + (*(s16 *) ((u8 *) arg0 + 0xBA)) * 0x18 + 4) & 1)) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    case 2: {
        Ovl11F413CArg1View sp30;

        if (ovl_11_func_800F413C((Ovl11F413CArg0View *) (((struct Ovl11FEF8State *) D_8006C838)->records + (*(s16 *) ((u8 *) arg0 + 0xBA)) * 0x18), &sp30) == 1) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 3;
            (*(u16 *) ((u8 *) arg0 + 0xB6)) = sp30.unk0;
        } else {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    }
    case 3:
        if (ovl_11_func_80110520((s32) arg0, *(s16 *) ((u8 *) arg0 + 0xB6)) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 4;
        }
        break;
    case 4:
        if (ovl_11_func_80110188((Ovl11StateArg0 *) arg0, 2, &sp18.field_0, arg0->field_30) == 1) {
            ovl_11_func_800D05D0(arg0, sp18);
        }
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 5;
        break;
    case 5:
        if (ovl_11_func_80110544((Ovl11E644Object *) arg0, 0x258) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 6;
        }
        break;
    case 6:
        if (ovl_11_func_80110118((Ovl11ActionCompletionView *) arg0) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    }
    return 0;
}
