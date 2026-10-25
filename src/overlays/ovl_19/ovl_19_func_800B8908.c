#include "common.h"
#include "game_types.h"
#include "message_window.h"

void ovl_19_func_800BA770(void);
/* Executable aliases: SetVal8005E2BC, SetMessagePortraitEnabled and GetMessageWindowStatus. */
void SetVal8005E2BC(s32 arg0);
void ovl_19_func_800BAC50(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2);

void ovl_19_func_800B8908(void) {
    s32 state;

    ovl_19_func_800BA770();
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 0x12);
    state = GetMessageWindowStatus();
    if (state == 5) {
        D_800BF4C0[0x100]++;
        if (D_800BF4C0[0x100] >= 45) {
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50], 1, 0);
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x70], 1, 0);
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x90], 0, 0);
            CloseMessageWindow();
            D_800BF4C0[0x100] = 0;
            D_800BF4C0[2] = state;
        }
    }
}
