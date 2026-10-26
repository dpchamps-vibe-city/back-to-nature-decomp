/*
 * Card-read and slot-transfer menu scenes: both dispatchers and all 54 stages,
 * in original order (ovl_15:801301A0–80132A8C exclusive). Their private prompt
 * copies implement different state numbering and exit/restore behavior; they
 * share the same card-monitor and menu context. Step only the active scene.
 * See ovl15_card_read_states.h and ovl15_card_read_sprite_states.h.
 *
 * PromptStateNN names preserve undecoded text meaning rather than guessing it.
 * Distinct return types, timeout paths and conditional card monitoring remain
 * intentional. Card I/O, save serialization and slot editing are external
 * services, not replicated here.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libmcrd.h"
#include "psyq/memory.h"
#include "ovl15_save_image.h"
#include "ovl15_card_read_states.h"
#include "ovl15_card_read_sprite_states.h"
#include "ovl15_save_menu_lifecycle.h"
#include "ovl15_save_menu_ui.h"
#include "game_types.h"
#include "psyq/strings.h"
#include "ovl15_card_filename.h"
#include "ovl15_save_checksums.h"
#include "beveled_panel.h"

void Ovl15CardReadEnter(void);
void Ovl15CardReadPollCards(void);
void Ovl15CardReadPromptNoCard(void);
void Ovl15CardReadChooseCard(void);
void Ovl15CardReadCheckSelectedCard(void);
s32 Ovl15CardReadPromptState05(void);
s32 Ovl15CardReadPromptState06(void);
void Ovl15CardReadScanSlots(void);
s32 Ovl15CardReadPromptState08(void);
void Ovl15CardReadChooseSaveSlot(void);
s32 Ovl15CardReadPromptState10(void);
void Ovl15CardReadBuildFilename(void);
void Ovl15CardReadReadSaveFile(void);
void Ovl15CardReadVerifyChecksums(void);
s32 Ovl15CardReadPromptState14(void);
s32 Ovl15CardReadPromptState15(void);
s32 Ovl15CardReadPromptState16(void);
void Ovl15CardReadConfirmExit(void);
void Ovl15CardReadReset(void);
void Ovl15CardReadRestoreImage(void);
s32 ovl_15_func_80135E9C(s16 arg0);

void Ovl15StepCardReadMenu(void) {
    s16 *temp_v0_2;
    s32 temp_v0;

    switch (gOvl15MenuState) {
    case 0:
        Ovl15CardReadEnter();
        return;
    case 1:
        Ovl15CardReadPollCards();
        return;
    case 2:
        Ovl15CardReadPromptNoCard();
        break;
    case 3:
        Ovl15CardReadChooseCard();
        break;
    case 4:
        Ovl15CardReadCheckSelectedCard();
        return;
    case 5:
        Ovl15CardReadPromptState05();
        break;
    case 6:
        Ovl15CardReadPromptState06();
        break;
    case 7:
        Ovl15CardReadScanSlots();
        return;
    case 8:
        Ovl15CardReadPromptState08();
        break;
    case 9:
        Ovl15CardReadChooseSaveSlot();
        break;
    case 10:
        Ovl15CardReadPromptState10();
        break;
    case 11:
        Ovl15CardReadBuildFilename();
        break;
    case 12:
        Ovl15CardReadReadSaveFile();
        return;
    case 13:
        Ovl15CardReadVerifyChecksums();
        break;
    case 14:
        Ovl15CardReadPromptState14();
        break;
    case 15:
        Ovl15CardReadPromptState15();
        break;
    case 16:
        Ovl15CardReadPromptState16();
        break;
    case 17:
        Ovl15CardReadConfirmExit();
        break;
    case 18:
        Ovl15CardReadReset();
        return;
    case 19:
        Ovl15CardReadRestoreImage();
        return;
    }

    temp_v0 = ovl_15_func_80135E9C(gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard);
    if (temp_v0 != 3) {
        if (temp_v0 != gOvl15CardStates[gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard]) {
            if (temp_v0 == 5) {
                gOvl15MenuState = 0;
            }
            if (temp_v0 == 4) {
                if (gOvl15SelectedCard == 2) {
                    gOvl15SelectedCard = (s16) (u16) gOvl15MonitoredCard;
                }
                gOvl15MenuState = 0x10;
            }
        }
        if (gOvl15SelectedCard == 2) {
            temp_v0_2 = &gOvl15CardStates[gOvl15MonitoredCard];
            if ((u32) ((u16) *temp_v0_2 - 6) >= 2U) {
                *temp_v0_2 = (s16) temp_v0;
            }
            gOvl15MonitoredCard = gOvl15MonitoredCard == 0;
            return;
        }
        if ((u32) ((u16) gOvl15CardStates[gOvl15MonitoredCard] - 6) >= 2U) {
            gOvl15CardStates[gOvl15SelectedCard] = (s16) temp_v0;
        }
    } else {
        return;
    }
}

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void Ovl15CardReadEnter(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052FC4);
    Ovl15ResetSaveMenuState();
    gOvl15MenuState = 1;
}

s16 ovl_15_func_80135C20(s32 arg0);

void Ovl15CardReadPollCards(void) {
    s16 *var_s0;
    s32 var_s1;
    s16 var_v1;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        for (var_s1 = 0; var_s1 < 2; var_s1++) {
            var_s0 = &gOvl15CardStates[var_s1];
            var_v1 = ovl_15_func_80135C20((s16)var_s1);
            if (var_v1 == 1) {
                var_v1 = 0;
            }
            if (var_v1 == 7) {
                var_v1 = 6;
            }
            if (var_v1 != *var_s0) {
                *var_s0 = var_v1;
            }
        }
        if (gOvl15CardStates[0] == 4 && gOvl15CardStates[1] == gOvl15CardStates[0]) {
            gOvl15MenuState = 2;
        } else {
            gOvl15MenuState = 3;
        }
    }
}

void Ovl15CardReadPromptNoCard(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052FFA);
    gOvl15MenuState = Ovl15WaitForMenuButton(2, 0x11);
    gOvl15ConfirmationChoice = 1;
}

void Ovl15CardReadChooseCard(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053530);
    temp_v0 = Ovl15UpdateTwoChoiceMenu();
    if (temp_v0 < 2) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15ConfirmationChoice = 1;
                gOvl15MenuState = 0x11;
            }
        } else {
            gOvl15SelectedCard = temp_v0;
            gOvl15MenuState = 4;
        }
    }
}

void Ovl15CardReadCheckSelectedCard(void) {
    s16 temp_v0;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        temp_v0 = ovl_15_func_80135C20(gOvl15SelectedCard) - 4;
        switch (temp_v0) {
        case 0:
            if (gOvl15CardStates[gOvl15SelectedCard] == 0) {
                gOvl15MenuState = 0x10;
            } else {
                gOvl15MenuState = 5;
            }
            break;
        case 1:
            gOvl15MenuState = 4;
            break;
        case 4:
            gOvl15MenuState = 6;
            break;
        case 2:
        case 3:
            gOvl15MenuState = 8;
            break;
        default:
            gOvl15MenuState = 7;
            break;
        }
    }
}

s32 Ovl15CardReadPromptState05(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005306A);
    ret = Ovl15WaitForMenuButton(5, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardReadPromptState06(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800530C8);
    ret = Ovl15WaitForMenuButton(6, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 ovl_15_func_80136288(s16 arg0, s16 arg1, Ovl15SaveSummary *arg2);
s32 ovl_15_func_80135FF0(s16 arg0, s16 arg1);

void Ovl15CardReadScanSlots(void) {
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = gOvl15CardStates;
    for (slot = 0; slot < 3; slot++) {
        gOvl15SaveSlotStatus[slot] = 2;
        status = ovl_15_func_80136288(gOvl15SelectedCard, (s16) slot, (Ovl15SaveSummary *) &gOvl15SaveSlotHeaders[slot]);
        if (status == 8) {
            gOvl15MenuState = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            gOvl15MenuState = 0x10;
            return;
        }
        if (status == 9) {
            fileStatus = ovl_15_func_80135FF0(gOvl15SelectedCard, (s16) slot);
            if (fileStatus == 0) {
                gOvl15SaveSlotStatus[slot] = 1;
            } else if (fileStatus == 0xF) {
                gOvl15SaveSlotStatus[slot] = 3;
            } else {
                gOvl15SaveSlotStatus[slot] = 2;
            }
        } else if (status == 0) {
            gOvl15SaveSlotStatus[slot] = 0;
        }
        if ((u32) ((u16) cardStates[gOvl15SelectedCard] - 6) < 2U) {
            gOvl15SaveSlotStatus[slot] = 1;
        }
    }
    if (gOvl15SaveSlotStatus[0] != 0 && gOvl15SaveSlotStatus[1] != 0 && gOvl15SaveSlotStatus[2] != 0) {
        gOvl15MenuState = 8;
    } else {
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 9;
    }
}

s32 Ovl15CardReadPromptState08(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800532FE);
    ret = Ovl15WaitForMenuButton(8, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardReadChooseSaveSlot(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053596);
    temp_v0 = Ovl15UpdateSaveSlotMenu(0);
    if (temp_v0 < 3) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15SelectedCard = 2;
                gOvl15MenuState = 0;
            }
        } else {
            if (gOvl15SaveSlotStatus[temp_v0] != 0) {
                gOvl15MenuState = 0xA;
            } else {
                gOvl15SelectedSlot = (s16) temp_v0;
                gOvl15MenuState = 0xB;
            }
        }
    }
}

s32 Ovl15CardReadPromptState10(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800523AC);
    ret = Ovl15WaitForMenuButton(10, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardReadBuildFilename(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    Ovl15BuildCardFilename((s32)Ovl15CardFilename, gOvl15SelectedSlot);
    gOvl15MenuState = 12;
}

s32 ovl_15_func_801367F8(s16 arg0, s16 arg1, u32 *arg2);

void Ovl15CardReadReadSaveFile(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    temp_v0 = ovl_15_func_801367F8(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
    switch (temp_v0) {
    case 1:
    case 3:
        break;
    case 4:
        gOvl15MenuState = 0x10;
        break;
    case 2:
    case 5:
        gOvl15MenuState = 0xF;
        break;
    case 0:
        gOvl15MenuState = 0xD;
        break;
    }
}

void Ovl15CardReadVerifyChecksums(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    if (Ovl15VerifySaveChecksums() == -1) {
        gOvl15MenuState = 0xE;
    }
    else if (gOvl15SaveSlotHeaders[gOvl15SelectedSlot].field_34 != 0x14) {
        gOvl15MenuState = 0xE;
    }
    else {
        gOvl15MenuState = 0x13;
    }
}

s32 Ovl15CardReadPromptState14(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005341E);
    ret = Ovl15WaitForMenuButton(0xE, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardReadPromptState15(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800533F6);
    ret = Ovl15WaitForMenuButton(15, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardReadPromptState16(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052F4E);
    ret = Ovl15WaitForMenuButton(0x10, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardReadConfirmExit(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80051E02);
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 0:
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 0x12;
        return;
    case 1:
    case -1:
        gOvl15MenuState = 0;
        return;
    }
}

void Ovl15CardReadReset(void) {
    Ovl15ResetSaveMenuState();
    gOvl15MenuExitRequest = -1;
}

void Ovl15CardReadRestoreImage(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32)(D_800533C8 + gTextBankOffsets[0]));
    gOvl15MenuState = Ovl15WaitForMenuButton(0x13, 0);
    if (gOvl15MenuStateTimer >= 0x5B) {
        gOvl15MenuState = 0;
    }
    if (gOvl15MenuState != 0x13) {
        Ovl15RemoveSaveChecksumSlots();
        Ovl15RestoreSaveImage();
        Ovl15ResetSaveMenuState();
        gOvl15MenuExitRequest = 1;
    }
}

/* Sprite-assisted slot-transfer dispatcher and its complete stage set. */

