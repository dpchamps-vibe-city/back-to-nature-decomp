/*
 * save_menu — memory-card scene lifecycle and write/read/slot-transfer menus.
 *
 * Reset, the three scene controllers and all menu stages share one state
 * protocol. Controllers advance the saturating update timer and consume exit
 * requests; stages change the state, select/monitor cards and request exit.
 * Only one controller may run at a time. Card commands can remain pending
 * across updates; do not reset or reuse the transfer workspace during I/O.
 *
 * Write builds a checksummed save image with title/icons. Read validates and
 * restores that image. Transfer validates a source image, selects slot records
 * and optionally writes the edited result. Card I/O, image serialization,
 * reusable UI widgets and record-pool editing remain separate services.
 * Public controllers: ovl15_save_menu.h. Membership: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl15_save_menu.h"
#include "psyq/libmcrd.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "aux_sprite_display.h"
#include "sprite_grid.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "message_window.h"
#include "ovl15_save_checksums.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "ovl15_save_sprite_resources.h"
#include "ovl15_card_service.h"
#include "beveled_panel.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "sound_commands.h"
#include "ovl15_save_menu_ui.h"
#include "ovl15_save_card_title.h"
#include "ovl15_save_image.h"
#include "ovl15_card_filename.h"
#include "ovl15_save_slot_header.h"
#include "ovl15_save_slot_record_pool.h"
#include "ui_sprites.h"
#include "ovl15_save_slot_selection.h"
#include "psyq/libapi.h"
#include "text_engine.h"

/* Internal frame steps retain external linkage. They require the controller's
 * initialized menu/card state and timer; calling a stage directly bypasses
 * that protocol. Non-I/O stages monitor card changes, alternating ports when
 * selected-card is 2. I/O stages deliberately skip that monitoring tail.
 *
 * Write: card/slot selection, format/overwrite confirmation and image write.
 * Read: card/slot selection, checksum validation and persistent-image restore.
 * Transfer: read/validate, select/copy slot records, then optional image write.
 * Prompts draw through the active render context. Exit requests are 1 after
 * completion and -1 after cancellation; some prompts also expire at 91 timer
 * updates. The scene controller, not these steps, services fades and timing. */
void Ovl15StepCardWriteMenu(void);
void Ovl15StepCardReadMenu(void);
void Ovl15StepCardTransferMenu(void);

/* Reset waits for an outstanding card command, then invalidates every
 * summary and clears the shared transfer workspace. It is not reentrant. */
void Ovl15ResetSaveMenuState(void) {
    s32 cardCommand;
    s32 cardResult;
    s32 summaryIndex;
    u16 invalid;
    u8 *saveWork;
    u8 *summary;

    MemCardSync(0, &cardCommand, &cardResult);
    gOvl15MenuState = 0;
    gOvl15LastMenuState = 0;
    gOvl15MenuExitRequest = 0;
    gOvl15MenuCursor = 0;
    gOvl15ConfirmationChoice = 0;
    gOvl15CardRetryCount = 0;
    gOvl15MonitoredCard = 0;
    gOvl15SelectedCard = 2;
    gOvl15SelectedSlot = 0;
    D_80137590 = 0;
    gOvl15CardStates[0] = 4;
    gOvl15CardStates[1] = 4;
    gOvl15SaveSlotStatus[0] = 2;
    gOvl15SaveSlotStatus[1] = 2;
    gOvl15SaveSlotStatus[2] = 2;
    gOvl15MenuStateTimer = 0;
    gOvl15CardIconResourceBuffer = D_8007BFF8;
    memset((u8 *)gOvl15SaveSlotHeaders, 0, 0x3C);
    memset((u8 *)gOvl15SaveSlotHeaders + 0x3C, 0, 0x3C);
    memset((u8 *)gOvl15SaveSlotHeaders + 0x78, 0, 0x3C);
    memset(gOvl15TransferBuffer, 0, 0x9690);
    memset(D_801377A0, 0, 0x80);
    saveWork = gOvl15TransferBuffer + 0x280;
    Ovl15InitializeSlotSelection();
    gOvl15SaveWorkBase = saveWork;
    memset(saveWork, 0, 0x6378);
    summary = (u8 *)gOvl15SaveSlotHeaders;
    invalid = 0xFFFF;
    for (summaryIndex = 2; summaryIndex >= 0; summaryIndex--) {
        (*(u16 *) (summary + 8)) = invalid;
        (*(u16 *) (summary + 0x18)) = invalid;
        (*(u16 *) (summary + 0x1A)) = invalid;
        (*(u16 *) (summary + 0x2A)) = invalid;
        summary += 0x3C;
    }
}

