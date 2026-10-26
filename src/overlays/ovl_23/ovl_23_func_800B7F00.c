#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl23_sprite_resources.h"

void ovl_23_func_800B81AC (void);

void ovl_23_func_800B7F00(void) {
    s32 *base;

    switch (*(u8 *)&D_800BBA3C) {
    case 0:
        Ovl23LoadSharedSpriteBank();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 1:
        Ovl23LoadSpriteImageBank();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 2:
        Ovl23LoadActorSpriteBank();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 3:
        Ovl23QueueSoundBankLoads();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 4:
        if (IsSoundTransferComplete() != 0) {
            *(u8 *)&D_800BBA3C = 5;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)&D_800BBA3C == 5) {
        ovl_23_func_800B81AC();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        *(u8 *)&D_800BBA3C = 0;
        base = (s32 *)&D_8006C838;
        ++base[0x448C >> 2];
    }
}
