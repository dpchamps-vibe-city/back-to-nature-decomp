#include "common.h"
#include "message_window.h"

void ovl_19_func_800BA770(void);

void ovl_19_func_800B86DC(void) {
    s32 status;
    s32 result;

    ovl_19_func_800BA770();
    switch (gOvl19SceneWork[4]) {
    case 0:
        gOvl19SceneWork[2] = 2;
        gOvl19SceneWork[3] = 0;
        return;
    case 1:
        RequestMessageWindow(4, 0xF);
        break;
    case 2:
        RequestMessageWindow(4, 0x10);
        break;
    }
    status = GetMessageWindowStatus();
    if (status == 2) {
        result = GetMessageWindowChoice();
        if (result == 1) {
            gOvl19SceneWork[0x100] = 0;
            gOvl19SceneWork[2] = 3;
        } else if (result == status) {
            gOvl19SceneWork[2] = result;
            gOvl19SceneWork[3] = 0;
        }
    }
}
