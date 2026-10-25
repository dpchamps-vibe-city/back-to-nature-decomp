#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_11_func_8011EC54(void) {
    CloseMessageWindow();
    PlaySoundEffect(3);
    D_80128420 = 8;
}
