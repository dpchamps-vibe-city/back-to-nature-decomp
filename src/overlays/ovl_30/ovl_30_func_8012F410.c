#include "common.h"
#include "sound_driver.h"
#include "sound_commands.h"

void ovl_30_func_8012F410(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E8, 0);
}
