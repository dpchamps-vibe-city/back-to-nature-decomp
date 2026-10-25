#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"

void ovl_19_func_800B843C(void);
void ovl_19_func_800BA770(void);
/* Executable aliases: SetVal8005E2BC, SetMessagePortraitEnabled and GetMessageWindowStatus. */
void SetVal8005E2BC(s32 arg0);

void ovl_19_func_800B8830(void) {
    ovl_19_func_800B843C();
    ovl_19_func_800BA770();
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, Ovl19_D_800BCFF0[D_800BF4C0[4]]);
    if (GetMessageWindowStatus() == 5) {
        D_800BF4C0[0x100]++;
        if (D_800BF4C0[0x100] >= 60) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x70], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x90], 4, 0);
            CloseMessageWindow();
            D_800BF4C0[0x100] = 0;
            D_800BF4C0[2] = 4;
        }
    }
}
