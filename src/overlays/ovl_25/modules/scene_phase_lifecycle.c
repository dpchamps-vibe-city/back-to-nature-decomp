/*
 * scene_phase_lifecycle — reconstructed translation unit.
 *
 * Engine scene entry and all five callbacks of its private phase table:
 * resource and sound loading, fade-in wait, frame-mode dispatch, fade-out
 * request and teardown.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl25_scene_phase_lifecycle.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl25_vram_animation_slots.h"
#include "ovl25_background_color_cycle.h"
#include "ovl25_scene_display_pipeline.h"
#include "ovl25_scene_mode_handlers.h"
#include "ovl25_scene_record_seeders.h"

void func_80017A48(u32 arg0);

u32 func_80017A64(void);

void Ovl25DispatchScenePhase(void) {
    s32 *work;
    s32 *phaseTable;
    u32 previousTextDrawMode;

    previousTextDrawMode = func_80017A64();
    func_80017A48(3U);
    phaseTable = gOvl25ScenePhaseTable;
    work = (s32 *) &D_8006C838;
    /* Keep the original word-view dispatch and forwarded table argument;
     * every phase handler ignores that argument. */
    ((void (*)(s32 *)) phaseTable[work[0x448C >> 2]])(phaseTable);
    func_80017A48(previousTextDrawMode);
    if (work[0xC >> 2] & 0x08000000) {
        work[0xC >> 2] &= 0xF7FFFFFF;
    }
}

void Ovl25LoadSceneStageAndInit(void) {
    char *far_base;
    s32 *base;

    switch (*(u8 *)gOvl25SceneAssetLoadStep) {
    case 0:
        Ovl25LoadSharedSpriteArchive();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 1:
        Ovl25UploadCommonSpriteImages();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 2:
        Ovl25LoadObjectSpriteSet();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 3:
        Ovl25LoadSceneGeometry();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 4:
        Ovl25QueueSceneSoundLoads();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            *(u8 *)gOvl25SceneAssetLoadStep = 6;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)gOvl25SceneAssetLoadStep == 6) {
        Ovl25InitVramAnimationSlots();
        far_base = (char *)&D_8007AFF0;
        *(Ovl25SetupStatePair *)(far_base + 0x25474) =
            *(Ovl25SetupStatePair *)(far_base + 0x2479C);
        Ovl25RequestVramAnimations((s32)gLoadedAreaHeaderCopy);
        Ovl25ResetSceneState();
        Ovl25InitSceneCamera();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        *(u8 *)gOvl25SceneAssetLoadStep = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}

void Ovl25UpdateSceneTransition(void) {
    u8 var_s0;

    Ovl25UpdateSceneCameraOffset();
    Ovl25ApplySceneCamera();
    ApplyGteSceneState();
    Ovl25DrawSceneGeometry();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    Ovl25ResetSceneRecords();
    Ovl25DrawSceneSprites();
    if (var_s0 == 2) {
        s32 *base;

        base = (s32 *)&D_8006C838;
        base[0x448C >> 2]++;
    }
    Ovl25SetBackgroundColor(0, 0xA5, 0xD5);
}

void Ovl25UpdateSceneMode(void) {
    UpdateMessageWindow();
    Ovl25UpdateSceneCameraOffset();
    Ovl25ApplySceneCamera();
    ApplyGteSceneState();
    Ovl25DrawSceneGeometry();
    Ovl25UpdateVramAnimations();
    gOvl25SceneModeHandlers[D_800BFE44_view.selector]();
    Ovl25StepBackgroundColorCycle();
}

s32 Ovl25BeginSceneExit(void) {
    s32 *work;
    s32 nextPhase;

    Ovl25DrawSceneSprites();
    BeginScreenFadeFeedback(10);
    BeginMasterVolumeFadeOut(10);
    work = (s32 *)&D_8006C838;
    nextPhase = work[0x448C >> 2] + 1;
    work[0x448C >> 2] = nextPhase;
    return nextPhase;
}

void Ovl25TeardownScene(void) {
    s32 var_s0;
    u8 *base;

    Ovl25DrawSceneSprites();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0 = (var_s0 + 1) & 0xFF;
    }
    if (var_s0 == 2) {
        if (gOvl25SceneEffectVoice != -1) {
            StopSoundEffectVoice((s32) gOvl25SceneEffectVoice);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        base = (u8 *)D_8006C838;
        *(s32 *)(base + 0x448C) = 0xFF;
        RequestSceneChange(6);
        *(s32 *)(base + 0xC) |= 0x80000;
        Ovl25SetBackgroundColor(0, 0, 0);
    }
}
