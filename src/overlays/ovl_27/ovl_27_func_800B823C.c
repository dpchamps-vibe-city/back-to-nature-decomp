#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "sprite_upload.h"
#include "sprite_render.h"


short SsSeqOpen (unsigned long *, short);
void SsSetMono (void);
void SsSetStereo (void);
void SsStart (void);
void SsUtReverbOn (void);
void ovl_27_func_800B9324 (void);
void ovl_27_func_800BA5BC (void);
s32 ovl_27_func_800BA620 (void);
void ovl_27_func_800BA750 (void);
s32 ovl_27_func_800BA7B0 (void);
void ovl_27_func_800BA514 (void);
s32 ovl_27_func_800BA578 (void);
void ovl_27_func_800BA80C (void);
s32 ovl_27_func_800BA814 (void);
void ovl_27_func_800BA4C4 (void);
void StepMasterVolumeRamp (void);

void ovl_27_func_800B823C(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    env_struct_0x134 *second;
    Ovl27StateCounterView *state;

    if (!(D_800C4A28 & 0x4000)) {
        ovl_27_func_800B9324();
    }
    switch (D_800C4A18) {
    case 0:
        temp_v0 = IsScreenFadeComplete();
        if (temp_v0 == 1) {
            D_800C4A18 = (s16) temp_v0;
            return;
        }
    default:
        return;
    case 1:
        ovl_27_func_800BA5BC();
        D_800C4A18 = 2;
        return;
    case 2:
        if (ovl_27_func_800BA620() != 0) {
            D_800C4A18 = 3;
            return;
        }
        break;
    case 3:
        ovl_27_func_800BA750();
        D_800C4A18 = 4;
        return;
    case 4:
        if (ovl_27_func_800BA7B0() != 0) {
            D_800C4A18 = 5;
            return;
        }
        break;
    case 5:
        ovl_27_func_800BA514();
        D_800C4A18 = 6;
        return;
    case 6:
        if (ovl_27_func_800BA578() != 0) {
            D_800C4A18 = 0xA;
            return;
        }
        break;
    case 10:
        ovl_27_func_800BA80C();
        D_800C4A18 = 0xB;
        return;
    case 11:
        if (ovl_27_func_800BA814() != 0) {
            D_800C4A18 = 7;
            return;
        }
        break;
    case 7:
        BeginScreenFadeIncrease(0x1E, 0, 2);
        ovl_27_func_800BA4C4();
        D_800C4A18 = 8;
        return;
    case 8:
        UpdateMasterVolumeAndSoundTransfer();
        if (IsSoundTransferComplete() != 0) {
            BeginMasterVolumeFadeIn(0xA);
            D_800C4A18 = 9;
            return;
        }
        break;
    case 9:
        if (GetMasterVolumeRampState() == 0) {
            D_800C4A18 = 0xC;
            return;
        }
        break;
    case 12:
        temp_v0_2 = IsScreenFadeComplete();
        if (temp_v0_2 == 1) {
            D_800C4A18 = 0;
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
