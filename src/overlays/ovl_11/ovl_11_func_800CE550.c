#include "common.h"
#include "game_types.h"

/* Overlay name for the matched SetVal8005E51C setter. */
void SetVal8005E51C(s32 arg0);
s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);

s32 ovl_11_func_800CE550(Vec3 *arg0) {
    Vec3 position;
    s32 result;
    u32 i;

    result = 1;
    i = 0;
    position = *arg0;
    for (; i < 5; i++) {
        SetVal8005E51C(D_801232C8[i]);
        if (ovl_11_func_800BDB88(&position.x, 0) == 0) {
            arg0->y = position.y;
            result = 0;
            break;
        }
    }
    SetVal8005E51C(0xC8);
    return result;
}
