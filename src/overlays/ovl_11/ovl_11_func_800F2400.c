#include "common.h"
#include "game_types.h"
#include "message_window.h"

s32 ovl_11_func_800F2724(void);
void ovl_11_func_800F3840(void);

void ovl_11_func_800F2400(void) {
    if (D_80126C90 == 0) {
        D_80126C94 = 0;
    } else {
        if (GetMessageWindowStatus() == 2) {
            if (D_80126C94 != 0) {
                D_80126C90 = 0;
                D_80126C94 = 0;
                ovl_11_func_800F3840();
            } else {
                if (ovl_11_func_800F2724() == 0) {
                    D_80126C90 = 0;
                }
            }
        } else if (((GfxObj *)gPad0Input)->field_8 & 0x20) {
            D_80126C90 = 0;
            CloseMessageWindow();
        }
    }
}
