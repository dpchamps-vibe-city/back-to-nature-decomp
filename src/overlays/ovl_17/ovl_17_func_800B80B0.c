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
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"


void SsSeqClose (short);
void SsSeqSetVol (short, short, short);
void SsVabClose (short);
void ovl_17_func_800BA808 (s32 arg0);
void ovl_17_func_800BB394 (s16 arg0);
void ovl_17_func_800BB3F4 (s16 arg0);

void ovl_17_func_800B80B0(void) {
    u8 var_s0 = 0;

    ovl_17_func_800BA808(0x800000);
    if (IsScreenFadeComplete() == 1) {
        var_s0++;
    }
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        if (D_800BDB10 != -1) {
            StopSoundEffectVoice((s32) D_800BDB10);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        if (((Ovl17CompletionView *) D_8006C838)->mode == 1) {
            ovl_17_func_800BB394(1);
            ovl_17_func_800BB394(2);
            ovl_17_func_800BB394(4);
            ovl_17_func_800BB394(6);
            ovl_17_func_800BB394(0xF);
            ovl_17_func_800BB394(0x10);
            ovl_17_func_800BB394(0x11);
            ovl_17_func_800BB394(0x13);
            ovl_17_func_800BB394(0x15);
            ovl_17_func_800BB394(0x16);
            ovl_17_func_800BB394(0x1A);
            ovl_17_func_800BB394(0x1C);
            ovl_17_func_800BB3F4(3);
            ovl_17_func_800BB3F4(7);
            ovl_17_func_800BB3F4(0x14);
        }
        ((Ovl17CompletionView *) D_8006C838)->state = 0xFF;
        ((Ovl17CompletionView *) D_8006C838)->flags |= 0x80000;
        RequestSceneChange(6);
    }
}
