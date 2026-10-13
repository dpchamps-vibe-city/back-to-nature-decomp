#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C3138(u8 *arg0);

s32 ovl_11_func_800C7838(u8 *arg0) {
    Ovl11AnimationObject *temp_v0;

    temp_v0 = (Ovl11AnimationObject *)ovl_11_func_800C3138(arg0);
    if (temp_v0 == 0) {
        return 0;
    } else {
        return temp_v0->field_1A != 2;
    }
}
