/*
 * save_menu_lifecycle — reconstructed translation unit.
 *
 * Memory-card menu initialization and the three scene lifecycle controllers;
 * see ovl15_save_menu_lifecycle.h for the state/timer and exit protocol.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl15_save_menu_lifecycle.h"
#include "psyq/libmcrd.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "aux_sprite_display.h"
#include "sprite_grid.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "ovl15_save_sprite_resources.h"
#include "ovl15_card_read_sprite_states.h"
#include "ovl15_card_read_states.h"

void ovl_15_func_8013468C(void);

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
    ovl_15_func_8013468C();
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

/* Shared text-token and pen-offset services. */
void func_80017A08(s32, s32);
void func_80017A38(s16, s16);
void func_8002495C(s16);
void ovl_15_func_8012EA18(void);

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
        func_8002495C(-1);
        state->state = 1;
    } else if (state->state == 1) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            state->state = 2;
        }
    } else if (state->state == 2) {
        gOvl15MenuExitRequest = 2;
        func_80017A08(0, (s32)&gOvl15SaveSlotLabels[gOvl15SelectedCard]);
        DrawScrollingSpriteGrid(sprite);
        func_8002495C(0);
        if (gOvl15MenuState != gOvl15LastMenuState) {
            gOvl15MenuStateTimer = 0;
            gOvl15LastMenuState = gOvl15MenuState;
        } else if (gOvl15MenuStateTimer < 0xFF) {
            gOvl15MenuStateTimer++;
        }
        func_80017A38(0, 2);
        ovl_15_func_8012EA18();
        func_80017A38(0, 0);
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
        func_8002495C(-1);
        state->state = 1;
    } else if (state->state == 1) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            state->state = 2;
        }
    } else if (state->state == 2) {
        gOvl15MenuExitRequest = 2;
        func_80017A08(0, (s32)&gOvl15SaveSlotLabels[gOvl15SelectedCard]);
        DrawScrollingSpriteGrid(sprite);
        func_8002495C(0);
        if (gOvl15MenuState != gOvl15LastMenuState) {
            gOvl15MenuStateTimer = 0;
            gOvl15LastMenuState = gOvl15MenuState;
        } else if (gOvl15MenuStateTimer < 0xFF) {
            gOvl15MenuStateTimer++;
        }
        func_80017A38(0, 2);
        Ovl15StepCardReadMenu();
        func_80017A38(0, 0);
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

void Ovl15UpdateCardReadSceneWithSprites(void) {
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
        func_8002495C(-1);
        state->state = 1;
    } else if (state->state == 1) {
        DrawScrollingSpriteGrid(sprite);
        if (IsScreenFadeComplete() == 1) {
            state->state = 2;
        }
    } else if (state->state == 2) {
        gOvl15MenuExitRequest = 2;
        func_80017A08(0, (s32)&gOvl15SaveSlotLabels[gOvl15SelectedCard]);
        DrawScrollingSpriteGrid(sprite);
        func_8002495C(0);
        if (gOvl15MenuState != gOvl15LastMenuState) {
            gOvl15MenuStateTimer = 0;
            gOvl15LastMenuState = gOvl15MenuState;
        } else if (gOvl15MenuStateTimer < 0xFF) {
            gOvl15MenuStateTimer++;
        }
        func_80017A38(0, 2);
        Ovl15StepCardTransferMenu();
        func_80017A38(0, 0);
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
