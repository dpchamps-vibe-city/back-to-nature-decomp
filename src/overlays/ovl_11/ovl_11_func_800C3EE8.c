#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
void ovl_11_func_800C32C8(Ovl11AnimationObject *obj, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800C3EE8(Ovl11AnimationObject *arg0, u16 *arg1, s32 arg2) {
    u16 temp_v1;
    s32 result;

    temp_v1 = arg0->flags;
    if (!(temp_v1 & 0x100)) {
        result = 1;
    } else if (!(temp_v1 & 2)) {
        ovl_11_func_80107C80(*(s16 *) ((u8 *) arg1 + 2), (u16) arg0->field_6, NULL, 1);
        arg0->flags |= 3;
        ovl_11_func_800C32C8(arg0, 5, arg0->field_6 + 1, -2);
        result = 1;
    } else {
        result = 0;
    }
    return result;
}
