#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "integer_power.h"
#include "ovl11_menu_grid_view.h"

void func_80017A38(s16 arg0, s16 arg1);
void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_800F7AA0(void);

void ovl_11_func_800F787C(void) {
    s32 firstText;
    s32 secondText;
    s32 message;
    s32 items;
    s32 selected;
    s32 snapshot;
    s32 object;
    s32 text;
    u32 *panel;
    s32 *display;

    func_80017A38(0, 2);
    if (IsMessageWindowStatusFinal() && (*(s32 *)((u8 *)gPad0Input + 8) & 0x800)) {
        gOvl11InteractionPhase = 0xFE;
        CloseMessageWindow();
        PlaySoundEffect(3);
        return;
    }
    firstText = gOvl11InteractionPhase;
    if (firstText == 0) {
        firstText = 0x448;
        secondText = 0x466;
        message = 0x3AD;
        items = (s32)D_8006C838 + 0x468A;
        selected = (s32)D_8006C838 + 0x524C;
        snapshot = (s32)D_8006C838 + 0x5258;
    } else if (firstText == 2) {
        firstText = 0x454;
        secondText = 0x476;
        message = 0x3AE;
        items = (s32)D_8006C838 + 0x450A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else if (firstText == 1) {
        firstText = 0x454;
        secondText = 0x484;
        message = 0x3AF;
        items = (s32)D_8006C838 + 0x480A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else {
        return;
    }
    firstText = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + firstText));
    secondText = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + secondText));
    object = gActiveRenderContext->field_D8;
    panel = (u32 *)(object + 0x68);
    text = object + 0x64;
    display = (s32 *)(object + 0x5C);
    Ovl11DrawMenuGridPanels(panel, text, firstText, secondText);
    Ovl11DrawMenuGridPageCount(text, 0x40);
    ovl_11_func_800F7AA0();
    Ovl11DrawMenuGridSlot(display, 0, (s16 *)selected);
    Ovl11DrawMenuGridSlots(display, 1, pow_int(2, gMenuGridLayoutIndex + 1), (s16 *)snapshot);
    Ovl11UpdateMenuGridNavigation(0x40);
    items += gOvl11MenuGridPageIndex * 48;
    Ovl11DrawMenuGridSlots(display, 9, 8, (s16 *)items);
    Ovl11UpdateMenuGridMessage(text, (s32)panel, gOvl11MenuGridCursorSlot, gOvl11MenuGridPageIndex);
    if (gOvl11MenuGridMessageState == 0xFF) {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, message);
    }
}