void Ovl15UpdateCardWriteScene(void) {
    s32 sprite;
    Ovl21MainStateView *state;

    sprite = gActiveRenderContext->field_D8 + 0x2C;
    state = (Ovl21MainStateView *)D_8006C838;
    if (state->state == 0) {
        SetSoundSequenceSlotVolume(0, 0, 0, 0, 0);
        SsUtAllKeyOff(0);
        DisableAuxSpriteDisplay();
        ResetMessageWindow();
        Ovl15ResetSaveMenuState();
        gOvl15MenuState = 0;
        Ovl15LoadMenuResources(0);
        BeginScreenFadeDecrease(0x3C, 0, 2);
        BeginMasterVolumeFadeIn(0x3C);
        StepUiSpriteTwoFramePhase(-1);
        state->state = 1;
    } else if (state->state == 1) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            state->state = 2;
        }
    } else if (state->state == 2) {
        gOvl15MenuExitRequest = 2;
        SetTextSourcePointer(0, (s32)&gOvl15SaveSlotLabels[gOvl15SelectedCard]);
        DrawScrollingSpriteGrid(sprite);
        StepUiSpriteTwoFramePhase(0);
        if (gOvl15MenuState != gOvl15LastMenuState) {
            gOvl15MenuStateTimer = 0;
            gOvl15LastMenuState = gOvl15MenuState;
        } else if (gOvl15MenuStateTimer < 0xFF) {
            gOvl15MenuStateTimer++;
        }
        SetTextSpacing(0, 2);
        Ovl15StepCardWriteMenu();
        SetTextSpacing(0, 0);
        switch (gOvl15MenuExitRequest) {
        case 2:
            break;
        case -1:
            ((Ovl21MainStateView *)D_8006C838)->state = 4;
            BeginScreenFadeIncrease(0xA, 0, 2);
            BeginMasterVolumeFadeOut(0xA);
            break;
        case 1:
            ((Ovl21MainStateView *)D_8006C838)->state = 3;
            BeginScreenFadeIncrease(0xA, 0, 2);
            BeginMasterVolumeFadeOut(0xA);
            break;
        }
    } else if ((unsigned)(state->state - 3) < 2) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            state->state = state->state == 3 ? 3 : 4;
            RequestSceneChange(0xF);
            SetSoundSequenceSlotVolume(0, 0, 0, 0x50, 0xA);
        }
    }
}

void Ovl15UpdateCardReadScene(void) {
    s32 sprite;
    Ovl21MainStateView *state;

    sprite = gActiveRenderContext->field_D8 + 0x2C;
    state = (Ovl21MainStateView *)D_8006C838;
    if (state->state == 0) {
        SetSoundSequenceSlotVolume(0, 0, 0, 0, 0);
        SsUtAllKeyOff(0);
        DisableAuxSpriteDisplay();
        ResetMessageWindow();
        Ovl15ResetSaveMenuState();
        gOvl15MenuState = 0;
        Ovl15LoadMenuResources(0);
        BeginScreenFadeDecrease(0x3C, 0, 2);
        BeginMasterVolumeFadeIn(0x3C);
        StepUiSpriteTwoFramePhase(-1);
        state->state = 1;
    } else if (state->state == 1) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            state->state = 2;
        }
    } else if (state->state == 2) {
        gOvl15MenuExitRequest = 2;
        SetTextSourcePointer(0, (s32)&gOvl15SaveSlotLabels[gOvl15SelectedCard]);
        DrawScrollingSpriteGrid(sprite);
        StepUiSpriteTwoFramePhase(0);
        if (gOvl15MenuState != gOvl15LastMenuState) {
            gOvl15MenuStateTimer = 0;
            gOvl15LastMenuState = gOvl15MenuState;
        } else if (gOvl15MenuStateTimer < 0xFF) {
            gOvl15MenuStateTimer++;
        }
        SetTextSpacing(0, 2);
        Ovl15StepCardReadMenu();
        SetTextSpacing(0, 0);
        switch (gOvl15MenuExitRequest) {
        case 2:
            break;
        case -1:
            ((Ovl21MainStateView *)D_8006C838)->state = 4;
            BeginScreenFadeIncrease(0xA, 0, 2);
            BeginMasterVolumeFadeOut(0xA);
            break;
        case 1:
            ((Ovl21MainStateView *)D_8006C838)->state = 3;
            BeginScreenFadeIncrease(0xA, 0, 2);
            BeginMasterVolumeFadeOut(0xA);
            break;
        }
    } else if ((unsigned)(state->state - 3) < 2) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            SetSoundSequenceSlotVolume(0, 0, 0, 0x50, 0xA);
            state->state = state->state == 3 ? 3 : 4;
            if (state->state == 4) {
                if (state->flags & 0x4000) {
                    RequestSceneChange(0xD);
                } else {
                    RequestSceneChange(0x13);
                }
            } else {
                if (state->flags & 0x4000) {
                    state->flags &= ~0x4000;
                }
                RequestSceneChange(0xE);
            }
        }
    }
}

