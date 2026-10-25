#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

extern s32 D_801273DC;
extern s8 D_801273E4;
extern s8 D_801273E5;

void ovl_11_func_800FFDCC(void);
void ovl_11_func_800F69D0(void);
extern void ovl_11_func_8010021C(void);
s32 ovl_11_func_800F77A8(s16 arg0);
void SetVal8005E2BC(s32 arg0);

void ovl_11_func_800FFEF0(void) {
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    ovl_11_func_800FFDCC();
    ovl_11_func_800F69D0();
    D_801273E4 = 1;
    D_801273E5 = 1;
    D_801273DC = (s32)ovl_11_func_8010021C;
    PlaySoundEffect(3);
    ovl_11_func_800F77A8(-1);
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B5);
}
