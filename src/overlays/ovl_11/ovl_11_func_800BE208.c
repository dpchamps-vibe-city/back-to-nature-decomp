#include "common.h"

#include "game_types.h"

/* Forward the two-halfword collision tag by value, as in 800BE26C. */
s32 ovl_11_func_800BEB28(s32 *arg0, Ovl11CollisionTag arg1, void **arg2,
                       s32 *arg3, s32 arg4, s32 arg5);

s32 ovl_11_func_800BE208(s32 *arg0, Ovl11CollisionTag arg1, u16 arg2,
                       u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7) {
    s32 result;

    if (D_80128810 >= 0x32) {
        *arg5 = 0;
        result = 1;
    } else {
        D_80128810 += 1;
        result = ovl_11_func_800BEB28(arg0, arg1, arg4, arg5, arg6, arg7);
    }
    return result;
}
