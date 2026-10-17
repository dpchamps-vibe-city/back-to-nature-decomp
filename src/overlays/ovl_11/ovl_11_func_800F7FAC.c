#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F8404(s32 arg0);
s32 ovl_11_func_800D5810(s16 arg0);
s32 ovl_11_func_800D57A4(s16 arg0);
s8 ovl_11_func_800D583C(s16 arg0);
void ovl_11_func_800F9174(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3, s32 arg4);
void ovl_11_func_800F8480(Ovl11CopyRecord *arg0, Ovl11CopyRecord *arg1);

s32 ovl_11_func_800F7FAC(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    switch (ovl_11_func_800F8404(arg3)) {
    case 2:
        if (ovl_11_func_800D5810((s16) arg0[0]) == 0) {
            return 0;
        }
        if (D_80126E40 != ovl_11_func_800D57A4((s16) arg0[0])) {
            return 0;
        }
        break;
    case 1:
        if (ovl_11_func_800D5810((s16) arg0[0]) == 0) {
            return 0;
        }
    case 0:
        if (ovl_11_func_800D583C((s16) arg0[0]) == 0 && ((s16 *) arg0)[2] >= 2) {
            return 0;
        }
        break;
    }
    switch (ovl_11_func_800F8404(arg1)) {
    case 2:
        if (ovl_11_func_800D5810((s16) arg2[0]) == 0) {
            return 0;
        }
        if (D_80126E40 != ovl_11_func_800D57A4((s16) arg2[0])) {
            return 0;
        }
        ovl_11_func_800F9174(arg0, arg1, arg2, arg3, 1);
        ovl_11_func_800F8480((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
        return 1;
    case 1:
        if (ovl_11_func_800D5810((s16) arg2[0]) == 0) {
            return 0;
        }
    case 0:
        if (ovl_11_func_800D583C((s16) arg2[0]) != 0 || ((s16 *) arg2)[2] < 2) {
            ovl_11_func_800F9174(arg0, arg1, arg2, arg3, 1);
            ovl_11_func_800F8480((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
            return 1;
        }
        break;
    }
    return 0;
}
