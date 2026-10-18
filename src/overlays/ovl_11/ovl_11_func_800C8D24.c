#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C8CC8(M2C_15512f31231d_Func800C7878Arg0 *arg0);
s32 ovl_11_func_800CB850(s32 arg0);

s32 ovl_11_func_800C8D24(M2C_15512f31231d_Func800C7878Arg0 *arg0, u16 arg1) {
    if (arg1 & 0x40) {
        if (!(((Ovl11C7010Object *) arg0)->unk6C & 0x18000000)) {
            if ((D_80128C54 & 0x2000) && ovl_11_func_800C8CC8(arg0) != 0) {
                return 1;
            }
        } else if (ovl_11_func_800CB850(0) != 0) {
            return 1;
        }
    }
    return 0;
}
