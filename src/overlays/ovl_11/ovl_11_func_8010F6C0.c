#include "common.h"
#include "game_types.h"

s32 ovl_11_func_801105C8 (Ovl11SetFieldsView *arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_80110544 (Ovl11E644Object *arg0, s16 arg1);
s32 ovl_11_func_8011031C (Ovl11ActionProbeObject *arg0);
s32 ovl_11_func_80110520 (s32 arg0, s16 arg1);
s32 ovl_11_func_80110188 (Ovl11StateArg0 *arg0, s16 arg1, s32 *arg2, s16 arg3);
void ovl_11_func_800D05D0 (Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
u16 ovl_11_func_80110118 (Ovl11ActionCompletionView *arg0);

s32 ovl_11_func_8010F6C0(Ovl11SetFieldsView *arg0, s32 arg1) {
    Ovl11PaddedVec3 sp18;
    s32 temp_v0;
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    switch (temp_v1) {
    case 0:
        if (ovl_11_func_801105C8(arg0, arg1, 1) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
            return 1;
        }
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 1;
        break;
    case 1:
        if (ovl_11_func_80110544((Ovl11E644Object *) arg0, 0xE6) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 2;
        }
        break;
    case 2:
        temp_v0 = ovl_11_func_8011031C((Ovl11ActionProbeObject *) arg0);
        (*(s16 *) ((u8 *) arg0 + 0xB6)) = temp_v0;
        if (temp_v0 & 0xFFFF) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 3;
        } else {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    case 3:
        if (ovl_11_func_80110520((s32) arg0, *(s16 *) ((u8 *) arg0 + 0xB6)) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 4;
        }
        break;
    case 4:
        if (ovl_11_func_80110188((Ovl11StateArg0 *) arg0, 2, (s32 *) &sp18, arg0->field_30) == 1) {
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
