#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *arg0);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_19_func_800BAC50(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2);

void ovl_19_func_800B9124(void) {
    s32 n;
    s32 state;

    n = D_800BD078 + 1;
    D_800BD078 = n;
    if (n % 30 == 0) {
        D_800BF4C0[0x101] = PlaySoundEffect(0x3A);
    }
    if (D_800BF4C0[7] == 0) {
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0xC);
        D_800BF4C0[7]++;
    }
    if (ovl_19_func_800BA65C((Ovl19Func800BAC40Arg *)&D_800BF4C0[8]) == 1) {
        ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)&D_800BF4C0[8], 2, 3, 0x1E);
        state = GetMessageWindowStatus();
        if (state == 5) {
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50], 2, 0x1E);
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)&D_800BF4C0[8], 4, 3, 0x1E);
            D_800BF4C0[0x101] = PlaySoundEffect(0x3B);
            D_800BF4C0[0x101] = PlaySoundEffect(0x44);
            CloseMessageWindow();
            D_800BD078 = 0;
            D_800BF4C0[7] = 0;
            D_800BF4C0[3] = state;
        }
    } else {
        ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)&D_800BF4C0[8], 3, 3, 6);
    }
}