void Ovl15CardTransferEnter (void);
void Ovl15CardTransferPollCards (void);
void Ovl15CardTransferPromptNoCard (void);
void Ovl15CardTransferChooseCard (void);
void Ovl15CardTransferCheckSelectedCard (void);
s32 Ovl15CardTransferPromptState05 (void);
s32 Ovl15CardTransferPromptState06 (void);
void Ovl15CardTransferScanSlots (void);
s32 Ovl15CardTransferPromptState08 (void);
void Ovl15CardTransferChooseSaveSlot (void);
s32 Ovl15CardTransferPromptState10 (void);
void Ovl15CardTransferBuildFilename (void);
void Ovl15CardTransferReadSaveFile (void);
void Ovl15CardTransferVerifyChecksums (void);
s32 Ovl15CardTransferPromptState14 (void);
s32 Ovl15CardTransferPromptState15 (void);
s32 Ovl15CardTransferPromptState30 (void);
void Ovl15CardTransferCompareHeader (void);
s32 Ovl15CardTransferPromptState17 (void);
void Ovl15CardTransferChooseFirstSlot (void);
void Ovl15CardTransferChooseSecondSlot (void);
void Ovl15CardTransferValidateCapacity (void);
void Ovl15CardTransferPromptState21 (void);
void Ovl15CardTransferPromptState22 (void);
void Ovl15CardTransferPromptState23 (void);
void Ovl15CardTransferPromptState24 (void);
void Ovl15CardTransferPromptState25 (void);
void Ovl15CardTransferConfirmSlots (void);
void Ovl15CardTransferShowSummary (void);
void Ovl15CardTransferWriteSaveFile (void);
s32 Ovl15CardTransferPromptState28 (void);
void Ovl15CardTransferConfirmExit (void);
void Ovl15CardTransferReset (void);
void Ovl15CardTransferFinish (void);
s32 ovl_15_func_80135E9C (s16 arg0);

