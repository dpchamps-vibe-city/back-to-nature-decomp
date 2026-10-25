#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"

void SetVal8005E2BC(s32 arg0);
void ovl_11_func_80100B50(u8 arg0, u8 arg1);
void ovl_11_func_80103714(void);
void ovl_11_func_8010021C(void);

void ovl_11_func_80100644(void) {
    s32 buttons;

    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B9);
    ovl_11_func_80100B50((u8)D_801273EC, 0);
    if (IsMessageWindowStatusFinal() != 0) {
        buttons = ((GfxObj *)gPad0Input)->field_8;
        if (buttons & 0x800) {
            CloseMessageWindow();
            Ovl11_D_801273DC = ovl_11_func_80103714;
        } else if (buttons & 0x70) {
            CloseMessageWindow();
            PlaySoundEffect(1);
            Ovl11_D_801273DC = ovl_11_func_8010021C;
        }
    }
}
