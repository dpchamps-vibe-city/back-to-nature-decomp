/*
 * scene_setup — reconstructed translation unit.
 *
 * Title scene phase dispatch and staged initial reads: shared
 * setup-stage/completion state, two coordinated resets and the initialization
 * call chain. The setup either preserves the existing work image (flag 0x4000)
 * or calls the work_area_init quartet after clearing state memory. Reads,
 * screen fades and sound transfers complete over successive update calls. A four-byte unrepresented rodata item separates its table from
 * the menu table, a mechanical limit rather than original boundary evidence;
 * see notes/file-groupings.md.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "message_window.h"
#include "random.h"
#include "state_memory_init.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "sprite_upload.h"
#include "sprite_render.h"
#include "ovl27_backdrop_sprite_pool.h"
#include "ovl27_scene_resources.h"
#include "ovl27_scene_setup.h"
#include "ovl27_work_area_init.h"

void Ovl27DispatchTitleScenePhase(void) {
    s32 *base = D_800C49F8;

    ((void (*)(s32 *))base[gScenePhase])(base);
}

void Ovl27ResetTitleAnimationState(void);

void Ovl27ResetTitleSceneState(void) {
    Ovl27ResetTitleAnimationState();
    gOvl27TitleSetupStage = 0;
    gOvl27TitleMenuStage = 0;
    gOvl27TitleAnimationTicks = 0;
    D_800C4A1A = 1;
    gOvl27TitleExitVariant = 0;
    gOvl27TitleSavedWorkFlags = 0;
}

void Ovl27ResetTitleAnimationState(void) {
    gOvl27TitleAnimationTicks = 0;
    D_800C4A1A = 1;
    D_800C4A2C = 0;
    D_800C4A2E = 0;
    D_800C4A30 = 0;
    D_800C4A32 = 0;
    D_800C4A34 = 0;
    D_800C4A36 = 0;
    gOvl27TitleBackdropY = -0xA0;
    gOvl27TitleBackdropExtent = 0x30;
    gOvl27TitlePanelX = 0x140;
}

void Ovl27ResetTitleSceneState(void);

void Ovl27BeginTitleSceneSetup(void) {
    u8 *base;
    s32 flags;
    s32 sel;

    Ovl27ResetTitleSceneState();
    base = (u8 *)&D_8006C838;
    flags = *(s32 *)(base + 0xC);
    gOvl27TitleSavedWorkFlags = flags;
    *(s32 *)(base + 0xC) = flags & 0xFFFF7FFF;
    if (!(flags & 0x4000)) {
        sel = *(s32 *)(base + 0x18);
        InitStateMemory();
        *(s32 *)(base + 0x18) = sel;
        Ovl27InitSpriteFrameRecords(Rand(3));
        Ovl27InitPrimaryObject();
        Ovl27InitWorkOptions();
        Ovl27CopyInitialWorkRecords();
    }
    BeginScreenFadeIncrease(0, 0, 2);
    ResetMessageWindow();
    Ovl27BeginBackdropStageLoad();
    *(s32 *)(base + 0xC) &= -0x1001;
    *(s32 *)(base + 0x4488) += 1;
}

void Ovl27PollTitleSceneSetupLoad(void) {
    env_struct_0x134 *second;
    Ovl27StateCounterView *state;
    Ovl27StateCounterView *state2;

    if (Ovl27PollBackdropSpritePoolLoad() != 0) {
        memcpy(D_800605E8, D_800C4A40[Rand(3)], 0x1800);
        if (gOvl27TitleSavedWorkFlags & 0x4000) {
            gOvl27TitleSetupStage = 7;
            state = (Ovl27StateCounterView *)D_8006C838;
            state->counter++;
        } else {
            gRenderContexts->unk19 = 0xFF;
            gRenderContexts->unk1A = 0xFF;
            gRenderContexts->unk1B = 0xFF;
            second = &gRenderContexts[1];
            second->unk19 = 0xFF;
            second->unk1A = 0xFF;
            second->unk1B = 0xFF;
            gRenderContexts->unk18 = 1;
            gRenderContexts[1].unk18 = 1;
            Ovl27DrawBackdropAnim30();
            BeginScreenFadeDecrease(0x1E, 0, 2);
            gOvl27TitleSetupStage = 0;
            state2 = (Ovl27StateCounterView *)D_8006C838;
            state2->counter++;
        }
    }
}

short SsSeqOpen (unsigned long *, short);
void SsSetMono (void);
void SsSetStereo (void);
void SsStart (void);
void SsUtReverbOn (void);
void StepMasterVolumeRamp (void);

void Ovl27UpdateTitleSceneSetup(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    env_struct_0x134 *second;
    Ovl27StateCounterView *state;

    if (!(gOvl27TitleSavedWorkFlags & 0x4000)) {
        Ovl27DrawBackdropAnim30();
    }
    switch (gOvl27TitleSetupStage) {
    case 0:
        temp_v0 = IsScreenFadeComplete();
        if (temp_v0 == 1) {
            gOvl27TitleSetupStage = (s16) temp_v0;
            return;
        }
    default:
        return;
    case 1:
        Ovl27BeginSharedSpriteRead();
        gOvl27TitleSetupStage = 2;
        return;
    case 2:
        if (Ovl27PollSharedSpriteRead() != 0) {
            gOvl27TitleSetupStage = 3;
            return;
        }
        break;
    case 3:
        Ovl27BeginImageBankRead();
        gOvl27TitleSetupStage = 4;
        return;
    case 4:
        if (Ovl27PollImageBankRead() != 0) {
            gOvl27TitleSetupStage = 5;
            return;
        }
        break;
    case 5:
        Ovl27BeginSceneDataRead();
        gOvl27TitleSetupStage = 6;
        return;
    case 6:
        if (Ovl27PollSceneDataRead() != 0) {
            gOvl27TitleSetupStage = 0xA;
            return;
        }
        break;
    case 10:
        Ovl27BeginPageCopy();
        gOvl27TitleSetupStage = 0xB;
        return;
    case 11:
        if (Ovl27CopySelectedPage() != 0) {
            gOvl27TitleSetupStage = 7;
            return;
        }
        break;
    case 7:
        BeginScreenFadeIncrease(0x1E, 0, 2);
        Ovl27QueueSceneSoundLoads();
        gOvl27TitleSetupStage = 8;
        return;
    case 8:
        UpdateMasterVolumeAndSoundTransfer();
        if (IsSoundTransferComplete() != 0) {
            BeginMasterVolumeFadeIn(0xA);
            gOvl27TitleSetupStage = 9;
            return;
        }
        break;
    case 9:
        if (GetMasterVolumeRampState() == 0) {
            gOvl27TitleSetupStage = 0xC;
            return;
        }
        break;
    case 12:
        temp_v0_2 = IsScreenFadeComplete();
        if (temp_v0_2 == 1) {
            gOvl27TitleSetupStage = 0;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x1A, 0x7F);
            gRenderContexts->unk19 = 0;
            gRenderContexts->unk1A = 0;
            gRenderContexts->unk1B = 0;
            second = &gRenderContexts[1];
            second->unk19 = 0;
            second->unk1A = 0;
            second->unk1B = 0;
            gRenderContexts->unk18 = (u8) temp_v0_2;
            gRenderContexts[1].unk18 = (u8) temp_v0_2;
            state = (Ovl27StateCounterView *)D_8006C838;
            state->counter++;
        }
        break;
    }
}
