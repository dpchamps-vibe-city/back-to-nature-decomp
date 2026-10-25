#include "common.h"
#include "sound_driver.h"
#include "sound_commands.h"

void ovl_23_func_800BB758(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x12, 1);
}
