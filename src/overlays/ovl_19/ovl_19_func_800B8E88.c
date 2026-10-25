#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo).
 * The two command records live at +0x10 and +0x58 (s16 index 8 and 0x2C);
 * the state halfword at +0x6 (index 3) and the display halfword at +0x202
 * (index 0x101) are written by this handler. */
extern s16 gOvl19SceneWork[];

/* POLICY EXCEPTION (user-approved 2026-07-31, see common.h): the target reads
 * hard $v0 between the func_8001FABC call and the following command call, so
 * the value stored at D_800BF4C0[0x101] is the most recent call's $v0 return.
 * CAPTURE_PREV_RET is the construct that reproduces that entry-liveness. */
void ovl_19_func_800B8E88(void) {
    s32 temp_s0;
    CAPTURE_PREV_RET(phantom);

    RequestMessageWindow(4, 9);
    temp_s0 = GetMessageWindowStatus();
    if (temp_s0 == 2) {
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
        PlaySoundEffect(0x3A);
        gOvl19SceneWork[0x101] = phantom;
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 2, 0, 0);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 1, 3, 0);
        gOvl19SceneWork[3] = temp_s0;
    }
}