void Ovl15UpdateCardTransferScene(void) {
    s32 sprite;
    Ovl21MainStateView *state;

    sprite = gActiveRenderContext->field_D8 + 0x2C;
    state = (Ovl21MainStateView *)D_8006C838;
    if (state->state == 0) {
        SetSoundSequenceSlotVolume(0, 0, 0, 0, 0);
        SsUtAllKeyOff(0);
        DisableAuxSpriteDisplay();
        ResetMessageWindow();
        Ovl15ResetSaveMenuState();
        gOvl15MenuState = 0;
        Ovl15LoadMenuResources(1);
        BeginScreenFadeDecrease(0x3C, 0, 2);
        BeginMasterVolumeFadeIn(0x3C);
        StepUiSpriteTwoFramePhase(-1);
        state->state = 1;
    } else if (state->state == 1) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            state->state = 2;
        }
    } else if (state->state == 2) {
        gOvl15MenuExitRequest = 2;
        SetTextSourcePointer(0, (s32)&gOvl15SaveSlotLabels[gOvl15SelectedCard]);
        DrawScrollingSpriteGrid(sprite);
        StepUiSpriteTwoFramePhase(0);
        if (gOvl15MenuState != gOvl15LastMenuState) {
            gOvl15MenuStateTimer = 0;
            gOvl15LastMenuState = gOvl15MenuState;
        } else if (gOvl15MenuStateTimer < 0xFF) {
            gOvl15MenuStateTimer++;
        }
        SetTextSpacing(0, 2);
        Ovl15StepCardTransferMenu();
        SetTextSpacing(0, 0);
        switch (gOvl15MenuExitRequest) {
        case 2:
            break;
        case -1:
            ((Ovl21MainStateView *)D_8006C838)->state = 4;
            BeginScreenFadeIncrease(0xA, 0, 2);
            BeginMasterVolumeFadeOut(0xA);
            break;
        case 1:
            ((Ovl21MainStateView *)D_8006C838)->state = 3;
            BeginScreenFadeIncrease(0xA, 0, 2);
            BeginMasterVolumeFadeOut(0xA);
            break;
        }
    } else if ((unsigned)(state->state - 3) < 2) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            SetSoundSequenceSlotVolume(0, 0, 0, 0x50, 0xA);
            state->state = state->state == 3 ? 3 : 4;
            RequestSceneChange(0x13);
        }
    }
}

/*
 * Card write, read and slot-transfer menu workflows, in original link order.
 * Only the active workflow may be stepped: the three dispatchers share menu
 * state, selection, timers, card-status caching and prompt protocols. Read
 * restores the validated persistent image; transfer edits selected slot
 * records; write packs the image, header, checksum slots and three icon frames.
 * Low-level card I/O, serialization and slot-record editing remain services.
 *
 * PromptStateNN names leave undecoded prompt text neutral. Timed prompts keep
 * their byte-state pointer stores and original return types; dispatcher early
 * returns intentionally skip the common card-monitor tail during I/O stages.
 * Membership and remaining boundary evidence: notes/file-groupings.md.
 */

void Ovl15CardWriteEnter (void);
void Ovl15CardWritePollCards (void);
void Ovl15CardWritePromptNoCard (void);
void Ovl15CardWriteChooseCard (void);
void Ovl15CardWriteCheckSelectedCard (void);
s32 Ovl15CardWritePromptState05 (void);
s32 Ovl15CardWritePromptState06 (void);
void Ovl15CardWriteScanSlots (void);
void Ovl15CardWriteChooseSaveSlot (void);
void Ovl15CardWriteConfirmOverwrite (void);
void Ovl15CardWriteCheckSlotCapacity (void);
void Ovl15CardWriteConfirmFormat (void);
void Ovl15CardWriteFormatSelectedCard (void);
s32 Ovl15CardWritePromptState14 (void);
void Ovl15CardWriteCreateSaveFile (void);
s32 Ovl15CardWritePromptState16 (void);
s32 Ovl15CardWritePromptState17 (void);
s32 Ovl15CardWritePromptState18 (void);
void Ovl15CardWritePrepareSaveImage (void);
void Ovl15CardWriteWriteSaveFile (void);
s32 Ovl15CardWritePromptState21 (void);
void Ovl15CardWriteConfirmExit (void);
void Ovl15CardWriteReset (void);
void Ovl15CardWriteFinish (void);

