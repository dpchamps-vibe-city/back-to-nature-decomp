#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

extern s8 D_801273E4;

void ovl_11_func_800BCF28(void);

void ovl_11_func_80103714(void) {
    char *base;

    D_801273E4 = 0;
    base = (char *)&D_8006C838;
    *(s32 *)(base + 0xC) |= 0x40000;
    ovl_11_func_800BCF28();
    PlaySoundEffect(3);
    *(s32 *)(base + 0x5234) |= 0x2000;
    ResetMessageWindow();
}
