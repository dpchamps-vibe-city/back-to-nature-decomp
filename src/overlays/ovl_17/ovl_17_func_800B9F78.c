#include "common.h"
#include "random.h"

s32 ovl_17_func_800B9F78(s16 arg0) {
    s32 callRet2;
    callRet2 = Rand(0x7F);
    return arg0 < callRet2;
}
