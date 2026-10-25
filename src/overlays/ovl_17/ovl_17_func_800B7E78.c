#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_17_func_800B9FA8(void);
void ovl_17_func_800BA21C(void);
void ovl_17_func_800BA2C8(void);
void ovl_17_func_800BA504(void);
void ovl_17_func_800BA54C(void);

void ovl_17_func_800B7E78(void) {
    switch (D_800BB524) {
    case 0:
        ovl_17_func_800B9FA8();
        D_800BB524++;
        break;
    case 1:
        ovl_17_func_800BA21C();
        D_800BB524++;
        break;
    case 2:
        ovl_17_func_800BA2C8();
        D_800BB524++;
        break;
    case 3:
        ovl_17_func_800BA504();
        D_800BB524++;
        break;
    case 4:
        if (IsSoundTransferComplete() != 0) {
            D_800BB524 = 5;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x15, 0x7F);
        }
        break;
    }
    if (D_800BB524 == 5) {
        D_800BD84C = 0;
        ovl_17_func_800BA54C();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        D_800BB524 = 0;
        ((Ovl17CompletionView *)D_8006C838)->state++;
    }
}
