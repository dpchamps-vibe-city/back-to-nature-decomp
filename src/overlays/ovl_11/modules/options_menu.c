/*
 * Pause-menu options pages and companion four-choice prompt.
 *
 * The options menu snapshots twelve packed settings bytes, edits that copy
 * and commits it on close. Sound/clock choices also preview their effects
 * immediately. The prompt has separate row/phase/result state, but shares
 * this menu lifecycle and the message-window/scene-exit protocol.
 *
 * All state is shared and non-reentrant. Original callback order and external
 * linkages are retained; the public lifecycle is in ovl11_options_menu.h.
 */
#include "common.h"
#include "ovl11_options_menu.h"
#include "game_types.h"
#include "game_flags.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "packet_work.h"
#include "sound_driver.h"
#include "pad_actuator_gate.h"
#include "pad_actuator_sequences.h"
#include "master_volume.h"
#include "sound_commands.h"
#include "ovl11_position_transition.h"
#include "message_window.h"
#include "beveled_panel.h"
#include "ovl11_clock_hud.h"

void func_80017A38(s16, s16);
void func_80017B3C(s32, s32, s32, s32);
void SetVal8005E2BC(s32);
void func_800248B0(s32, s16, s16);
void func_80017D9C(s32 *, u16 *, s16, s16, s32);
void ovl_11_func_800BCF28(void);

void Ovl11UpdateOptionsMenu(void);
void Ovl11ShowOptionsListRowDescription(s16 column, s16 row);
void Ovl11ShowOptionsRowDescription(s16 choice, s16 row);
s32 Ovl11ShowMenuChoice(void);

void Ovl11ResetOptionsMenuState(void) {
    gOvl11OptionsMenuState = 0;
    gOvl11OptionsSettingRow = 0;
    gOvl11OptionsSettingChoice = 0;
    gOvl11OptionsListRow = 0;
    gOvl11OptionsListColumn = 0;
    gOvl11OptionsRootRow = 0;
}

void Ovl11OpenOptionsMenu(void) {
    u8 *snapshot;
    s32 *work;
    u8 *farWork;

    if (gOvl11OptionsMenuState == 0) {
        Ovl11ResetOptionsMenuState();
        gOvl11OptionsMenuState = 1;
        snapshot = (u8 *)&gOvl11OptionsSettings_menu;
        work = (s32 *)&D_8006C838;
        farWork = (u8 *)(work + (0x8000 >> 2));
        *(Ovl11MenuSettings *)snapshot = *(Ovl11MenuSettings *)(farWork + 0x64CC);
        PlaySoundEffect(3);
    }
}

void Ovl11DrawOptionsMenu(void) {
    if (gOvl11OptionsMenuState != 0) {
        DrawScreenSubtractOverlay(0xA000U, 0xA000U, 0xA000U);
        func_80017A38(0, 2);
        Ovl11UpdateOptionsMenu();
        func_80017A38(0, 0);
    }
}

void Ovl11UpdateOptionsMenu(void) {
    s32 ready;
    u32 state;
    s32 buttons;
    s16 quotient;
    u8 *work;

    if (gOvl11OptionsMenuState == 0x30) {
        if (GetMessageWindowStatus() == 2) {
            work = (u8 *)D_8006C838;
            *(Ovl11MenuSettings *)(work + 0xE4CC) = gOvl11OptionsSettings_menu;
            ResetMessageWindow();
            gOvl11OptionsMenuState = 0;
            ovl_11_func_800BCF28();
            *(u32 *)(work + 0xC) |= 0x40000;
            Ovl11ApplyWorkSettings();
        }
        return;
    }
    ready = IsMessageWindowStatusFinal();
    if (ready == 1 && (*(s32 *)((u8 *)gPad0Input + 8) & 0x800)) {
        gOvl11OptionsMenuState = 0x30;
        PlaySoundEffect(3);
        CloseMessageWindow();
        return;
    }
    state = gOvl11OptionsMenuState;
    if (state != 1) {
        gOvl11OptionsPageHandlers[(state >> 4) - 1]();
    } else {
        DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x64), 1, 0x64, 0x4A, 0x82, 0x22);
        func_80017B3C(gActiveRenderContext->field_D8 + 0x60, (s32)(D_80053AE8 + gTextBankOffsets[0]), 0x70, 0x4E);
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3B0);
        func_800248B0(gActiveRenderContext->field_D8 + 0x5C, 0x6C, (s16)(gOvl11OptionsRootRow_selection[0] * 14 + 0x54));
        if (ready == 1) {
            buttons = *(s32 *)((u8 *)gPad0Input + 8);
            if (buttons & 0x20) {
                gOvl11OptionsMenuState = 0x30;
                PlaySoundEffect(3);
                CloseMessageWindow();
            } else if (buttons & 0x40) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                gOvl11OptionsMenuState = (gOvl11OptionsRootRow_selection[0] + 1) << 4;
            } else {
                buttons = *(s32 *)gPad0Input;
                if (buttons & 0x4000) {
                    PlaySoundEffect(5);
                    gOvl11OptionsRootRow_selection[0]++;
                } else if (buttons & 0x1000) {
                    gOvl11OptionsRootRow_selection[0]++;
                    PlaySoundEffect(5);
                }
                quotient = gOvl11OptionsRootRow_selection[0] / 2;
                gOvl11OptionsRootRow_selection[0] -= quotient * 2;
            }
        }
    }
}

