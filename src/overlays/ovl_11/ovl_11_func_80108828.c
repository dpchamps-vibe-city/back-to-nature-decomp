#include "common.h"
#include "message_window.h"

extern s32 D_8012D040;

s32 ovl_11_func_80108828(void) {
    return RequestMessageWindow(3, D_8012D050[D_8012D040].field_0);
}
