#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_23_func_800B9310(void);
/* Executable imports: SetVal8005E2BC and SetMessagePortraitEnabled. */
void SetVal8005E2BC(s32 arg0);

void ovl_23_func_800B8754(void) {
    ovl_23_func_800B9310();
    if (Ovl23_D_800BF87C_header.unk416 == 0) {
        CloseMessageWindow();
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0x2E);
    }
    Ovl23_D_800BF87C_header.unk416++;
    if (Ovl23_D_800BF87C_header.unk416 >= 60) {
        Ovl23_D_800BF87C_header.unk414 = PlaySoundEffect(0x38);
        CloseMessageWindow();
        Ovl23_D_800BF87C_header.unk0 = 6;
        Ovl23_D_800BF87C_header.unk416 = 0;
    }
}