void Ovl15StepCardTransferMenu(void) {
    s16 *cardStatus;
    s32 status;

    switch (gOvl15MenuState) {
    case 0:
        Ovl15CardTransferEnter();
        return;
    case 1:
        Ovl15CardTransferPollCards();
        return;
    case 2:
        Ovl15CardTransferPromptNoCard();
        break;
    case 3:
        Ovl15CardTransferChooseCard();
        break;
    case 4:
        Ovl15CardTransferCheckSelectedCard();
        return;
    case 5:
        Ovl15CardTransferPromptState05();
        break;
    case 6:
        Ovl15CardTransferPromptState06();
        break;
    case 7:
        Ovl15CardTransferScanSlots();
        return;
    case 8:
        Ovl15CardTransferPromptState08();
        break;
    case 9:
        Ovl15CardTransferChooseSaveSlot();
        break;
    case 10:
        Ovl15CardTransferPromptState10();
        break;
    case 11:
        Ovl15CardTransferBuildFilename();
        break;
    case 12:
        Ovl15CardTransferReadSaveFile();
        return;
    case 13:
        Ovl15CardTransferVerifyChecksums();
        break;
    case 14:
        Ovl15CardTransferPromptState14();
        break;
    case 15:
        Ovl15CardTransferPromptState15();
        break;
    case 30:
        Ovl15CardTransferPromptState30();
        break;
    case 16:
        Ovl15CardTransferCompareHeader();
        break;
    case 17:
        Ovl15CardTransferPromptState17();
        break;
    case 18:
        Ovl15CardTransferChooseFirstSlot();
        break;
    case 19:
        Ovl15CardTransferChooseSecondSlot();
        break;
    case 20:
        Ovl15CardTransferValidateCapacity();
        break;
    case 21:
        Ovl15CardTransferPromptState21();
        break;
    case 22:
        Ovl15CardTransferPromptState22();
        break;
    case 23:
        Ovl15CardTransferPromptState23();
        break;
    case 24:
        Ovl15CardTransferPromptState24();
        break;
    case 25:
        Ovl15CardTransferPromptState25();
        break;
    case 26:
        Ovl15CardTransferConfirmSlots();
        break;
    case 27:
        Ovl15CardTransferShowSummary();
        break;
    case 29:
        Ovl15CardTransferWriteSaveFile();
        return;
    case 28:
        Ovl15CardTransferPromptState28();
        break;
    case 31:
        Ovl15CardTransferConfirmExit();
        break;
    case 32:
        Ovl15CardTransferReset();
        return;
    case 33:
        Ovl15CardTransferFinish();
        return;
    default:
        break;
    }

    status = ovl_15_func_80135E9C(gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard);
    if (status != 3) {
        if (status != gOvl15CardStates[gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard]) {
            if (status == 5) {
                gOvl15MenuState = 0;
            }
            if (status == 4) {
                if (gOvl15SelectedCard == 2) {
                    gOvl15SelectedCard = (s16) (u16) gOvl15MonitoredCard;
                }
                gOvl15MenuState = 0x1E;
            }
        }
        if (gOvl15SelectedCard == 2) {
            cardStatus = &gOvl15CardStates[gOvl15MonitoredCard];
            if ((u32) ((u16) *cardStatus - 6) >= 2U) {
                *cardStatus = (s16) status;
            }
            gOvl15MonitoredCard = gOvl15MonitoredCard == 0;
            return;
        }
        /* The guard intentionally still indexes the monitored channel. */
        if ((u32) ((u16) gOvl15CardStates[gOvl15MonitoredCard] - 6) >= 2U) {
            gOvl15CardStates[gOvl15SelectedCard] = (s16) status;
        }
    } else {
        return;
    }
}

