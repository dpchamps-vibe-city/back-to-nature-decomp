#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"

/* ovl_19 overlay-local state counter; absolute-addressed (lui + %lo) from this
 * TU. Incremented once per call and reset when the handler runs. */
extern s32 D_800BD074;

/* State handler on the shared ovl_19 D_800BF4C0 s16 state array. The compiler
 * materialises the D_800BF560 label (D_800BF4C0 + 0xA0) and folds the +0xA0/
 * +0xE0 command records and the -0x90/-0x48 records as displacements from it. */
void ovl_19_func_800B8F38(void) {
    s32 n;

    n = D_800BD074 + 1;
    D_800BD074 = n;
    if (n % 30 == 0) {
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3A);
    }
    RequestMessageWindow(4, 0xA);
    if (GetMessageWindowStatus() == 2) {
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 2, 0x1E);
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 4, 0, 0x1E);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 7, 0, 0x5A);
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3B);
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3C);
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x44);
        D_800BD074 = 0;
        gOvl19SceneWork[7] = 0;
        gOvl19SceneWork[3] = 3;
    }
}
