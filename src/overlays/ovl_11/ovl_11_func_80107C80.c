#include "common.h"
#include "psyq/stddef.h"

s32 ovl_11_func_80107390(s32 arg0);
s32 func_8001AF44(u32 arg0);
u16 ovl_11_func_8010734C(s32 id);
s32 ovl_11_func_80106F20(s16 arg0, u16 arg1);

s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3) {
    s16 *var_v1;
    s16 var_a1;
    s16 var_a2;
    s32 var_a0;
    s32 var_s2;

    var_s2 = -1;
    var_a1 = -1;
    var_a2 = 0;
    var_v1 = (s16 *) D_80070C72;
    for (var_a0 = 0; var_a0 < 5; var_a0++) {
        if ((var_v1[0] == arg0) && (arg0 != -1)) {
            var_a1 = arg0;
            var_a2 = var_v1[1];
        }
        var_v1 += 2;
    }
    if (var_a1 == -1) {
        if (arg3 == 0) {
            var_s2 = ovl_11_func_80107390((s32) arg0);
        }
        if ((arg0 == 0x20) && (func_8001AF44(0x76U) == 1)) {
            *arg2 = 0x209;
            goto block_21;
        }
        if (D_8006C848 & 0x04000000) {
            var_s2 = -1;
        }
        if (var_s2 != -1) {
            if (arg2 != NULL) {
                *arg2 = var_s2;
                if (ovl_11_func_8010734C((s32) arg0) != 1) {
                    *arg2 = -1;
                    return 0;
                }
            }
            return 0;
        }
        goto block_18;
    }
    if (arg3 != 0) {
block_18:
        return ovl_11_func_80106F20(arg0, arg1 & 0xFFFF);
    }
    if (arg2 != NULL) {
        *arg2 = (s32) var_a2;
block_21:
        ;
    }
    return 0;
}