void Ovl15StepCardWriteMenu(void) {
    s16 *temp_v0_2;
    s32 temp_v0;

    switch (gOvl15MenuState) {
    case 0:
        Ovl15CardWriteEnter();
        return;
    case 1:
        Ovl15CardWritePollCards();
        return;
    case 2:
        Ovl15CardWritePromptNoCard();
        break;
    case 3:
        Ovl15CardWriteChooseCard();
        break;
    case 4:
        Ovl15CardWriteCheckSelectedCard();
        return;
    case 5:
        Ovl15CardWritePromptState05();
        break;
    case 6:
        Ovl15CardWritePromptState06();
        break;
    case 7:
        Ovl15CardWriteScanSlots();
        return;
    case 8:
        Ovl15CardWriteChooseSaveSlot();
        break;
    case 9:
        Ovl15CardWriteConfirmOverwrite();
        break;
    case 10:
        Ovl15CardWriteCheckSlotCapacity();
        return;
    case 11:
        Ovl15CardWriteConfirmFormat();
        break;
    case 13:
        Ovl15CardWriteFormatSelectedCard();
        return;
    case 14:
        Ovl15CardWritePromptState14();
        break;
    case 15:
        Ovl15CardWriteCreateSaveFile();
        return;
    case 16:
        Ovl15CardWritePromptState16();
        break;
    case 17:
        Ovl15CardWritePromptState17();
        break;
    case 18:
        Ovl15CardWritePromptState18();
        break;
    case 19:
        Ovl15CardWritePrepareSaveImage();
        break;
    case 20:
        Ovl15CardWriteWriteSaveFile();
        return;
    case 21:
        Ovl15CardWritePromptState21();
        break;
    case 22:
        Ovl15CardWriteConfirmExit();
        break;
    case 23:
        Ovl15CardWriteReset();
        return;
    case 24:
        Ovl15CardWriteFinish();
        return;
    }
    temp_v0 = Ovl15PollCardPresence(gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard);
    if (temp_v0 != 3) {
        if (temp_v0 != gOvl15CardStates[gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard]) {
            if (temp_v0 == 5) {
                gOvl15MenuState = 0;
            }
            if (temp_v0 == 4) {
                if (gOvl15SelectedCard == 2) {
                    gOvl15SelectedCard = (s16) (u16) gOvl15MonitoredCard;
                }
                gOvl15MenuState = 0x15;
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
        if ((u32) ((u16) gOvl15CardStates[gOvl15SelectedCard] - 6) >= 2U) {
            gOvl15CardStates[gOvl15SelectedCard] = (s16) temp_v0;
        }
    } else {
        return;
    }
}

void Ovl15CardWriteEnter(void) {
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052FC4);
    Ovl15ResetSaveMenuState();
    gOvl15MenuState = 1;
}

void Ovl15CardWritePollCards(void) {
    register s16 *states;
    s32 i;
    s32 status;
    s32 text;

    text = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *)(text + 0x18), text + 0x14,
                 (s32) &D_80052FC4[gTextBankOffsets[0]]);
    if (gOvl15MenuStateTimer >= 2) {
        for (i = 0; i < 2; i++) {
            states = &gOvl15CardStates[i];
            status = Ovl15ProbeCard((s16)i);
            if (status == 1) {
                status = 0;
            }
            if (status == 7) {
                status = 6;
            }
            if (status != *states) {
                *states = status;
            }
        }
        states = gOvl15CardStates;
        if (gOvl15CardStates[0] == 4 && states[1] == 4) {
            gOvl15MenuState = 2;
        } else {
            gOvl15MenuState = 3;
        }
    }
}

void Ovl15CardWritePromptNoCard(void) {
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052FFA);
    gOvl15MenuState = Ovl15WaitForMenuButton(2, 0x16);
    gOvl15ConfirmationChoice = 1;
}

void Ovl15CardWriteChooseCard(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053530);
    temp_v0 = Ovl15UpdateTwoChoiceMenu();
    if (temp_v0 < 2) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15ConfirmationChoice = 1;
                gOvl15MenuState = 0x16;
            }
        } else {
            gOvl15SelectedCard = temp_v0;
            gOvl15MenuState = 4;
        }
    }
}

void Ovl15CardWriteCheckSelectedCard(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        switch (Ovl15ProbeCard(gOvl15SelectedCard)) {
        case 4:
            if (gOvl15CardStates[gOvl15SelectedCard] == 0) {
                gOvl15MenuState = 0x15;
            } else {
                gOvl15MenuState = 5;
            }
            break;
        case 5:
            gOvl15MenuState = 4;
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

s32 Ovl15CardWritePromptState05(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005306A);
    ret = Ovl15WaitForMenuButton(5, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardWritePromptState06(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800530C8);
    ret = Ovl15WaitForMenuButton(6, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardWriteScanSlots(void) {
    s16 *slotState;
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    text = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = gOvl15CardStates;
    slotState = gOvl15SaveSlotStatus;
    for (slot = 0; slot < 3; slot++, slotState++) {
        *slotState = 2;
        status = Ovl15ReadSaveSummary(gOvl15SelectedCard, (s16) slot, (Ovl15SaveSummary *) &gOvl15SaveSlotHeaders[slot]);
        if (status == 8) {
            gOvl15MenuState = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            gOvl15MenuState = 0x15;
            return;
        }
        if (status == 9) {
            fileStatus = Ovl15CheckSaveCapacity(gOvl15SelectedCard, (s16) slot);
            if (fileStatus == 0) {
                *slotState = 1;
            } else if (fileStatus == 0xF) {
                *slotState = 3;
            } else {
                *slotState = 2;
            }
        } else if (status == 0) {
            *slotState = 0;
        }
        if ((u32) ((u16) cardStates[gOvl15SelectedCard] - 6) < 2U) {
            *slotState = 1;
        }
    }
    gOvl15MenuCursor = 0;
    gOvl15MenuState = 8;
}

void Ovl15CardWriteChooseSaveSlot(void) {
    s16 temp_v1;
    s32 temp_a1;
    s32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32) D_80053566);
    temp_v0 = Ovl15UpdateSaveSlotMenu(1);
    if (temp_v0 < 3) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15SelectedCard = 2;
                gOvl15MenuState = 0;
            }
        } else {
            temp_v1 = gOvl15SaveSlotStatus[(s16) temp_v0];
            gOvl15SelectedSlot = (s16) temp_v0;
            if (temp_v1 == 0) {
                gOvl15ConfirmationChoice = 1;
                gOvl15MenuState = 9;
            } else if (temp_v1 == 1) {
                gOvl15MenuState = 0xA;
            } else if (temp_v1 == 2) {
                gOvl15MenuState = 0x11;
            } else if (temp_v1 == 3) {
                gOvl15MenuState = 0x12;
            }
        }
    }
}

