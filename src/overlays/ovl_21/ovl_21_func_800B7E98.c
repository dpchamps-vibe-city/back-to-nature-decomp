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

void ovl_21_func_800BB328(void);
void ovl_21_func_800BB59C(void);
void ovl_21_func_800BB648(void);
void ovl_21_func_800BB8B8(void);
void ovl_21_func_800BBA3C(void);
void ovl_21_func_800B8228(void);

void ovl_21_func_800B7E98(void) {
    s32 *base;

    switch (D_800BCBB8) {
    case 0:
        ovl_21_func_800BB328();
        D_800BCBB8++;
        break;
    case 1:
        ovl_21_func_800BB59C();
        D_800BCBB8++;
        break;
    case 2:
        ovl_21_func_800BB648();
        D_800BCBB8++;
        break;
    case 3:
        ovl_21_func_800BB8B8();
        D_800BCBB8++;
        break;
    case 4:
        ovl_21_func_800BBA3C();
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
