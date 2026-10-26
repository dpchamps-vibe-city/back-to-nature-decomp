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
#include "ovl19_record_value_increment.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_projected_sprite.h"


void SsSeqClose (short);
void SsSeqSetVol (short, short, short);
void SsVabClose (short);
void ovl_19_func_800BA770 (void);
void ovl_19_func_800BA834 (s16 *arg0);
void ovl_19_func_800BAB4C (Ovl19Func800BAB4CArg *arg0);

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
            Ovl19RaiseRecordByteRangeValue(1);
            Ovl19RaiseRecordByteRangeValue(4);
            Ovl19RaiseRecordByteRangeValue(5);
            Ovl19RaiseRecordByteRangeValue(6);
            Ovl19RaiseRecordByteRangeValue(8);
            Ovl19RaiseRecordByteRangeValue(0xF);
            Ovl19RaiseRecordByteRangeValue(0x11);
            Ovl19RaiseRecordByteRangeValue(0x13);
            Ovl19RaiseRecordByteRangeValue(0x15);
            Ovl19RaiseRecordByteRangeValue(0x19);
            Ovl19RaiseRecordByteRangeValue(0x1C);
            Ovl19RaiseRecordByteRangeValue(0x1F);
        }
    }
}
