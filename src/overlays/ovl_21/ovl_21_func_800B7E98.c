#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl21_scene_camera_geometry.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "ovl21_scene_resources.h"

void ovl_21_func_800B8228(void);

void ovl_21_func_800B7E98(void) {
    s32 *base;

    switch (D_800BCBB8) {
    case 0:
        Ovl21LoadSceneSpriteArchive();
        D_800BCBB8++;
        break;
    case 1:
        Ovl21UploadSceneSpriteChunk();
        D_800BCBB8++;
        break;
    case 2:
        Ovl21LoadUiAndObjectSprites();
        D_800BCBB8++;
        break;
    case 3:
        Ovl21LoadSceneGeometry();
        D_800BCBB8++;
        break;
    case 4:
        Ovl21QueueSceneSound();
        D_800BCBB8++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            D_800BCBB8 = 6;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (D_800BCBB8 == 6) {
        ovl_21_func_800B8228();
        Ovl21InitSceneCamera();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        D_800BCBB8 = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}
