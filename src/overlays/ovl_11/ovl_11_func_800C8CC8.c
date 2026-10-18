#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C7878(M2C_15512f31231d_Func800C7878Arg0 *arg0, M2C_15512f31231d_F413CArg1 *arg1);
s32 ovl_11_func_800CB540(u16 *arg0);

s32 ovl_11_func_800C8CC8(M2C_15512f31231d_Func800C7878Arg0 *arg0) {
    s16 item[3];

    item[0] = 0;
    ovl_11_func_800C7878(arg0, (M2C_15512f31231d_F413CArg1 *) item);
    if (item[0] != 0) {
        ovl_11_func_800CB540((u16 *) item);
        ((Ovl11C7010Object *) arg0)->unk6C &= ~0x200000;
        return 1;
    }
    return 0;
}
