#include "common.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"


void SsSeqClose(short);
void SsSeqSetVol(short, short, short);
void SsVabClose(short);

void ovl_23_func_800B8104(void) {
    s32 *base;
    s32 var_s0;

    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0 = (var_s0 + 1) & 0xFF;
    }
    if (var_s0 == 2) {
        if (D_800BFC90 != -1) {
            StopSoundEffectVoice((s32) D_800BFC90);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        base = (s32 *)&D_8006C838;
        base[0x448C >> 2] = 0xFF;
        RequestSceneChange(6);
        base[0xC >> 2] |= 0x80000;
    }
}
