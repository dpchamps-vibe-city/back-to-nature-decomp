#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"

void ovl_19_func_800BA770(void);
/* Executable aliases: SetVal8005E2BC, SetMessagePortraitEnabled and GetMessageWindowStatus. */
void SetVal8005E2BC(s32 arg0);

void ovl_19_func_800B8908(void) {
    s32 state;

    ovl_19_func_800BA770();
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 0x12);
    state = GetMessageWindowStatus();
    if (state == 5) {
        gOvl19SceneWork[0x100]++;
        if (gOvl19SceneWork[0x100] >= 45) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x90], 0, 0);
            CloseMessageWindow();
            gOvl19SceneWork[0x100] = 0;
            gOvl19SceneWork[2] = state;
        }
    }
}
