#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"

/* Overlay aliases of SetMessagePortraitEnabled and SetVal8005E2BC. */
void SetVal8005E2BC(s32 arg0);
void ovl_11_func_800F6640(void);
void ovl_11_func_800FA1CC(Ovl11FA1CCArg *arg0);
void ovl_11_func_800F9FF4(void);
void ovl_11_func_800F9F58(void);
void ovl_11_func_800FA410(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);

void ovl_11_func_800F9E4C(void) {
    struct Ovl11B264StateView *state;
    Ovl11FA1CCArg *entry;
    s32 object;

    if (IsMessageWindowStatusFinal() && (((GfxObj *)gPad0Input)->field_8 & 0x800)) {
        if (D_80126F88 == 1) {
            ovl_11_func_800F6640();
        }
        CloseMessageWindow();
        D_80126F7C = 2;
        PlaySoundEffect(3);
    } else {
        entry = (Ovl11FA1CCArg *)D_80070CF0_halfwords;
        ovl_11_func_800FA1CC(entry);
        ovl_11_func_800F9FF4();
        if (D_80126F80 == 0xFF) {
            SetMessagePortraitEnabled(0);
            SetVal8005E2BC(0);
            RequestMessageWindow(3, 0x3B4);
        } else {
            object = gActiveRenderContext->field_D8;
            state = (struct Ovl11B264StateView *)((char *)entry - 0x44B8);
            ovl_11_func_800FA410(object + 0x58, object + 0x5C,
                state->field_44B8, state->field_44BA, (s16)D_80126F84);
            return;
        }
        ovl_11_func_800F9F58();
    }
}
