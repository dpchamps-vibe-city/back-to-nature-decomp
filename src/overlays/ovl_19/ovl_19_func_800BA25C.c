#include "common.h"
#include "game_types.h"
#include "ovl19_actor_controls.h"

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo).
 * Two records live at this base + 0x10 and + 0x58; the fields read are the
 * s16 at +0x12 and the two s16 command arguments at +0x04 and +0x06. */
extern s16 D_800BF4C0[];

s32 ovl_19_func_800BA25C(void) {
    if (D_800BF4C0[0x35] >= -0x5A) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x2C], 0xB, D_800BF4C0[0x2E], 2);
        return 0;
    }
    if (D_800BF4C0[0x11] < -0x5A) {
        return -1;
    }
    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&D_800BF4C0[8], 0xB, D_800BF4C0[0xA], 2);
    return 1;
}
