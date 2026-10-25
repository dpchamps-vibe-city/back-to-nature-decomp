#include "common.h"
#include "message_window.h"

s32 ovl_11_func_8010876C(void);

s32 ovl_11_func_80108738(void) {
    s32 callRet2;
    s32 callRet4;
    callRet2 = ovl_11_func_8010876C();
    CloseMessageWindow();
    callRet4 = RequestMessageWindow(3, callRet2);
    return callRet4;
}
