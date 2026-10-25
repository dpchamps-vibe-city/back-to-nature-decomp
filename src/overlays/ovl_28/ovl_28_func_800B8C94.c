#include "common.h"
#include "sound_commands.h"

void ovl_28_func_800B8C94(void) {
    SetSoundCommandReadBuffer(0, D_8005E3B0 + 0x4290);
    QueueSoundLoadCommand(0, 0);
    QueueSoundLoadCommand(0x3E8, 0);
    QueueSoundLoadCommand(0x23, 0);
}
