#include "common.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SetVal8005E2BC(s32 arg0);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_80101814(void);
void ovl_11_func_80101964(void);
void ovl_11_func_801014A4(s8 arg0);
void ovl_11_func_801014F8(void);
s32 ovl_11_func_80101B28(s8 arg0);
s32 ovl_11_func_80101B84(s8 arg0);
void ovl_11_func_80101B9C(void);
void ovl_11_func_80100194(void);
void ovl_11_func_8010021C(void);

void ovl_11_func_80100FFC(void) {
    s32 ui;
    s32 text;
    u32 *panels;

    ui = gActiveRenderContext->field_D8;
    panels = (u32 *)(ui + 0x74);
    text = ui + 0x70;
    DrawMenuPanel(panels, 1, 0x16, 0x10, 0x10E, 0x4A);
    DrawMenuPanel(panels, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(text, (s32)(D_8005396A + 0x28 + gTextBankOffsets[0]), 0x21, 0x20);
    func_80017B3C(text, (s32)(D_8005396A + gTextBankOffsets[0]), 0x21, 0x6A);
    ovl_11_func_80101814();
    ovl_11_func_80101964();
    if (D_801273E4_state == 4) {
        ovl_11_func_801014A4(Ovl11_D_801273E7);
    } else if (D_801273E4_state == 5) {
        ovl_11_func_801014F8();
    } else {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3BC);
        if (IsMessageWindowStatusFinal()) {
            switch (((u32 *)gPad0Input)[0]) {
            case 0x1000:
            case 0x4000:
                Ovl11_D_801273E7 = Ovl11_D_801273E7 >= 4 ?
                    Ovl11_D_801273E7 - 4 : Ovl11_D_801273E7 + 4;
                PlaySoundEffect(5);
                break;
            case 0x2000:
                Ovl11_D_801273E7++;
                if (Ovl11_D_801273E7 == Ovl11_D_801273E7 / 4 * 4) {
                    Ovl11_D_801273E7 -= 4;
                }
                PlaySoundEffect(5);
                break;
            case 0x8000:
                Ovl11_D_801273E7--;
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == -1) {
                    Ovl11_D_801273E7 += 4;
                }
                PlaySoundEffect(5);
                break;
            }
        }
        func_800248B0(gActiveRenderContext->field_D8 + 0x54,
                     (Ovl11_D_801273E7 - Ovl11_D_801273E7 / 4 * 4) * 34 + 0x87,
                     Ovl11_D_801273E7 / 4 * 34 + 0x67);
        if (IsMessageWindowStatusFinal()) {
            if (((u32 *)gPad0Input)[2] & 0x800) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                D_801273F0 = 0;
                Ovl11_D_801273DC = ovl_11_func_80101B9C;
            } else if (((u32 *)gPad0Input)[2] & 0x20) {
                PlaySoundEffect(1);
                if (D_801273EE_flags != 0) {
                    D_801273EE_flags = 0;
                } else if (D_801273E5_origin == 0) {
                    CloseMessageWindow();
                    Ovl11_D_801273DC = ovl_11_func_80100194;
                } else {
                    CloseMessageWindow();
                    Ovl11_D_801273DC = ovl_11_func_8010021C;
                }
            } else if (((u32 *)gPad0Input)[2] & 0x10) {
                if (D_80070D04 & ovl_11_func_80101B84(Ovl11_D_801273E7)) {
                    CloseMessageWindow();
                    PlaySoundEffect(0);
                    D_801273E4_state = 4;
                } else {
                    PlaySoundEffect(2);
                }
            } else if (((u32 *)gPad0Input)[2] & 0x40) {
                if (ovl_11_func_80101B28(Ovl11_D_801273E7) == 1) {
                    PlaySoundEffect(1);
                    if (Ovl11_D_801273E7 < 7) {
                        D_801273EE_flags &= ~ovl_11_func_80101B84(Ovl11_D_801273E7);
                    } else {
                        CloseMessageWindow();
                        D_801273E4_state = 5;
                    }
                } else {
                    if (D_80070D04 & ovl_11_func_80101B84(Ovl11_D_801273E7)) {
                        PlaySoundEffect(0);
                        if (Ovl11_D_801273E7 < 7) {
                            D_801273EE_flags |= ovl_11_func_80101B84(Ovl11_D_801273E7);
                        } else {
                            CloseMessageWindow();
                            D_801273E4_state = 5;
                        }
                    } else {
                        PlaySoundEffect(2);
                    }
                }
            }
        }
    }
}