void ovl_15_func_8013468C(void);

void Ovl15CardTransferEnter(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052FC4);
    Ovl15ResetSaveMenuState();
    gOvl15MenuState = 1;
    ovl_15_func_8013468C();
}

void Ovl15CardTransferPollCards(void) {
    s16 *var_s0;
    s32 var_s1;
    s16 var_v1;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        for (var_s1 = 0; var_s1 < 2; var_s1++) {
            var_s0 = &gOvl15CardStates[var_s1];
            var_v1 = ovl_15_func_80135C20((s16)var_s1);
            if (var_v1 == 1) {
                var_v1 = 0;
            }
            if (var_v1 == 7) {
                var_v1 = 6;
            }
            if (var_v1 != *var_s0) {
                *var_s0 = var_v1;
            }
        }
        if (gOvl15CardStates[0] == 4 && gOvl15CardStates[1] == gOvl15CardStates[0]) {
            gOvl15MenuState = 2;
        } else {
            gOvl15MenuState = 3;
        }
    }
}

void Ovl15CardTransferPromptNoCard(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052FFA);
    gOvl15MenuState = Ovl15WaitForMenuButton(2, 0x1F);
    gOvl15ConfirmationChoice = 1;
}

void Ovl15CardTransferChooseCard(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053530);
    temp_v0 = Ovl15UpdateTwoChoiceMenu();
    if (temp_v0 < 2) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15ConfirmationChoice = 1;
                gOvl15MenuState = 0x1F;
            }
        } else {
            gOvl15SelectedCard = temp_v0;
            gOvl15MenuState = 4;
        }
    }
}

