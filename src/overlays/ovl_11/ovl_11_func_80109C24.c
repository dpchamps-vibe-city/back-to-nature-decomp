#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80109974(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 position, s16 arg5, s32 arg6);
void func_8001FABC(s16 arg0);

s32 ovl_11_func_80109C24(Ovl11SetFieldsView *arg0) {
    s32 result;
    char *far_base;

    if (*(u16 *) arg0 == 0) {
        return -1;
    }
    if (!(arg0->field_34 & 0x8000) ||
        (u32) (D_80070CF8_halfword - 6) >= 0xF) {
        return -1;
    }
    result = -1;
    if (ovl_11_func_80109974(arg0,
            *(Ovl11PaddedVec3 *) ((u8 *) arg0 + 0x38),
            *(s16 *) ((u8 *) arg0 + 0x30), 0x7D0) == 1) {
        far_base = (char *) &D_8007AFF0;
        if (*(s16 *) (far_base + 0x25476) ==
            *(s16 *) ((u8 *) arg0 + 0x30)) {
            func_8001FABC(0x19);
        }
        arg0->field_34 |= 0x20;
        result = 0;
    }
    return result;
}
