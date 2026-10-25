#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_scene_camera_geometry.h"
#include "message_window.h"
#include "ovl19_scene_resources.h"

void ovl_19_func_800B82B8(void);

void ovl_19_func_800B7F34(void) {
    s32 *base;

    switch (D_800BCEE4) {
    case 0:
        Ovl19LoadSceneSpriteArchive();
        D_800BCEE4++;
        break;
    case 1:
        Ovl19UploadSceneSpriteChunk();
        D_800BCEE4++;
        break;
    case 2:
        Ovl19LoadUiAndActorSprites();
        D_800BCEE4++;
        break;
    case 3:
        Ovl19LoadSceneGeometry();
        D_800BCEE4++;
        break;
    case 4:
        Ovl19QueueSceneSound();
        D_800BCEE4++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            D_800BCEE4 = 6;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0xF, 0x7F);
        }
        break;
    }
    if (D_800BCEE4 == 6) {
        ovl_19_func_800B82B8();
        Ovl19InitSceneCamera();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        D_800BCEE4 = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}
