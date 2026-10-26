#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl17_record_work_init.h"
#include "ovl17_sprite_resources.h"

void ovl_17_func_800B7E78(void) {
    switch (gOvl17SceneLoadStep) {
    case 0:
        Ovl17LoadSharedSpriteBank();
        gOvl17SceneLoadStep++;
        break;
    case 1:
        Ovl17LoadSpriteImageBank();
        gOvl17SceneLoadStep++;
        break;
    case 2:
        Ovl17LoadRecordSpriteBank();
        gOvl17SceneLoadStep++;
        break;
    case 3:
        Ovl17QueueSoundBankLoads();
        gOvl17SceneLoadStep++;
        break;
    case 4:
        if (IsSoundTransferComplete() != 0) {
            gOvl17SceneLoadStep = 5;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x15, 0x7F);
        }
        break;
    }
    if (gOvl17SceneLoadStep == 5) {
        D_800BD84C = 0;
        Ovl17InitRecordWork();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        gOvl17SceneLoadStep = 0;
        ((Ovl17CompletionView *)D_8006C838)->state++;
    }
}
