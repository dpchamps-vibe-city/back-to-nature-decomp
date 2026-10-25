#include "common.h"
#include "message_window.h"

s16 *ovl_11_func_800F82FC(s32 arg0, s32 arg1);
s32 ovl_11_func_800D60D4(s16 *arg0);

void ovl_11_func_800F87AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 *record;
    s32 message;

    if (D_80126E4E != 255) {
        record = ovl_11_func_800F82FC(arg2, arg3);
        if (record[0] == 0xA4) {
            message = ovl_11_func_800D60D4(record) + 1000;
        } else {
            message = record[0];
        }
        RequestMessageWindow(3, message);
        if (GetMessageWindowStatus() == 2) {
            D_80126E4E = 255;
            CloseMessageWindow();
        }
    }
}
