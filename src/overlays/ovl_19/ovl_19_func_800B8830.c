#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"
#include "ovl19_projected_actor_render.h"

void ovl_19_func_800B843C(void);
/* Executable aliases: SetVal8005E2BC, SetMessagePortraitEnabled and GetMessageWindowStatus. */
void SetVal8005E2BC(s32 arg0);

void ovl_19_func_800B8830(void) {
    ovl_19_func_800B843C();
    Ovl19DrawProjectedActorSprites();
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, Ovl19_D_800BCFF0[gOvl19SceneWork[4]]);
    if (GetMessageWindowStatus() == 5) {
        gOvl19SceneWork[0x100]++;
        if (gOvl19SceneWork[0x100] >= 60) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x90], 4, 0);
            CloseMessageWindow();
            gOvl19SceneWork[0x100] = 0;
            gOvl19SceneWork[2] = 4;
        }
    }
}