void Ovl11UpdateOptionsListPage(void) {
    s32 pressed;
    s32 held;
    s32 rowCount;
    s32 remainder;

    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x58), 1, 0x14, 0x37, 0x118, 0x69);
    func_80017A38(0, 2);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32)((void *)((u8 *)&D_80053B22 + *gTextBankOffsets)), 0x20, 0x3B);
    func_80017A38(0, 0);
    if (gOvl11OptionsMenuState == 0x10 && IsMessageWindowStatusFinal() == 1) {
        if (*(s32 *)((u8 *)gPad0Input + 0) & 0xA000) {
            gOvl11OptionsListColumn = (gOvl11OptionsListColumn + 1) % 2;
            PlaySoundEffect(5);
        }
        rowCount = 6;
        if (gOvl11OptionsListColumn == 0) {
            rowCount = 5;
            if (gOvl11OptionsListRow_selection[0] == 5) {
                gOvl11OptionsListRow_selection[0] = (u16)gOvl11OptionsListRow_selection[0] - 1;
            }
        }
        held = *(s32 *)((u8 *)gPad0Input + 0);
        if (held & 0x5000) {
            s16 *selection = &gOvl11OptionsListRow_selection[0];
            *selection++ = gOvl11OptionsListRow_selection[0]
                + ((held & 0x4000) ? 1 : rowCount - 1);
            PlaySoundEffect(5);
        }
        remainder = gOvl11OptionsListRow_selection[0] % rowCount;
        pressed = *(s32 *)((u8 *)gPad0Input + 8);
        gOvl11OptionsListRow_selection[0] = remainder;
        if (pressed & 0x20) {
            PlaySoundEffect(1);
            CloseMessageWindow();
            gOvl11OptionsMenuState = 1;
        } else if (pressed & 0x40) {
            PlaySoundEffect(0);
            if (gOvl11OptionsMenuState == 0x10) {
                CloseMessageWindow();
                gOvl11OptionsMenuState = 0x11;
            } else {
                gOvl11OptionsMenuState = 0x10;
            }
        }
    }
    func_800248B0(gActiveRenderContext->field_D8 + 0x50, (s16)(gOvl11OptionsListColumn * 0x96 + 0x1C), (s16)((gOvl11OptionsListRow_selection[0] + 1) * 0xE + 0x41));
    Ovl11ShowOptionsListRowDescription(gOvl11OptionsListColumn, gOvl11OptionsListRow_selection[0]);
}

void Ovl11ShowOptionsListRowDescription(s16 column, s16 row) {
    s32 messageBase;

    if (gOvl11OptionsMenuState == 0x11) {
        messageBase = column * 5 + 0x367;
        RequestMessageWindow(3, row + messageBase);
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            gOvl11OptionsMenuState = 0x10;
        }
    }
    if (gOvl11OptionsMenuState == 0x10) {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3B2);
    }
}