void Ovl15CardTransferCheckSelectedCard(void) {
    s32 text;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, (s32) &D_80052FC4[gTextBankOffsets[0]]);
    if (gOvl15MenuStateTimer >= 2) {
        switch (ovl_15_func_80135C20(gOvl15SelectedCard)) {
        case 4:
            if (gOvl15CardStates[gOvl15SelectedCard] == 0) {
                gOvl15MenuState = 0x1E;
            } else {
                gOvl15MenuState = 5;
            }
            break;
        case 8:
            gOvl15MenuState = 6;
            break;
        default:
            gOvl15MenuState = 7;
            break;
        }
    }
}

s32 Ovl15CardTransferPromptState05(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005306A);
    ret = Ovl15WaitForMenuButton(5, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardTransferPromptState06(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800530C8);
    ret = Ovl15WaitForMenuButton(6, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardTransferScanSlots(void) {
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    slot = 0;
    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = gOvl15CardStates;
    for (; slot < 3; slot++) {
        gOvl15SaveSlotStatus[slot] = 2;
        status = ovl_15_func_80136288(gOvl15SelectedCard, (s16) slot, (Ovl15SaveSummary *) &gOvl15SaveSlotHeaders[slot]);
        if (status == 8) {
            gOvl15MenuState = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            gOvl15MenuState = 0x1E;
            return;
        }
        if (status == 9) {
            fileStatus = ovl_15_func_80135FF0(gOvl15SelectedCard, (s16) slot);
            if (fileStatus == 0) {
                gOvl15SaveSlotStatus[slot] = 1;
            } else if (fileStatus == 0xF) {
                gOvl15SaveSlotStatus[slot] = 3;
            } else {
                gOvl15SaveSlotStatus[slot] = 2;
            }
        } else if (status == 0) {
            gOvl15SaveSlotStatus[slot] = 0;
        }
        if ((u32) ((u16) cardStates[gOvl15SelectedCard] - 6) < 2U) {
            gOvl15SaveSlotStatus[slot] = 1;
        }
    }
    if (gOvl15SaveSlotStatus[0] != 0 && gOvl15SaveSlotStatus[1] != 0 && gOvl15SaveSlotStatus[2] != 0) {
        gOvl15MenuState = 8;
    } else {
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 9;
    }
}

s32 Ovl15CardTransferPromptState08(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800532FE);
    ret = Ovl15WaitForMenuButton(8, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardTransferChooseSaveSlot(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80053596[*gTextBankOffsets]);
    temp_v0 = Ovl15UpdateSaveSlotMenu(0);
    if (temp_v0 < 3) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15SelectedCard = 2;
                gOvl15MenuState = 0;
            }
        } else {
            if (gOvl15SaveSlotStatus[temp_v0] != 0) {
                gOvl15MenuState = 0xA;
            } else {
                gOvl15SelectedSlot = (s16) temp_v0;
                gOvl15MenuState = 0xB;
            }
        }
    }
}

