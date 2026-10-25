#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo).
 * The two command records live at +0x10 and +0x58 (s16 index 8 and 0x2C);
 * the +0xA0/+0xE0 records at s16 index 0x50/0x70; the state halfword at +0x6
 * (index 3) and the display halfword at +0xE (index 7) are written here. */
extern s16 gOvl19SceneWork[];

void ovl_19_func_800B9050(void) {
    RequestMessageWindow(4, 0xB);
    if (gOvl19SceneWork[9] == 4) {
        if (gOvl19SceneWork[0xB] < 2) {
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 6, 0, 0x3C);
        }
    }
    if (GetMessageWindowStatus() == 2) {
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 3, 3, 0x1E);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 1, 3, 0x1E);
        gOvl19SceneWork[7] = 0;
        gOvl19SceneWork[3] = 4;
    }
}
