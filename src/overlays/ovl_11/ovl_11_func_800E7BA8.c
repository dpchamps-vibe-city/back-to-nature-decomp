#include "common.h"
#include "screen_primitives.h"

s32 ovl_11_func_800E7BA8(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_t1;

    var_t1 = 0;
    if (arg0 != 0) {
        var_t1 = 5;
    }
    DrawScreenFlatRect((s32 *)(gActiveRenderContext->field_D8 + var_t1 * 4), 0, 0, 0x140, 0xF0,
                  (arg3 & 0xFF) | ((arg2 & 0xFFFF) << 8) |
                      ((arg1 & 0xFF) << 0x10),
                  1);
    return 1;
}
