#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800CAE64(Ovl11Func800CADBCArg0 *arg0, s32 arg1, Ovl11Func800CADBCArg0 *arg2, s32 arg3);

extern Ovl11Func800CADBCArg0 D_80071AC0;

s32 ovl_11_func_800C9DE8(s32 arg0) {
    if (arg0 == 0) {
        ovl_11_func_800CAE64(&D_80071AC0, 8, (&D_80071AC0 - 9), 0);
    } else {
        ovl_11_func_800CAE64(&D_80071AC0, 8, (&D_80071AC0 - 9), 1);
    }
    return 1;
}
