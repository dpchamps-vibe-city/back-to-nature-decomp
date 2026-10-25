#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl19_actor_controls.h"
#include "ovl19_actor_behavior_states.h"

void ovl_19_func_800B942C(void);
s32 ovl_19_func_800BA25C(void);
void ovl_19_func_800B9454(void);
void ovl_19_func_800B95D4(void);
s32 ovl_19_func_800BA73C(Ovl19Func800BA73CArg *arg0);
void ovl_19_func_800BA770(void);

void ovl_19_func_800B89C8(void) {
    s32 selection;
    s16 quotient;
    Ovl19Func800BAC40Arg *other;

    if (((s32 *)gPad0Input)[2] & 0x800) {
        *(s32 *)gOvl19SceneWork = *(s32 *)gOvl19SceneWork != 1;
        if (*(s32 *)gOvl19SceneWork == 1) {
            Ovl19PauseActorSprites();
            gOvl19SceneWork[0x101] = PlaySoundEffect(0x36);
        } else {
            Ovl19ResumeActorSprites();
        }
    }
    if (*(s32 *)gOvl19SceneWork == 1) {
        ovl_19_func_800B942C();
    } else {
        gOvl19SceneWork[5]++;
        if (gOvl19SceneWork[5] == 10000) {
            gOvl19SceneWork[5] = 0;
        }
        selection = ovl_19_func_800BA25C();
        if (selection != -1) {
            if (D_800BD070 == -1) {
                D_800BD070 = selection;
                RequestMessageWindow(4, 0x15);
                gOvl19SceneWork[0x102] = PlaySoundEffect(0x43);
                Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50 + selection * 0x20], 3, 0);
                other = (Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50];
                if (selection == 0) {
                    other = (Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70];
                }
                Ovl19SetActorState(other, 0, 0);
                Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x90], 4, 0);
            }
            if (GetMessageWindowStatus() == 2) {
                StopSoundEffectVoice(gOvl19SceneWork[0x102]);
                gOvl19SceneWork[2] = 6;
                gOvl19SceneWork[6] = D_800BD070;
                gOvl19SceneWork[0x102] = PlaySoundEffect(0x43);
            } else {
                Ovl19UpdateActorBehaviors();
            }
        } else {
            D_800BD070 = -1;
            if (gOvl19SceneWork[0x51] == 1) {
                if (((s32 *)gPad0Input)[2] & 0x40) {
                    Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 2, 30);
                }
            } else {
                gOvl19SceneWork[0x52]--;
                if (gOvl19SceneWork[0x52] <= 0) {
                    Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
                }
            }
            ovl_19_func_800B9454();
            ovl_19_func_800B95D4();
            Ovl19UpdateActorBehaviors();
            if (ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)&gOvl19SceneWork[8]) == 1) {
                quotient = gOvl19SceneWork[5] / 30;
                if (gOvl19SceneWork[5] == quotient * 30) {
                    gOvl19SceneWork[0x101] = PlaySoundEffect(0x3A);
                }
            }
        }
        ovl_19_func_800BA770();
    }
}