void Ovl15CardWriteConfirmOverwrite(void) {
    s32 var_s0;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardOverwriteConfirmTextAnchor);
    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        Ovl15DrawSaveSlotSummary((s16)var_s0, 1);
    }
    DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 4, 0x39, (gOvl15MenuCursor * 0x31) + 0x18);
    switch (Ovl15UpdateConfirmationMenu()) {
    case 1:
    case -1:
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 8;
        break;
    case 0:
        gOvl15MenuState = 0x13;
        break;
    }
}

void Ovl15CardWriteCheckSlotCapacity(void) {
    s32 temp_a1;
    s16 status;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32) &D_80052FC4[gTextBankOffsets[0]]);
    if (gOvl15MenuStateTimer >= 2) {
        status = Ovl15ProbeCard(gOvl15SelectedCard);
        if (status == 1) {
            status = 0;
        }
        if (status == 7) {
            status = 6;
        }
        switch (status) {
        case 0:
            switch (Ovl15CheckSaveCapacity(gOvl15SelectedCard, gOvl15SelectedSlot)) {
            case 0:
                gOvl15MenuState = 0xF;
                return;
            case 4:
                switch (gOvl15CardStates[gOvl15SelectedCard]) {
                case 0:
                    gOvl15MenuState = 0x15;
                    break;
                default:
                    gOvl15MenuState = 5;
                    break;
                }
                return;
            case 11:
                gOvl15MenuState = 0x11;
                return;
            case 15:
                gOvl15MenuState = 0x12;
                return;
            case 8:
                gOvl15MenuState = 6;
                return;
            }
            break;
        case 5:
            gOvl15MenuState = 0;
            break;
        case 4:
            if (gOvl15CardStates[gOvl15SelectedCard] == 0) {
                gOvl15MenuState = 0x15;
            } else {
                gOvl15MenuState = 5;
            }
            break;
        case 8:
            gOvl15MenuState = 6;
            break;
        case 6:
            gOvl15ConfirmationChoice = 1;
            gOvl15FormatPromptStep = 0;
            gOvl15MenuState = 0xB;
            break;
        }
    }
}

void Ovl15CardWriteConfirmFormat(void) {
    s16 temp_s0;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_v0;

    temp_s0 = gOvl15FormatPromptStep;
    switch (temp_s0) {
    case 0:
        temp_a1 = gActiveRenderContext->field_D8;
        DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053114);
        if (*(s32 *)((u8 *)gPad0Input + 8) & 0x40) {
            PlaySoundEffect(0);
            gOvl15FormatPromptStep = 1;
        }
        break;
    case 1:
        temp_a1_2 = gActiveRenderContext->field_D8;
        DrawMenuPanelWithText((void *)(temp_a1_2 + 0x18), temp_a1_2 + 0x14, gTextBankOffsets[0] + (s32)gCardFormatConfirmTextAnchor);
        temp_v0 = Ovl15UpdateConfirmationMenu();
        if (temp_v0 != 0) {
            if (temp_v0 <= 0 ? temp_v0 == -1 : temp_v0 == temp_s0) {
                gOvl15MenuState = 0;
            }
        } else {
            gOvl15MenuState = 0xD;
        }
        break;
    }
}

void Ovl15CardWriteFormatSelectedCard(void) {
    s32 temp_a1;
    s32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32)(D_8005316C + gTextBankOffsets[0]));
    if (gOvl15MenuStateTimer >= 2) {
        temp_v0 = Ovl15FormatCard(gOvl15SelectedCard);
        if (temp_v0 == 0) {
            gOvl15MenuState = 0xF;
        } else if (temp_v0 == 4) {
            gOvl15MenuState = 0x15;
        } else {
            gOvl15MenuState = 0xE;
        }
    }
}

s32 Ovl15CardWritePromptState14(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800531B8);
    ret = Ovl15WaitForMenuButton(14, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardWriteCreateSaveFile(void) {
    s32 var_v0;

    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardSavingInProgressTextAnchor);
    if (gOvl15MenuStateTimer >= 2) {
        var_v0 = Ovl15CreateSaveFile(gOvl15SelectedCard, gOvl15SelectedSlot);
        if (var_v0 == 0) {
            gOvl15MenuState = 0x13;
        } else if (var_v0 == 0xB) {
            gOvl15MenuState = 0x11;
        } else if (var_v0 == 4) {
            gOvl15MenuState = 0x15;
        } else {
            gOvl15MenuState = 0x10;
        }
    }
}

