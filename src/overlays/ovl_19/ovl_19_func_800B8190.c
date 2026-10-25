#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"


void SsSeqClose (short);
void SsSeqSetVol (short, short, short);
void SsVabClose (short);
void ovl_19_func_800BA770 (void);
void ovl_19_func_800BBD14 (s16 arg0);
void ovl_19_func_800BA834 (s16 *arg0);
void ovl_19_func_800BAB4C (Ovl19Func800BAB4CArg *arg0);
void ovl_19_func_800BAFAC (s32 arg0, ObjectState *arg1, s16 *arg2, s16 arg3);

void ovl_19_func_800B8190(void) {
    u8 var_s0;
    u8 *base;

    ovl_19_func_800BA770();
    var_s0 = 0;
    if (IsScreenFadeComplete() == 1) {
        var_s0++;
    }
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        if (D_800BF6C2 != -1) {
            StopSoundEffectVoice((s32) D_800BF6C2);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        base = (u8 *)D_8006C838;
        *(s32 *)(base + 0x448C) = 0xFF;
        RequestSceneChange(6);
        *(s32 *)(base + 0xC) |= 0x80000;
        if (*(s32 *)(base + 0xE624) == 1) {
            ovl_19_func_800BBD14(1);
            ovl_19_func_800BBD14(4);
            ovl_19_func_800BBD14(5);
            ovl_19_func_800BBD14(6);
            ovl_19_func_800BBD14(8);
            ovl_19_func_800BBD14(0xF);
            ovl_19_func_800BBD14(0x11);
            ovl_19_func_800BBD14(0x13);
            ovl_19_func_800BBD14(0x15);
            ovl_19_func_800BBD14(0x19);
            ovl_19_func_800BBD14(0x1C);
            ovl_19_func_800BBD14(0x1F);
        }
    }
}
