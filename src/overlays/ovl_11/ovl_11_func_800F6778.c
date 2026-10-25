#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_11_func_800F6680(void);
void SetVal8005E2BC(s32 arg0);

void ovl_11_func_800F6778(void) {
    ovl_11_func_800F6680();
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    PlaySoundEffect(3);
    D_80126E40 = 4;
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x37A);
}