s32 Ovl15CardWritePromptState16(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardSaveFailedTextAnchor);
    ret = Ovl15WaitForMenuButton(0x10, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardWritePromptState17(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005321A);
    ret = Ovl15WaitForMenuButton(0x11, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

s32 Ovl15CardWritePromptState18(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80053294);
    ret = Ovl15WaitForMenuButton(0x12, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardWritePrepareSaveImage(void) {
    s32 temp_a1;
    u32 *icons;
    u32 paletteOffset;

    temp_a1 = gActiveRenderContext->field_D8;
    icons = gOvl15CardIconFrameOffsets[gOvl15SelectedSlot];
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)(gCardSavingInProgressTextAnchor + gTextBankOffsets[0]));
    Ovl15BuildSaveSlotHeader(gOvl15SelectedSlot);
    Ovl15SerializeSaveImage();
    Ovl15InsertSaveChecksumSlots();
    Ovl15WriteSaveChecksums();
    Ovl15BuildCardFilename(Ovl15CardFilename, gOvl15SelectedSlot);
    /* The three-byte signature includes NUL; the following type store replaces it. */
    memcpy(&Ovl15CardHeader, "SC", 3);
    Ovl15CardHeader.type = 0x13;
    Ovl15CardHeader.blocks = 4;
    Ovl15BuildSaveCardTitle(gOvl15SelectedSlot);
    memset(Ovl15CardHeader.reserved, 0, sizeof(Ovl15CardHeader.reserved));
    paletteOffset = icons[0] + 0x14;
    Ovl15CardHeader.palette = *(Ovl15IconPalette *)(gOvl15CardIconResourceBuffer + paletteOffset);
    memcpy(gOvl15TransferBuffer, &Ovl15CardHeader, sizeof(Ovl15CardHeader));
    memcpy(gOvl15TransferBuffer + 0x80, (void *)(icons[0] + (u32)gOvl15CardIconResourceBuffer + 0x40), 0x80);
    memcpy(gOvl15TransferBuffer + 0x100, (void *)(icons[1] + (u32)gOvl15CardIconResourceBuffer + 0x40), 0x80);
    memcpy(gOvl15TransferBuffer + 0x180, (void *)(icons[2] + (u32)gOvl15CardIconResourceBuffer + 0x40), 0x80);
    gOvl15MenuState = 0x14;
}

void Ovl15CardWriteWriteSaveFile(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32) gCardSavingInProgressTextAnchor);
    temp_v0 = Ovl15WriteSaveFile(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
    switch (temp_v0) {
    case 1:
    case 3:
        break;
    case 2:
    case 5:
        gOvl15MenuState = 0x10;
        break;
    case 4:
        gOvl15MenuState = 0x15;
        break;
    case 0:
        gOvl15MenuState = 0x18;
        break;
    }
}

s32 Ovl15CardWritePromptState21(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052F4E);
    ret = Ovl15WaitForMenuButton(0x15, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardWriteConfirmExit(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardSaveAbortConfirmTextAnchor);
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 0:
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 0x17;
        return;
    case 1:
    case -1:
        gOvl15MenuState = 0;
        return;
    }
}

void Ovl15CardWriteReset(void) {
    Ovl15ResetSaveMenuState();
    gOvl15MenuExitRequest = -1;
}

void Ovl15CardWriteFinish(void) {
    s8 *p;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardSaveCompleteTextAnchor);
    *p = Ovl15WaitForMenuButton(0x18, 0);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    if (*p != 0x18) {
        Ovl15ResetSaveMenuState();
        gOvl15MenuExitRequest = 1;
    }
}

/*
 * Card-read and slot-transfer menu scenes: both dispatchers and all 54 stages,
 * in original order (ovl_15:801301A0–80132A8C exclusive). Their private prompt
 * copies implement different state numbering and exit/restore behavior; they
 * share the same card-monitor and menu context. Step only the active scene.
 * Their internal frame contracts are documented above the scene controllers.
 *
 * PromptStateNN names preserve undecoded text meaning rather than guessing it.
 * Distinct return types, timeout paths and conditional card monitoring remain
 * intentional. Card I/O, save serialization and slot editing are external
 * services, not replicated here.
 */

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

    temp_v0 = Ovl15PollCardPresence(gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard);
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

void Ovl15CardReadEnter(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052FC4);
    Ovl15ResetSaveMenuState();
    gOvl15MenuState = 1;
}

void Ovl15CardReadPollCards(void) {
    s16 *var_s0;
    s32 var_s1;
    s16 var_v1;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        for (var_s1 = 0; var_s1 < 2; var_s1++) {
            var_s0 = &gOvl15CardStates[var_s1];
            var_v1 = Ovl15ProbeCard((s16)var_s1);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052FFA);
    gOvl15MenuState = Ovl15WaitForMenuButton(2, 0x11);
    gOvl15ConfirmationChoice = 1;
}