void Ovl11UpdateOptionsSettingsPage(void) {
    s16 choiceCount;
    s32 buttons;
    s32 remainder;
    s32 *textOt;

    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x58), 1, 0x3A, 0x1E, 0xCA, 0x84);
    func_80017A38(0, 2);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32)(D_80053CB8 + gTextBankOffsets[0]), 0x3E, 0x22);
    func_80017A38(0, 0);
    if (gOvl11OptionsMenuState == 0x20 && IsMessageWindowStatusFinal() == 1) {
        if (((u32 *)gPad0Input)[0] & 0x5000) {
            if (gOvl11OptionsSettingChoice >= 2 && gOvl11OptionsSettingRow == 0) {
                gOvl11OptionsSettingChoice = 1;
            }
            gOvl11OptionsSettingRow += (((u32 *)gPad0Input)[0] & 0x4000) ? 1 : 3;
            PlaySoundEffect(5);
        }
        gOvl11OptionsSettingRow %= 4;
        remainder = 2;
        if (gOvl11OptionsSettingRow == 0) {
            remainder = 3;
        }
        choiceCount = remainder;
        buttons = ((u32 *)gPad0Input)[0];
        if (buttons & 0xA000) {
            s16 *selection = &gOvl11OptionsSettingChoice;
            *selection++ = gOvl11OptionsSettingChoice + ((buttons & 0x2000) ? 1 : choiceCount - 1);
            PlaySoundEffect(5);
        }
        remainder = gOvl11OptionsSettingChoice % choiceCount;
        buttons = ((u32 *)gPad0Input)[2];
        gOvl11OptionsSettingChoice = remainder;
        if (buttons & 0x20) {
            CloseMessageWindow();
            PlaySoundEffect(1);
            gOvl11OptionsMenuState = 1;
        } else if (buttons & 0x40) {
            switch (gOvl11OptionsSettingRow) {
            case 0:
                gOvl11OptionsSettings_options[0] = remainder;
                break;
            case 1:
                if (remainder == 0 && gOvl11OptionsSettings_options[1] == 0) {
                    SetPadActuatorsEnabled(1);
                    RequestPadActuatorSequence(0, 0);
                }
                gOvl11OptionsSettings_options[1] = gOvl11OptionsSettingChoice != 1;
                break;
            case 2:
                gOvl11OptionsSoundSetters[remainder]();
                gOvl11OptionsSettings_options[2] = gOvl11OptionsSettingChoice != 1;
                break;
            case 3:
                gOvl11OptionsClockSetters[remainder]();
                gOvl11OptionsSettings_options[3] = gOvl11OptionsSettingChoice != 1;
                break;
            }
            PlaySoundEffect(0);
        } else if (buttons & 0x10) {
            PlaySoundEffect(0);
            if (gOvl11OptionsMenuState == 0x20) {
                CloseMessageWindow();
                gOvl11OptionsMenuState = 0x21;
            } else {
                gOvl11OptionsMenuState = 0x20;
            }
        }
    }
    func_800248B0(gActiveRenderContext->field_D8 + 0x50, (s16)(gOvl11OptionsSettingChoice * 65 + 0x42), (s16)((gOvl11OptionsSettingRow + 1) * 28 + 0x28));
    textOt = (s32 *)(gActiveRenderContext->field_D8 + 0x54);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + gTextBankOffsets[0]), 0x46, 0x3E, gOvl11OptionsSettings_options[0] != 0);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0xE + gTextBankOffsets[0]), 0x87, 0x3E, gOvl11OptionsSettings_options[0] != 1);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0x1C + gTextBankOffsets[0]), 0xC8, 0x3E, gOvl11OptionsSettings_options[0] != 2);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0x38 + gTextBankOffsets[0]), 0x46, 0x5A, gOvl11OptionsSettings_options[1] != 1);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0x3E + gTextBankOffsets[0]), 0x87, 0x5A, gOvl11OptionsSettings_options[1] != 0);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0x46 + gTextBankOffsets[0]), 0x46, 0x76, gOvl11OptionsSettings_options[2] != 1);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0x54 + gTextBankOffsets[0]), 0x87, 0x76, gOvl11OptionsSettings_options[2] != 0);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0x5E + gTextBankOffsets[0]), 0x46, 0x92, gOvl11OptionsSettings_options[3] != 1);
    func_80017D9C(textOt, (u16 *)(D_80053D6A + 0x66 + gTextBankOffsets[0]), 0x87, 0x92, gOvl11OptionsSettings_options[3] != 0);
    Ovl11ShowOptionsRowDescription(gOvl11OptionsSettingChoice, gOvl11OptionsSettingRow);
}

void Ovl11ShowOptionsRowDescription(s16 choice, s16 row) {
    s32 message;

    if (gOvl11OptionsMenuState == 0x21) {
        if (row == 0) {
            message = choice + 0x37B;
        } else {
            message = row + 0x37E;
        }
        RequestMessageWindow(3, message);
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            gOvl11OptionsMenuState = 0x20;
        }
    }
    if (gOvl11OptionsMenuState == 0x20) {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3B3);
    }
}

