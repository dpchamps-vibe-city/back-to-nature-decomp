#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_23_func_800B9310(void);

void ovl_23_func_800B8584(void) {
    ovl_23_func_800B9310();
    RequestMessageWindow(4, 0x2B);
    if (GetMessageWindowStatus() == 2) {
        if (GetMessageWindowChoice() == 1) {
            Ovl23_D_800BF87C_header.unk414 = PlaySoundEffect(0x41);
            Ovl23_D_800BF87C_header.unk0 = 3;
        } else {
            Ovl23_D_800BF87C_header.unk0 = 1;
        }
    }
}