void Ovl15CardReadChooseCard(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053530);
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
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        temp_v0 = Ovl15ProbeCard(gOvl15SelectedCard) - 4;
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005306A);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800530C8);
    ret = Ovl15WaitForMenuButton(6, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardReadScanSlots(void) {
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    text = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = gOvl15CardStates;
    for (slot = 0; slot < 3; slot++) {
        gOvl15SaveSlotStatus[slot] = 2;
        status = Ovl15ReadSaveSummary(gOvl15SelectedCard, (s16) slot, (Ovl15SaveSummary *) &gOvl15SaveSlotHeaders[slot]);
        if (status == 8) {
            gOvl15MenuState = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            gOvl15MenuState = 0x10;
            return;
        }
        if (status == 9) {
            fileStatus = Ovl15CheckSaveCapacity(gOvl15SelectedCard, (s16) slot);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800532FE);
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
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053596);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800523AC);
    ret = Ovl15WaitForMenuButton(10, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardReadBuildFilename(void) {
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadProgressTextAnchor);
    Ovl15BuildCardFilename((s32)Ovl15CardFilename, gOvl15SelectedSlot);
    gOvl15MenuState = 12;
}

void Ovl15CardReadReadSaveFile(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadProgressTextAnchor);
    temp_v0 = Ovl15ReadSaveFile(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
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
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadProgressTextAnchor);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardDiaryDamagedTextAnchor);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadFailedTextAnchor);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052F4E);
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
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardReadAbortConfirmTextAnchor);
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
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32)(gCardLoadCompleteTextAnchor + gTextBankOffsets[0]));
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

    status = Ovl15PollCardPresence(gOvl15SelectedCard != 2 ? gOvl15SelectedCard : gOvl15MonitoredCard);
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

void Ovl15CardTransferEnter(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052FC4);
    Ovl15ResetSaveMenuState();
    gOvl15MenuState = 1;
    Ovl15InitializeSlotSelection();
}

void Ovl15CardTransferPollCards(void) {
    s16 *var_s0;
    s32 var_s1;
    s16 var_v1;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        for (var_s1 = 0; var_s1 < 2; var_s1++) {
            var_s0 = &gOvl15CardStates[var_s1];
            var_v1 = Ovl15ProbeCard((s16)var_s1);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052FFA);
    gOvl15MenuState = Ovl15WaitForMenuButton(2, 0x1F);
    gOvl15ConfirmationChoice = 1;
}

void Ovl15CardTransferChooseCard(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053530);
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
    DrawMenuPanelWithText((u32 *) (text + 0x18), text + 0x14, (s32) &D_80052FC4[gTextBankOffsets[0]]);
    if (gOvl15MenuStateTimer >= 2) {
        switch (Ovl15ProbeCard(gOvl15SelectedCard)) {
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005306A);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800530C8);
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
    DrawMenuPanelWithText((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = gOvl15CardStates;
    for (; slot < 3; slot++) {
        gOvl15SaveSlotStatus[slot] = 2;
        status = Ovl15ReadSaveSummary(gOvl15SelectedCard, (s16) slot, (Ovl15SaveSummary *) &gOvl15SaveSlotHeaders[slot]);
        if (status == 8) {
            gOvl15MenuState = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            gOvl15MenuState = 0x1E;
            return;
        }
        if (status == 9) {
            fileStatus = Ovl15CheckSaveCapacity(gOvl15SelectedCard, (s16) slot);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800532FE);
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
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80053596[*gTextBankOffsets]);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800523AC);
    ret = Ovl15WaitForMenuButton(10, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardTransferBuildFilename(void) {
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadProgressTextAnchor);
    Ovl15BuildCardFilename((s32)Ovl15CardFilename, gOvl15SelectedSlot);
    gOvl15MenuState = 12;
}

void Ovl15CardTransferReadSaveFile(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadProgressTextAnchor);
    temp_v0 = Ovl15ReadSaveFile(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
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
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadProgressTextAnchor);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardDiaryDamagedTextAnchor);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadFailedTextAnchor);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052F4E);
    ret = Ovl15WaitForMenuButton(0x1E, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}

void Ovl15CardTransferCompareHeader(void) {
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardLoadProgressTextAnchor);
    if (bcmp(gOvl15SaveWorkBase + 0x225C, (u8 *)D_8006C838, 8) == 0) {
        gOvl15MenuState = 0x11;
    } else {
        Ovl15InitializeSlotSelection();
        gOvl15MenuState = 0x12;
    }
}

s32 Ovl15CardTransferPromptState17(void) {
    s8 *p;
    s32 ret;
    s32 val;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005204A);
    ret = Ovl15WaitForMenuButton(0x11, 9);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        val = 9;
        *p = val;
        return val;
    }
    return ret;
}

