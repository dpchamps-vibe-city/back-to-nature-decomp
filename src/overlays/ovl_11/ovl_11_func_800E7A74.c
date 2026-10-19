#include "common.h"
#include "psyq/stddef.h"

s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
u16 ovl_11_func_8010734C(s32 id);

s32 ovl_11_func_800E7A74(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 var_s0;
    s32 var_s1;

    if (arg3 != 0) {
        var_s0 = *(s16 *)&D_80129560[arg0];
        var_s1 = 1;
    } else {
        var_s0 = arg0;
        var_s1 = 1;
    }
    switch (arg1) {
    case 0:
        ovl_11_func_80107C80(var_s0, arg2 & 0xFFFF, NULL, 1);
        break;
    case 1:
        var_s1 = ovl_11_func_8010734C(var_s0) == 1;
        break;
    case 2:
        var_s1 = ovl_11_func_8010734C(var_s0) == 3;
        break;
    case 3:
        var_s1 = ovl_11_func_8010734C(var_s0) == 2;
        break;
    case 4:
        var_s1 = 0;
        if (ovl_11_func_8010734C(var_s0) != 0) {
            var_s1 = ovl_11_func_8010734C(var_s0) != 1;
        }
        break;
    }
    return var_s1;
}
