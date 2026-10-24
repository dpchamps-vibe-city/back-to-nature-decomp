#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void *func_800154CC(Struct_800154CC *arg0, s32 *arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7);

void *func_80015644(Struct_800154CC *arg0, s32 *arg1, s32 arg2, s32 arg3, s16 arg4,
                    s16 arg5, s32 arg6, s16 arg7, u8 arg8) {
    arg0 = func_800154CC(arg0, arg1, (s16)arg2, (s16)arg3, arg4, arg5, arg6, arg7);
    setDrawTPage(arg0, 0, 1, (arg8 & 3) << 5);

    addPrim(arg1, arg0);

    arg0 = (Struct_800154CC *)((char *)arg0 + 8);
    return arg0;
}
