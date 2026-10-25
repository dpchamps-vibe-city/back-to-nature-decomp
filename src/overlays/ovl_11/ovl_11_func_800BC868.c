#include "common.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"

s32 ovl_11_func_800BCB5C(void);

void ovl_11_func_800BC868(void) {
    s32 temp_s1;

    temp_s1 = ovl_11_func_800BCB5C();
    if (!(((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x4000) && (((struct Ovl11SoundWork *) &D_8007AFF0)->selected != temp_s1)) {
        BeginMasterVolumeFadeOut(0x14);
        while (GetMasterVolumeRampState() != 0) {
            UpdateMasterVolumeAndSoundTransfer();
        }
        StopSoundSequence(0);
        OpenSoundSequencesAndStart();
        RewindSoundLoadSlots(2, 0);
        ((struct Ovl11SoundWork *) &D_8007AFF0)->previous = ((struct Ovl11SoundWork *) &D_8007AFF0)->header[7];
        QueueSoundLoadCommand((s16) temp_s1, 0);
        ((struct Ovl11SoundWork *) &D_8007AFF0)->selected = temp_s1;
        if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0xF80) {
            D_801217A4 = 0;
        } else {
            D_801217A4 = 1;
        }
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x80;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x800;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x100;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x200;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x400;
    }
}