void Ovl11ApplyWorkSettings(void) {
    s16 *work;
    char *soundWork;
    char *clockWork;

    work = (s16 *)D_8006C838;
    if (work[0x7267]) {
        SetPadActuatorsEnabled(1);
    } else {
        SetPadActuatorsEnabled(0);
    }
    soundWork = (char *)&D_8006C838;
    soundWork += 0x8000;
    if (*(s16 *)(soundWork + 0x64D0)) {
        SetSoundStereo();
    } else {
        SetSoundMono();
    }
    clockWork = (char *)&D_8006C838;
    clockWork += 0x8000;
    if (*(s16 *)(clockWork + 0x64D2)) {
        Ovl11EnableClockHud();
    } else {
        Ovl11DisableClockHud();
    }
}

void Ovl11ResetMenuChoiceState(void) {
    gOvl11MenuChoiceRow = 0;
    gOvl11MenuChoiceState = 0xFF;
}

void Ovl11BeginMenuChoice(void) {
    Ovl11ResetMenuChoiceState();
    gOvl11MenuChoiceState = 0;
}

void Ovl11ServiceMenuChoice(void) {
    s32 result;

    if (gOvl11MenuChoiceState == 0) {
        result = Ovl11ShowMenuChoice();
        gOvl11MenuChoiceResult = result;
        if (result != 0) {
            gOvl11MenuChoiceState = 0xFE;
        }
    } else if (gOvl11MenuChoiceState == 0xFE) {
        if (IsMessageWindowStatusFinal() != 0) {
            gOvl11MenuChoiceState = 0xFF;
            ResetMessageWindow();
            if (gOvl11MenuChoiceResult == -1 || gOvl11MenuChoiceResult == 3 ||
                gOvl11MenuChoiceResult == 2 || gOvl11MenuChoiceResult == 1) {
                ovl_11_func_800BCF28();
            }
        }
    }
}

/* The prompt wraps and masks its selector as an unsigned halfword. Keep this
 * lvalue view distinct from the signed reset/state declarations. */
#define MENU_CHOICE_ROW (*(u16 *)&gOvl11MenuChoiceRow)

s32 Ovl11ShowMenuChoice(void) {
    s32 ready;
    s32 buttons;
    s16 textRow;

    DrawScreenSubtractOverlay(0x40000, 0x40000, 0x40000);
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x1B7);
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x58), 1, 0x10, 0x10, 0xAC, 0x58);
    func_80017A38(0, 2);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32)(D_80053A22 + gTextBankOffsets[0]), 0x12, 0x12);
    func_80017A38(0, 0);
    ready = IsMessageWindowStatusFinal();
    if (ready) {
        buttons = *(s32 *)gPad0Input;
        if (buttons & 0x4000) {
            PlaySoundEffect(5);
            MENU_CHOICE_ROW++;
        } else if (buttons & 0x1000) {
            PlaySoundEffect(5);
            MENU_CHOICE_ROW += 3;
        }
        MENU_CHOICE_ROW &= 3;
    }
    if (MENU_CHOICE_ROW == 0) {
        textRow = MENU_CHOICE_ROW;
    } else if (MENU_CHOICE_ROW == 1) {
        textRow = MENU_CHOICE_ROW + 1;
    } else {
        textRow = MENU_CHOICE_ROW + 2;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 0x50, 0x16, (s16)(textRow * 14 + 0x18));
    if (ready) {
        buttons = *(s32 *)((u8 *)gPad0Input + 8);
        if (buttons & 0x820) {
            PlaySoundEffect(1);
            CloseMessageWindow();
            return -1;
        } else if (buttons & 0x40) {
            PlaySoundEffect(0);
            CloseMessageWindow();
            switch (MENU_CHOICE_ROW) {
            case 1:
                SetGameFlag(0x48, 1);
                /* fall through: both first choices start the transition. */
            case 0:
                Ovl11BeginPositionTransition((Ovl11C7010Object *)&gPrimaryObjectBase);
                break;
            case 2:
                ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_254A0 = 0xF;
                /* fall through: both last choices request a faded scene exit. */
            case 3:
                if (MENU_CHOICE_ROW == 3) {
                    ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_254A0 = 0x11;
                }
                gSceneSubphase = 0;
                BeginScreenFadeIncrease(0xA, 0, 2);
                BeginMasterVolumeFadeOut(0xA);
                break;
            }
            return MENU_CHOICE_ROW + 1;
        }
    }
    return 0;
}
