#include "common.h"
#include "message_window.h"

extern s16 D_800BFE46;

void ovl_25_func_800BA818(void) {
    s32 var_v1;

    RequestMessageWindow(4, 0x31);
    var_v1 = GetMessageWindowStatus();
    if (var_v1 == 2) {
        D_800BFE46 = var_v1;
    }
}
