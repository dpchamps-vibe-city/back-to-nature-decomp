#include "common.h"
#include "message_window.h"

s32 ovl_11_func_8011C298(s16 arg0, s16 arg1, s16 arg2, s16 arg3);

s32 ovl_11_func_80119470(void) {
    if (((struct Ovl11CallbackGateState *)D_8006C838)->flags & 0x20) {
        if (((struct Ovl11CallbackGateState *)D_8006C838)->status & 0x800) {
            RequestMessageWindow(2, 0xD6);
            return 1;
        }
        return ovl_11_func_8011C298(0x64, 2, 0xD5, 2);
    }
    return 1;
}
