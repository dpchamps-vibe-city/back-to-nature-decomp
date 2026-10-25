#include "common.h"
#include "sound_commands.h"

void ovl_27_func_800BA4C4(void) {
    SetSoundCommandReadBuffer(0, gGraphicsHeapBase + 0x4290);
    QueueSoundLoadCommand(0, 0);
    QueueSoundLoadCommand(0x3E8, 0);
    QueueSoundLoadCommand(0x1A, 0);
}