s32 Ovl15CardTransferPromptState10(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800523AC);
    ret = Ovl15WaitForMenuButton(10, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardTransferBuildFilename(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    Ovl15BuildCardFilename((s32)Ovl15CardFilename, gOvl15SelectedSlot);
    gOvl15MenuState = 12;
}

void Ovl15CardTransferReadSaveFile(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    temp_v0 = ovl_15_func_801367F8(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
    switch (temp_v0) {
    case 1:
    case 3:
        break;
    case 4:
        gOvl15MenuState = 0x1E;
        break;
    case 2:
    case 5:
        gOvl15MenuState = 0xF;
        break;
    case 0:
        gOvl15MenuState = 0xD;
        break;
    }
}

void Ovl15CardTransferVerifyChecksums(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    if (Ovl15VerifySaveChecksums() == -1) {
        gOvl15MenuState = 0xE;
    }
    else if (gOvl15SaveSlotHeaders[gOvl15SelectedSlot].field_34 != 0x14) {
        gOvl15MenuState = 0xE;
    }
    else {
        Ovl15RemoveSaveChecksumSlots();
        gOvl15MenuState = 0x10;
    }
}

s32 Ovl15CardTransferPromptState14(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005341E);
    ret = Ovl15WaitForMenuButton(0xE, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardTransferPromptState15(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800533F6);
    ret = Ovl15WaitForMenuButton(15, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardTransferPromptState30(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052F4E);
    ret = Ovl15WaitForMenuButton(0x1E, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardTransferCompareHeader(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    if (bcmp(gOvl15SaveWorkBase + 0x225C, (u8 *)D_8006C838, 8) == 0) {
        gOvl15MenuState = 0x11;
    } else {
        ovl_15_func_8013468C();
        gOvl15MenuState = 0x12;
    }
}

s32 Ovl15CardTransferPromptState17(void) {
    s8 *p;
    s32 ret;
    s32 val;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005204A);
    ret = Ovl15WaitForMenuButton(0x11, 9);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        val = 9;
        *p = val;
        return val;
    }
    return ret;
}

s32 ovl_15_func_80133808(s16 arg0);
void ovl_15_func_80134724(void);

void Ovl15CardTransferChooseFirstSlot(void) {
    s32 result;

    result = ovl_15_func_80133808(0);
    switch (result) {
    case 0:
        break;
    case 1:
        ovl_15_func_80134724();
        gOvl15MenuState = 0x13;
        break;
    case -1:
        gOvl15MenuState = 9;
        break;
    }
}

void Ovl15CardTransferChooseSecondSlot(void) {
    s32 temp_v0;

    temp_v0 = ovl_15_func_80133808(1);
    switch (temp_v0) {
    case 1:
        gOvl15MenuState = 0x14;
        break;
    case 0:
        break;
    case -1:
        ovl_15_func_80134724();
        gOvl15MenuState = 0x12;
        break;
    }
}

s16 ovl_15_func_80133538 (u8 *arg0, s16 arg1);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* Each side has five selected slots. Kind 0 and kinds 1/2 consume distinct
 * record pools; validate both resulting populations before allowing exchange.
 * Side 0 is live state, side 1 is the loaded save. */
void Ovl15CardTransferValidateCapacity(void) {
    s16 selectedCount[2];
    s16 selectedKind0[2];
    s16 selectedKind12[2];
    s16 kind0Count[2];
    s16 kind1Count[2];
    s16 kind0Capacity[2];
    s16 kind1Capacity[2];
    s16 liveKind0Limit;
    s16 saveKind0Limit;
    s16 liveKind1Limit;
    s16 saveKind1Limit;
    s32 *liveState;
    s32 text;
    s32 side;
    s32 slot;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(text + 0x18), text + 0x14, (s32) ((void *) ((u8 *) &D_800520A0 + *gTextBankOffsets)));
    for (side = 0; side < 2; side++) {
        selectedCount[side] = selectedKind0[side] = selectedKind12[side] = 0;
        for (slot = 0; slot < 5; slot++) {
            if (Ovl15Slots[side][slot].unk0 == 0) {
                selectedKind0[side] += 1;
            }
            if (Ovl15Slots[side][slot].unk0 == 1) {
                selectedKind12[side] += 1;
            }
            if (Ovl15Slots[side][slot].unk0 == 2) {
                selectedKind12[side] += 1;
            }
        }
        selectedCount[side] = selectedKind0[side] + selectedKind12[side];
    }
    kind0Count[0] = ovl_15_func_80133538((u8 *) &gSmallTaggedRecordBase, 0);
    kind0Count[1] = ovl_15_func_80133538(gOvl15SaveWorkBase, 0);
    kind1Count[0] = ovl_15_func_80133538((u8 *) &gSmallTaggedRecordBase, 1);
    liveState = &gSmallTaggedRecordBase - 0x1EAD;
    kind1Count[1] = ovl_15_func_80133538(gOvl15SaveWorkBase, 1);
    liveKind0Limit = 0xA;
    if ((*(u16 *) ((u8 *) liveState + 0x44D2)) == 0) {
        liveKind0Limit = 5;
    }
    kind0Capacity[0] = liveKind0Limit;
    saveKind0Limit = 0xA;
    if ((*(u16 *) ((u8 *) gOvl15SaveWorkBase + 0x236A)) == 0) {
        saveKind0Limit = 5;
    }
    liveKind1Limit = 0x14;
    kind0Capacity[1] = saveKind0Limit;
    if ((*(u16 *) ((u8 *) liveState + 0x44D0)) == 0) {
        liveKind1Limit = 0xA;
    }
    kind1Capacity[0] = liveKind1Limit;
    saveKind1Limit = 0x14;
    if ((*(u16 *) ((u8 *) gOvl15SaveWorkBase + 0x2368)) == 0) {
        saveKind1Limit = 0xA;
    }
    kind1Capacity[1] = saveKind1Limit;
    if ((selectedCount[0] == 0) && (selectedCount[1] == 0)) {
        gOvl15MenuState = 0x15;
        return;
    }
    if (kind0Capacity[0] < ((kind0Count[0] - selectedKind0[0]) + selectedKind0[1])) {
        gOvl15MenuState = 0x16;
        return;
    }
    if (kind0Capacity[1] < ((kind0Count[1] - selectedKind0[1]) + selectedKind0[0])) {
        gOvl15MenuState = 0x17;
        return;
    }
    if (kind1Capacity[0] < ((kind1Count[0] - selectedKind12[0]) + selectedKind12[1])) {
        gOvl15MenuState = 0x18;
        return;
    }
    if (kind1Capacity[1] < ((kind1Count[1] - selectedKind12[1]) + selectedKind12[0])) {
        gOvl15MenuState = 0x19;
        return;
    }
    gOvl15ConfirmationChoice = 1;
    gOvl15MenuState = 0x1A;
}

void Ovl15CardTransferPromptState21(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052150);
    *p = Ovl15WaitForMenuButton(0x15, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x15) {
        ovl_15_func_80134724();
    }
}

void Ovl15CardTransferPromptState22(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052178);
    *p = Ovl15WaitForMenuButton(0x16, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x16) {
        ovl_15_func_80134724();
    }
}

