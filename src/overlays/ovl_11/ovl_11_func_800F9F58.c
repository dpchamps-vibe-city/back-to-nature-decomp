#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_11_func_800F6640(void);

void ovl_11_func_800F9F58(void) {
    s32 flags;

    if (IsMessageWindowStatusFinal() != 0) {
        flags = ((GfxObj *)gPad0Input)->field_8;
        if (flags & 0x50) {
            CloseMessageWindow();
            PlaySoundEffect(0);
            D_80126F80 = 0;
            return;
        }
        if (flags & 0x20) {
            if (D_80126F88 == 1) {
                ovl_11_func_800F6640();
            }
            CloseMessageWindow();
            D_80126F7C = 2;
            PlaySoundEffect(3);
        }
    }
}
