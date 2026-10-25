#include "common.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "gte_scene_state.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"

void ovl_21_func_800BB138(void);
void ovl_21_func_800BB250(void);
void ovl_21_func_800BB2B4(void);
void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B8134(void) {
    s32 var_s0;

    ovl_21_func_800BB138();
    ovl_21_func_800BB250();
    ApplyGteSceneState();
    ovl_21_func_800BB2B4();
    ovl_21_func_800BA4C0();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0 = (var_s0 + 1) & 0xFF;
    }
    if (var_s0 == 2) {
        if (((Ovl21ExitStateView *) D_800C0448)->handles[0] != -1) {
            StopSoundEffectVoice(((Ovl21ExitStateView *) D_800C0448)->handles[0]);
        }
        if (((Ovl21ExitStateView *) D_800C0448)->handles[1] != -1) {
            StopSoundEffectVoice(((Ovl21ExitStateView *) D_800C0448)->handles[1]);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        ((Ovl21MainStateView *) D_8006C838)->state = 0xFF;
        RequestSceneChange(6);
        ((Ovl21MainStateView *) D_8006C838)->flags |= 0x80000;
    }
}
