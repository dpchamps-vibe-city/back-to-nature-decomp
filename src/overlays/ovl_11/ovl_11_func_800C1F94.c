#include "common.h"
#include "game_types.h"

void ovl_11_func_800C32C8(Ovl11AnimationObject *obj, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800C1F94(Ovl11AnimationObject *arg0, u16 *arg1) {
    u16 temp_v1;

    temp_v1 = *arg1;
    switch (temp_v1) {
    case 1:
    case 16:
        if (*(s16 *) ((u8 *) arg0 + 0x10) != 0) {
            ovl_11_func_800C32C8(arg0, 2, 0, -2);
            return;
        }
        ovl_11_func_800C32C8(arg0, 2, arg0->field_6 + 1, -2);
        return;
    case 0:
        *(s32 *) ((u8 *) arg0 + 0x50) = 0;
        *(s32 *) ((u8 *) arg0 + 0x54) = 0;
        *(s32 *) ((u8 *) arg0 + 0x58) = 0;
        ovl_11_func_800C32C8(arg0, 1, 0, -2);
        return;
    }
}
