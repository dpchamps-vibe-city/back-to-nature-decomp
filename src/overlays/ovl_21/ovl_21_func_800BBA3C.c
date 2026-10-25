#include "common.h"
#include "sound_driver.h"
#include "sound_commands.h"

void ovl_21_func_800BBA3C(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x12, 1);
}
