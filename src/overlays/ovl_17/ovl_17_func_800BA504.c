#include "common.h"
#include "sound_driver.h"
#include "sound_commands.h"

void ovl_17_func_800BA504(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x15, 1);
}
