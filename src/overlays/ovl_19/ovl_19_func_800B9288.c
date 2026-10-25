#include "common.h"
#include "game_types.h"
#include "message_window.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_19_func_800BA25C(void);
void ovl_19_func_800B843C(void);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_19_func_800BAC50(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2);

/* State handler on the shared ovl_19 D_800BF4C0 s16 state array. The compiler
 * materialises the D_800BF560 label (D_800BF4C0 + 0xA0) and folds the +0xA0/
 * +0xE0 command records and the -0x90/-0x48 records as displacements from it. */
void ovl_19_func_800B9288(void) {
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 0xD);
    if (D_800BF4C0[9] == 4) {
        if (D_800BF4C0[0xB] < 2) {
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)&D_800BF4C0[8], 5, 3, 0x3C);
        }
    }
    if (ovl_19_func_800BA25C() == 1) {
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(1);
            SetMessagePortraitEnabled(1);
        }
        if (GetMessageWindowStatus() == 2) {
            ovl_19_func_800B843C();
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50], 0, 0);
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x70], 0, 0);
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)&D_800BF4C0[8], 1, 1, 0);
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x2C], 1, 3, 0);
            CloseMessageWindow();
            D_800BF4C0[7] = 0;
            D_800BF4C0[3] = 6;
        }
    }
}

