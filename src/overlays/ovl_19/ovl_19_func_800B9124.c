#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *arg0);

void ovl_19_func_800B9124(void) {
    s32 n;
    s32 state;

    n = D_800BD078 + 1;
    D_800BD078 = n;
    if (n % 30 == 0) {
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3A);
    }
    if (gOvl19SceneWork[7] == 0) {
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0xC);
        gOvl19SceneWork[7]++;
    }
    if (ovl_19_func_800BA65C((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8]) == 1) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 2, 3, 0x1E);
        state = GetMessageWindowStatus();
        if (state == 5) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 2, 0x1E);
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 4, 3, 0x1E);
            gOvl19SceneWork[0x101] = PlaySoundEffect(0x3B);
            gOvl19SceneWork[0x101] = PlaySoundEffect(0x44);
            CloseMessageWindow();
            D_800BD078 = 0;
            gOvl19SceneWork[7] = 0;
            gOvl19SceneWork[3] = state;
        }
    } else {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 3, 3, 6);
    }
}
