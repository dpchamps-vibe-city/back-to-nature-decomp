#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_19_func_800BA770(void);
void func_80017A08(s32 arg0, s32 arg1);
s32 ovl_19_func_800BADAC(s16 arg0);
void ovl_19_func_800B843C(void);

void ovl_19_func_800B8C74(void) {
    s32 flag;

    ovl_19_func_800BA770();
    flag = ((Ovl19SelectionState *)gOvl19SceneWork)->flag;
    if (flag == 0) {
        func_80017A08(0, ((Ovl19CompletionCounterView *)D_8006C838)->text);
        RequestMessageWindow(4, 0x16);
    } else {
        func_80017A08(0, (s32)((u8 *)gOvl19ValueLabelText +
            ovl_19_func_800BADAC(((Ovl19SelectionState *)gOvl19SceneWork)->values[
                D_800BCF24[((Ovl19SelectionState *)gOvl19SceneWork)->selection]])));
        RequestMessageWindow(4, 0x17);
    }
    if (GetMessageWindowStatus() == 2) {
        ((Ovl19SelectionState *)gOvl19SceneWork)->selection++;
        StopSoundEffectVoice(((Ovl19SelectionState *)gOvl19SceneWork)->handle);
        if (flag == 0) {
            if (((Ovl19SelectionState *)gOvl19SceneWork)->selection < 3) {
                ovl_19_func_800B843C();
                ((Ovl19SelectionState *)gOvl19SceneWork)->state = 0;
            } else {
                ((Ovl19SelectionState *)gOvl19SceneWork)->state = 7;
            }
        } else {
            ((Ovl19CompletionCounterView *)D_8006C838)->completed = 0;
            ((Ovl19CompletionCounterView *)D_8006C838)->counter++;
        }
    }
}