void Ovl15CardTransferPromptState23(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_800521F6);
    *p = Ovl15WaitForMenuButton(0x17, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x17) {
        ovl_15_func_80134724();
    }
}

void Ovl15CardTransferPromptState24(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052280);
    *p = Ovl15WaitForMenuButton(0x18, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x18) {
        ovl_15_func_80134724();
    }
}

void Ovl15CardTransferPromptState25(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_8005230A);
    *p = Ovl15WaitForMenuButton(0x19, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x19) {
        ovl_15_func_80134724();
    }
}

void ovl_15_func_801335F8(void);

void Ovl15CardTransferConfirmSlots(void) {
    s32 temp_a1;
    s8 temp_v0;

    ovl_15_func_801335F8();
    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)(D_80051EC6 + gTextBankOffsets[0]));
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 1:
    case -1:
        ovl_15_func_80134724();
        gOvl15MenuState = 0x13;
        return;
    case 0:
        gOvl15ConfirmationChoice = 1;
        gOvl15MenuState = 0x1B;
        return;
    }
}

void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void ovl_15_func_80132C1C(void);

void Ovl15CardTransferShowSummary(void) {
    s32 var_s0;
    s32 temp_a1;
    s8 temp_v0;

    var_s0 = 0;
    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)&D_80051FEA[gTextBankOffsets[0]]);
    do {
        Ovl15DrawSaveSlotSummary(var_s0, 0);
        var_s0 += 1;
    } while (var_s0 < 3);
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x39, (gOvl15MenuCursor * 0x31) + 0x18);
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 1:
    case -1:
        ovl_15_func_80134724();
        gOvl15ConfirmationChoice = 1;
        gOvl15MenuState = 0x1A;
        return;
    case 0:
        ovl_15_func_80132C1C();
        Ovl15InsertSaveChecksumSlots();
        Ovl15WriteSaveChecksums();
        gOvl15MenuState = 0x1D;
        return;
    }
}

