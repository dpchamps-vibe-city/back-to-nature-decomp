#include "common.h"

s16 *ovl_11_func_800F82FC(s32 arg0, s32 arg1);
s32 ovl_11_func_800F7E38(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);
s32 ovl_11_func_800F7FAC(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);
s32 ovl_11_func_800F8188(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);

s32 ovl_11_func_800F7D34(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 result;
    s16 *second;
    s16 *first;

    if (arg0 != arg1 || arg2 != arg3) {
        first = ovl_11_func_800F82FC(arg0, arg2);
        second = ovl_11_func_800F82FC(arg1, arg3);
        if (*first == 0 && *second == 0) {
            return 1;
        }
        if (*second == 0) {
            return ovl_11_func_800F7E38((u16 *)first, arg0, (u16 *)second, arg1);
        }
        if (*first == 0) {
            return ovl_11_func_800F7E38((u16 *)second, arg1, (u16 *)first, arg0);
        }
        if (*first != *second) {
            return ovl_11_func_800F7FAC((u16 *)first, arg0, (u16 *)second, arg1);
        }
        result = ovl_11_func_800F8188((u16 *)first, arg0, (u16 *)second, arg1);
        return result;
    }
    return 0;
}
