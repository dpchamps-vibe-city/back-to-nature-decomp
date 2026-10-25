#include "common.h"
#include "screen_fade.h"
#include "screen_primitives.h"
#include "cd_audio.h"
#include "master_volume.h"
#include "sound_commands.h"

void func_80017A08(s32 arg0, s32 arg1);
/* Overlay imports of GetVal8005E5B8 and GetVal8005E5B4. */
s32 GetVal8005E5B8(void);
s32 GetVal8005E5B4(void);
s32 func_8002261C(s32 arg0, s32 arg1);
void ovl_11_func_800BC9AC(void);
void ovl_11_func_8011F958(void);
void ovl_11_func_8011F9EC(s32 *arg0);
s32 ovl_11_func_8011FB80(s16 arg0);

void ovl_11_func_8011F608(void) {
    switch (D_80128540) {
    case 1:
        StopSoundSequence(0);
        BeginScreenFadeIncrease(30, 1, 2);
        BeginMasterVolumeFadeOut(30);
        D_80128540 = 2;
        break;
    case 2:
        if (IsScreenFadeComplete() == 1 && GetMasterVolumeRampState() == 0) {
            D_80128540 = 3;
            ovl_11_func_8011F9EC(Ovl11_D_80128544);
            BeginMasterVolumeFadeIn(1);
        }
        break;
    case 3:
        func_80017A08(0, Ovl11_D_80128544[0]);
        func_80017A08(1, Ovl11_D_80128544[1]);
        func_80017A08(2, Ovl11_D_80128544[2]);
        func_8002261C(2, 0xFD);
        if (GetVal8005E5B4() == 2) {
            D_80128556 = ovl_11_func_8011FB80((s16)GetVal8005E5B8());
            D_80128540 = 4;
        }
        break;
    case 4:
        func_8002261C(2, D_80128556);
        if (GetVal8005E5B4() == 2) {
            if (D_80128556 == 0x10A) {
                PlaySoundSequenceWithReverb(0, 0, ((struct Ovl11SelectionMusic *)&D_8007AFF0)->music, 0);
                ovl_11_func_800BC9AC();
                BeginScreenFadeDecrease(10, 1, 2);
                BeginMasterVolumeFadeIn(10);
                D_80128540 = 8;
            } else {
                DrawScreenTileRect((u32 *)gActiveRenderContext->field_120, 0, 0, *(s16 *)&HWD0, *(s16 *)&VWD0, 0, 0);
                BeginScreenFadeIncrease(10, 0, 1);
                BeginMasterVolumeFadeIn(10);
                D_80128540 = 5;
            }
        }
        break;
    case 5:
        DrawScreenTileRect((u32 *)gActiveRenderContext->field_120, 0, 0, *(s16 *)&HWD0, *(s16 *)&VWD0, 0, 0);
        if (IsScreenFadeComplete() == 1 && GetMasterVolumeRampState() == 0) {
            StartCdAudioTrack(25);
            D_80128540 = 6;
        }
        break;
    case 6:
        if (GetCdAudioState() == 0) {
            D_80128558 = 30;
            D_80128540 = 7;
        }
        break;
    case 7:
        if (--D_80128558 <= 0) {
            BeginScreenFadeDecrease(10, 0, 1);
            PlaySoundSequenceWithReverb(0, 0, ((struct Ovl11SelectionMusic *)&D_8007AFF0)->music, 0);
            ovl_11_func_800BC9AC();
            BeginMasterVolumeFadeIn(10);
            D_80128540 = 8;
        }
        break;
    case 8:
        if (IsScreenFadeComplete() == 1 && GetMasterVolumeRampState() == 0) {
            ResetScreenFade();
            D_80128540 = 9;
        }
        break;
    case 9:
        ovl_11_func_8011F958();
        break;
    }
}