s32 ovl_15_func_80136660(s16 arg0, s16 arg1, u32 *arg2);
void ovl_15_func_801331C4(void);

void Ovl15CardTransferWriteSaveFile(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_8005346A[*gTextBankOffsets]);
    temp_v0 = ovl_15_func_80136660(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
    switch (temp_v0) {
    case 1:
    case 3:
        break;
    case 2:
    case 5:
        gOvl15MenuState = 0x1C;
        break;
    case 4:
        gOvl15MenuState = 0x1E;
        break;
    case 0:
        ovl_15_func_801331C4();
        gOvl15MenuState = 0x21;
        break;
    }
}

s32 Ovl15CardTransferPromptState28(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80053506);
    ret = Ovl15WaitForMenuButton(0x1C, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardTransferConfirmExit(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80051E88);
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 0:
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 0x20;
        return;
    case 1:
    case -1:
        gOvl15MenuState = 0;
        return;
    }
}

void Ovl15CardTransferReset(void) {
    Ovl15ResetSaveMenuState();
    gOvl15MenuExitRequest = -1;
}

void Ovl15CardTransferFinish(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052022);
    *p = Ovl15WaitForMenuButton(0x21, 0);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    if (*p != 0x21) {
        Ovl15ResetSaveMenuState();
        gOvl15MenuExitRequest = 1;
    }
}
