#include "common.h"

void ovl_11_func_8011D0C8(s32 arg0, s16 arg1, s16 arg2, s16 arg3);
s32 ovl_11_func_8011D264(s16 *arg0);

s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2) {
    if (arg1 == -1) {
        D_80128294 = 0;
        return 0;
    }
    if (arg1 == -2) {
        D_80128294 = 1;
        return 0;
    }
    ovl_11_func_8011D0C8(arg0, arg1, arg2, D_80128294);
    if (ovl_11_func_8011D264(&D_80128294) == 1) {
        if (D_80128294 == 0) {
            D_80128294 = 0;
            return 1;
        }
        D_80128294 = 0;
        return 2;
    }
    return 0;
}
