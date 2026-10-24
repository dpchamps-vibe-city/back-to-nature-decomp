#include "common.h"
#include "psyq/libsnd.h"

s32 func_80020900(s16 arg0) {
    s32 callRet2;
    callRet2 = SsUtSetReverbType(arg0);
    SsUtSetReverbDepth(0, 0);
    return callRet2;
}