void Ovl15CardTransferChooseFirstSlot(void) {
    s32 result;

    result = Ovl15UpdateSlotSelectionMenu(0);
    switch (result) {
    case 0:
        break;
    case 1:
        Ovl15ResetSlotSelection();
        gOvl15MenuState = 0x13;
        break;
    case -1:
        gOvl15MenuState = 9;
        break;
    }
}

void Ovl15CardTransferChooseSecondSlot(void) {
    s32 temp_v0;

    temp_v0 = Ovl15UpdateSlotSelectionMenu(1);
    switch (temp_v0) {
    case 1:
        gOvl15MenuState = 0x14;
        break;
    case 0:
        break;
    case -1:
        Ovl15ResetSlotSelection();
        gOvl15MenuState = 0x12;
        break;
    }
}

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
    DrawMenuPanelWithText((u32 *)(text + 0x18), text + 0x14, (s32) ((void *) ((u8 *) &gLivestockTransferInstructionsTextAnchor + *gTextBankOffsets)));
    for (side = 0; side < 2; side++) {
        selectedCount[side] = selectedKind0[side] = selectedKind12[side] = 0;
        for (slot = 0; slot < 5; slot++) {
            if (Ovl15Slots[side][slot].kind == 0) {
                selectedKind0[side] += 1;
            }
            if (Ovl15Slots[side][slot].kind == 1) {
                selectedKind12[side] += 1;
            }
            if (Ovl15Slots[side][slot].kind == 2) {
                selectedKind12[side] += 1;
            }
        }
        selectedCount[side] = selectedKind0[side] + selectedKind12[side];
    }
    kind0Count[0] = Ovl15CountOccupiedPoolUnits((u8 *) &gSmallTaggedRecordBase, 0);
    kind0Count[1] = Ovl15CountOccupiedPoolUnits(gOvl15SaveWorkBase, 0);
    kind1Count[0] = Ovl15CountOccupiedPoolUnits((u8 *) &gSmallTaggedRecordBase, 1);
    liveState = &gSmallTaggedRecordBase - 0x1EAD;
    kind1Count[1] = Ovl15CountOccupiedPoolUnits(gOvl15SaveWorkBase, 1);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052150);
    *p = Ovl15WaitForMenuButton(0x15, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x15) {
        Ovl15ResetSlotSelection();
    }
}

void Ovl15CardTransferPromptState22(void) {
    s8 *p;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052178);
    *p = Ovl15WaitForMenuButton(0x16, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x16) {
        Ovl15ResetSlotSelection();
    }
}

void Ovl15CardTransferPromptState23(void) {
    s8 *p;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_800521F6);
    *p = Ovl15WaitForMenuButton(0x17, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x17) {
        Ovl15ResetSlotSelection();
    }
}

void Ovl15CardTransferPromptState24(void) {
    s8 *p;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052280);
    *p = Ovl15WaitForMenuButton(0x18, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x18) {
        Ovl15ResetSlotSelection();
    }
}

void Ovl15CardTransferPromptState25(void) {
    s8 *p;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_8005230A);
    *p = Ovl15WaitForMenuButton(0x19, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x19) {
        Ovl15ResetSlotSelection();
    }
}

void Ovl15CardTransferConfirmSlots(void) {
    s32 temp_a1;
    s8 temp_v0;

    Ovl15DrawSlotBanks();
    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)(gCardTransferLivestockConfirmTextAnchor + gTextBankOffsets[0]));
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 1:
    case -1:
        Ovl15ResetSlotSelection();
        gOvl15MenuState = 0x13;
        return;
    case 0:
        gOvl15ConfirmationChoice = 1;
        gOvl15MenuState = 0x1B;
        return;
    }
}

void Ovl15CardTransferShowSummary(void) {
    s32 var_s0;
    s32 temp_a1;
    s8 temp_v0;

    var_s0 = 0;
    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)&gCardOverwriteConfirmTextAnchor[gTextBankOffsets[0]]);
    do {
        Ovl15DrawSaveSlotSummary(var_s0, 0);
        var_s0 += 1;
    } while (var_s0 < 3);
    DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 4, 0x39, (gOvl15MenuCursor * 0x31) + 0x18);
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 1:
    case -1:
        Ovl15ResetSlotSelection();
        gOvl15ConfirmationChoice = 1;
        gOvl15MenuState = 0x1A;
        return;
    case 0:
        Ovl15StageSelectedSlotTransfers();
        Ovl15InsertSaveChecksumSlots();
        Ovl15WriteSaveChecksums();
        gOvl15MenuState = 0x1D;
        return;
    }
}

void Ovl15CardTransferWriteSaveFile(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    DrawMenuPanelWithText((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, (s32) &gCardSavingInProgressTextAnchor[*gTextBankOffsets]);
    temp_v0 = Ovl15WriteSaveFile(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
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
        Ovl15RestoreStagedSlotRecords();
        gOvl15MenuState = 0x21;
        break;
    }
}

s32 Ovl15CardTransferPromptState28(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)gCardSaveFailedTextAnchor);
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
    DrawMenuPanelWithText((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)gCardTransferAbortConfirmTextAnchor);
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
    DrawMenuPanelWithText((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
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
