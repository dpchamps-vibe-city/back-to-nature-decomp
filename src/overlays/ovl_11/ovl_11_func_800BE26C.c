#include "common.h"

#include "game_types.h"

s32 ovl_11_func_800BEB28(s32 *arg0, Ovl11CollisionTag arg1, void **arg2,
                       s32 *arg3, s32 arg4, s32 arg5);

s32 ovl_11_func_800BE26C(s32 *arg0, Ovl11CollisionTag arg1, s32 arg2,
                       s32 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7) {
    D_80128810 += 1;
    return ovl_11_func_800BEB28(arg0, arg1, arg4, arg5, arg6, arg7);
}
