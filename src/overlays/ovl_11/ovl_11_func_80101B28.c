#include "common.h"
extern u16 gOvl11SelectionWorkFlags;

s32 ovl_11_func_80101B84(s8 arg0);

s32 ovl_11_func_80101B28(s8 arg0) {
    s32 callRet1;
    if (arg0 < 7) {
        callRet1 = ovl_11_func_80101B84(arg0);
        return ((u32)0) < ((u32)(gOvl11SelectionWorkFlags & callRet1));
    } else {
        return ((u32)0) < ((u32)(gOvl11SelectionWorkFlags & 0xF80));
    }
}
