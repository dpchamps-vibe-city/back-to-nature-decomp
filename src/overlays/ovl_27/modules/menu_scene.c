/*
 * menu_scene — reconstructed translation unit.
 *
 * Title menu controller, scene-exit phases and its closed self-installing
 * animation callback chain with private draw helpers. The signed menu-stage
 * halfword selects text entry, indexed selection and message-window stages;
 * animation callbacks independently raise/shrink the backdrop and open/blink/
 * close the panel. Animation/idle timers count update calls. The main timer
 * wraps modulo 108000; idle transitions use a threshold of 1800. Membership and the
 * evidence-limited pool edge and mechanical rodata limit on the setup side
 * are documented in notes/file-groupings.md.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "text_entry.h"
#include "ovl27_backdrop_sprite_pool.h"
#include "scene_selection.h"
#include "random.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "ovl27_menu_scene.h"
#include "ovl27_scene_setup.h"
#include "ovl27_work_area_init.h"

void Ovl27DrawTitleRecordSummary(void);
void Ovl27DrawTitleMenuBackdropFull(void);
void Ovl27DrawTitleMenuBackdropPreview(void);
void Ovl27DrawTitleMenuBackdrop(void);

void Ovl27UpdateTitleMenu(void) {
    s32 *base;

    UpdateMessageWindow();
    gOvl27TitleAnimationTicks = (gOvl27TitleAnimationTicks + 1) % 108000;
    switch (gOvl27TitleMenuStage) {
    case 0:
        ((void (*)(void))gOvl27TitleAnimationCallback)();
        BeginScreenFadeDecrease(10, 0, 2);
        gOvl27TitleMenuStage = 1;
        break;
    case 1:
        ((void (*)(void))gOvl27TitleAnimationCallback)();
        if (IsScreenFadeComplete() == 1) {
            gOvl27TitleIdleTicks = 0;
            gOvl27TitleMenuStage = 2;
        }
        break;
    case 2:
        gOvl27TitleIdleTicks++;
        ((void (*)(void))gOvl27TitleAnimationCallback)();
        if (((s32 *)gPad0Input)[2] & 0x840) {
            PlaySoundEffect(0);
            BeginScreenFadeIncrease(10, 0, 2);
            gOvl27TitleMenuStage = 3;
        } else if (gOvl27TitleIdleTicks >= 1800) {
            gOvl27TitleExitVariant = 1;
            gOvl27TitleMenuStage = 33;
        }
        break;
    case 3:
        ((void (*)(void))gOvl27TitleAnimationCallback)();
        if (IsScreenFadeComplete() == 1) {
            Ovl27ResetTitleAnimationState();
            gOvl27TitleMenuStage = 4;
        }
        break;
    case 4:
        Ovl27DrawTitleMenuBackdropPreview();
        BeginScreenFadeDecrease(10, 0, 2);
        gOvl27TitleMenuStage = 5;
        break;
    case 5:
        Ovl27DrawTitleMenuBackdropPreview();
        if (IsScreenFadeComplete() == 1) {
            gOvl27TitleAnimationTicks = 0;
            gOvl27TitleMenuStage = 6;
        }
        break;
    case 6:
        Ovl27DrawTitleMenuBackdropPreview();
        if (gOvl27TitleAnimationTicks >= 1800) {
            gOvl27TitleExitVariant = 1;
            gOvl27TitleMenuStage = 33;
        }
        if (((s32 *)gPad0Input)[2] & 0x800) {
            PlaySoundEffect(0);
            Ovl27ResetTitleAnimationState();
            gOvl27TitleMenuStage = 7;
        }
        break;
    case 7:
        Ovl27DrawTitleMenuBackdropFull();
        if (((s32 *)gPad0Input)[2] & 0x5100) {
            PlaySoundEffect(5);
            D_800C4A1A++;
            D_800C4A1A -= (D_800C4A1A / 2) * 2;
        } else if (((s32 *)gPad0Input)[2] & 0x840) {
            PlaySoundEffect(0);
            if (D_800C4A1A == 0) {
                gOvl27TitleMenuStage = 8;
            } else {
                BeginScreenFadeIncrease(10, 0, 2);
                BeginMasterVolumeFadeOut(10);
                gOvl27TitleMenuStage = 9;
            }
        }
        break;
    case 8:
        Ovl27DrawTitleMenuBackdropFull();
        gOvl27TitleMenuStage = 11;
        break;
    case 9:
        Ovl27DrawTitleMenuBackdropFull();
        if (IsScreenFadeComplete() == 1) {
            gOvl27TitleMenuStage = 10;
        }
        break;
    case 10:
        if (GetMasterVolumeRampState() == 0) {
            s32 *counterBase;
            s32 count;

            counterBase = (s32 *)&D_8006C838;
            count = counterBase[0x4488 / 4] + 1;
            gOvl27TitleMenuStage = 0;
            counterBase[0x4488 / 4] = count;
        }
        break;
    case 11:
        Ovl27DrawTitleMenuBackdrop();
        RequestMessageWindow(3, 0x3A2);
        gOvl27TitleMenuStage = 12;
        break;
    case 12:
        Ovl27DrawTitleMenuBackdrop();
        if (GetMessageWindowStatus() == 2) {
            gOvl27TitleMenuStage = 13;
        }
        break;
    case 13:
        Ovl27DrawTitleMenuBackdrop();
        ResetTextEntry();
        RequestMessageWindow(3, 0x397);
        gOvl27TitleMenuStage = 14;
        break;
    case 14:
        Ovl27DrawTitleMenuBackdrop();
        if (GetMessageWindowStatus() == 2) {
            BeginRequiredTextEntry((u16 *)&gPrimaryObjectBase);
            gOvl27TitleMenuStage = 15;
        }
        break;
    case 15:
        Ovl27DrawTextBankLabel(0);
        Ovl27DrawTitleMenuBackdrop();
        if (UpdateTextEntry() == 1) {
            gOvl27TitleMenuStage = 16;
        }
        break;
    case 16:
        Ovl27DrawTitleMenuBackdrop();
        ResetIndexedSelection();
        RequestMessageWindow(3, 0x3A5);
        gOvl27TitleMenuStage = 17;
        break;
    case 17:
        Ovl27DrawTitleMenuBackdrop();
        if (GetMessageWindowStatus() == 2) {
            BeginIndexedSelection((s32)D_80071A7C);
            gOvl27TitleMenuStage = 18;
        }
        break;
    case 18:
        Ovl27DrawTextBankLabel(1);
        Ovl27DrawTitleMenuBackdrop();
        switch (UpdateIndexedSelection()) {
        case -1:
            gOvl27TitleMenuStage = 13;
            break;
        case 1:
            gOvl27TitleMenuStage = 19;
            break;
        }
        break;
    case 19:
        Ovl27DrawTitleMenuBackdrop();
        ResetTextEntry();
        RequestMessageWindow(3, 0x3A4);
        gOvl27TitleMenuStage = 20;
        break;
    case 20:
        Ovl27DrawTitleMenuBackdrop();
        if (GetMessageWindowStatus() == 2) {
            BeginTextEntryAlternateLayout((u16 *)&D_80070D18);
            gOvl27TitleMenuStage = 21;
        }
        break;
    case 21:
        Ovl27DrawTextBankLabel(2);
        Ovl27DrawTitleMenuBackdrop();
        switch (UpdateTextEntry()) {
        case -1:
            gOvl27TitleMenuStage = 16;
            break;
        case 1:
            gOvl27TitleMenuStage = 22;
            break;
        }
        break;
    case 22:
        Ovl27DrawTitleMenuBackdrop();
        ResetTextEntry();
        RequestMessageWindow(3, 0x3A3);
        gOvl27TitleMenuStage = 23;
        break;
    case 23:
        Ovl27DrawTitleMenuBackdrop();
        if (GetMessageWindowStatus() == 2) {
            BeginTextEntry((u16 *)&D_800759E8);
            gOvl27TitleMenuStage = 24;
        }
        break;
    case 24:
        Ovl27DrawTextBankLabel(3);
        Ovl27DrawTitleMenuBackdrop();
        switch (UpdateTextEntry()) {
        case -1:
            gOvl27TitleMenuStage = 19;
            break;
        case 1:
            gOvl27TitleMenuStage = 25;
            break;
        }
        break;
    case 25:
        Ovl27DrawTitleMenuBackdrop();
        Ovl27DrawTitleRecordSummary();
        ResetTextEntry();
        RequestMessageWindow(3, 0x3AA);
        gOvl27TitleMenuStage = 26;
        break;
    case 26:
        Ovl27DrawTitleMenuBackdrop();
        Ovl27DrawTitleRecordSummary();
        if (GetMessageWindowStatus() == 2) {
            if ((s16)GetMessageWindowChoice() == 1) {
                BeginScreenFadeIncrease(10, 0, 2);
                BeginMasterVolumeFadeOut(10);
                gOvl27TitleMenuStage = 27;
            } else {
                gOvl27TitleMenuStage = 13;
            }
        }
        break;
    case 27:
        Ovl27DrawTitleMenuBackdrop();
        if (IsScreenFadeComplete() == 1) {
            gOvl27TitleMenuStage = 28;
        }
        break;
    case 28:
        if (GetMasterVolumeRampState() == 0) {
            s32 *counterBase;

            gOvl27TitleMenuStage = 0;
            counterBase = (s32 *)&D_8006C838;
            counterBase[3] |= 0x2000;
            counterBase[0x4488 / 4]++;
        }
        break;
    case 29:
        if (((s32 *)gPad0Input)[2] & 0x800) {
            Ovl27ResetTitleAnimationState();
            gOvl27TitleMenuStage = 4;
        }
        break;
    case 33:
        ((void (*)(void))gOvl27TitleAnimationCallback)();
        BeginScreenFadeIncrease(10, 0, 2);
        BeginMasterVolumeFadeOut(10);
        gOvl27TitleMenuStage = 34;
        break;
    case 34:
        ((void (*)(void))gOvl27TitleAnimationCallback)();
        if (IsScreenFadeComplete() == 1) {
            gOvl27TitleMenuStage = 35;
        }
        break;
    case 35:
        if (GetMasterVolumeRampState() == 0) {
            base = (s32 *)&D_8006C838;
            if (base[3] & 0x4000) {
                base[3] &= ~0x4000;
            }
            gOvl27TitleMenuStage = 0;
            base[0x4488 / 4]++;
        }
        break;
    }
}

s32 Ovl27BeginTitleExitFade(void) {
    s32 *base;
    s32 v;

    BeginScreenFadeIncrease(10, 0, 2);
    BeginMasterVolumeFadeOut(10);
    base = (s32 *)&D_8006C838;
    v = base[0x4488 >> 2] + 1;
    base[0x4488 >> 2] = v;
    return v;
}

s32 Ovl27PollTitleExitFade(void) {
    s32 *base;
    u8 x;
    s32 ret;

    x = (IsScreenFadeComplete() ^ 1) == 0;
    if (GetMasterVolumeRampState() == 0) {
        x = (x + 1) & 0xFF;
    }
    ret = 2;
    if (x == 2) {
        StopSoundSequence(0);
        OpenSoundSequencesAndStart();
        RewindSoundLoadSlots(2, 0);
        base = (s32 *)&D_8006C838;
        ret = base[0x4488 >> 2] + 1;
        base[0x4488 >> 2] = ret;
    }
    return ret;
}

void Ovl27FinishTitleMenu(void) {
    u8 *base;
    u8 *p;
    s32 i;

    base = (u8 *)&D_8006C838;
    *(s32 *)(base + 0x4450) &= -2;
    if (gOvl27TitleExitVariant != 0) {
        *(s32 *)(base + 0xC) |= 0x8000;
        *(s32 *)(*(u8 **)(base + 0x1C)) = 0;
        *(s32 *)(base + 0x4450) |= 1;
        Ovl27InitSpriteFrameRecords(3);
    }
    *(s32 *)(base + 0x4488) = 0;
    if (D_800C4A1A == 0) {
        if (*(s32 *)(base + 0xC) & 0x4000) {
            *(s32 *)(base + 0xC) &= ~0x4000;
        }
        p = base;
        for (i = 3; i >= 0; i--) {
            *(s16 *)p = Rand(0xFFFF);
            p += 2;
        }
        RequestSceneChange(2);
    } else {
        if (*(s32 *)(base + 0xC) & 0x8000) {
            Ovl27InitSpriteFrameRecords(3);
            RequestSceneChange(2);
        } else {
            *(s32 *)(base + 0xC) |= 0x4000;
            RequestSceneChange(8);
        }
    }
}

void Ovl27WaitTitleAnimation(void);

s32 Ovl27InstallTitleAnimationWait(void) {
    Ovl27DrawTitleMenuBackdrop();
    gOvl27TitleAnimationTicks = 0;
    gOvl27TitleAnimationCallback = (s32)Ovl27WaitTitleAnimation;
    return (s32)Ovl27WaitTitleAnimation;
}

void Ovl27RaiseTitleBackdrop(void);

void Ovl27WaitTitleAnimation(void) {
    Ovl27DrawTitleMenuBackdrop();
    if (gOvl27TitleAnimationTicks >= 0x1E) {
        gOvl27TitleAnimationCallback = (s32)Ovl27RaiseTitleBackdrop;
    }
}

void Ovl27ShrinkTitleBackdrop(void);

void Ovl27RaiseTitleBackdrop(void) {
    Ovl27DrawTitleMenuBackdrop();
    Ovl27DrawBackdropAnim31(gOvl27TitleBackdropY);
    gOvl27TitleBackdropY = gOvl27TitleBackdropY + 1;
    if (gOvl27TitleBackdropY >= 0) {
        gOvl27TitleAnimationCallback = (s32)Ovl27ShrinkTitleBackdrop;
    }
}

void Ovl27OpenTitlePanel(void);

void Ovl27ShrinkTitleBackdrop(void) {
    Ovl27DrawTitleMenuBackdrop();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(gOvl27TitleBackdropExtent);
    gOvl27TitleBackdropExtent = gOvl27TitleBackdropExtent - 1;
    if (gOvl27TitleBackdropExtent <= 0) {
        gOvl27TitleAnimationCallback = (s32)Ovl27OpenTitlePanel;
    }
}

void Ovl27BlinkTitlePanel(void);

void Ovl27OpenTitlePanel(void) {
    Ovl27DrawTitleMenuBackdrop();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    Ovl27DrawBackdropAnim33(gOvl27TitlePanelX);
    gOvl27TitlePanelX = gOvl27TitlePanelX - 2;
    if (gOvl27TitlePanelX <= 0) {
        gOvl27TitleAnimationTicks = 0;
        gOvl27TitleAnimationCallback = (s32)Ovl27BlinkTitlePanel;
    }
}

void Ovl27CloseTitlePanel(void);

void Ovl27BlinkTitlePanel(void) {
    Ovl27DrawTitleMenuBackdrop();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    if ((gOvl27TitleAnimationTicks % 30) < 0xF) {
        Ovl27DrawBackdropAnim33(0);
    }
    if (gOvl27TitleAnimationTicks >= 0x5A) {
        gOvl27TitleAnimationCallback = (s32)Ovl27CloseTitlePanel;
    }
}

s32 Ovl27RestartTitlePanelLoop(void);

void Ovl27CloseTitlePanel(void) {
    Ovl27DrawTitleMenuBackdrop();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    Ovl27DrawBackdropAnim33(gOvl27TitlePanelX);
    gOvl27TitlePanelX = gOvl27TitlePanelX - 2;
    if (gOvl27TitlePanelX <= -HWD0) {
        gOvl27TitleAnimationCallback = (s32)Ovl27RestartTitlePanelLoop;
    }
}

s32 Ovl27RestartTitlePanelLoop(void) {
    Ovl27DrawTitleMenuBackdrop();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    gOvl27TitlePanelX = 0x140;
    gOvl27TitleAnimationCallback = (s32)Ovl27OpenTitlePanel;
    return (s32)Ovl27OpenTitlePanel;
}

void func_80017A38 (s16 arg0, s16 arg1);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80024A10 (s32 arg0, s16 arg1, s16 arg2, s16 arg3);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);

void Ovl27DrawTitleRecordSummary(void) {
    s32 temp_s2;
    u8 *base;

    temp_s2 = gActiveRenderContext->field_D8 + 0x80;
    func_80017A38(0, 1);
    DrawMenuPanel((u32 *) (gActiveRenderContext->field_D8 + 0x84), 1, 0x48, 0x38, 0xB0, 0x5A);
    func_80017B3C(temp_s2, (s32) ((u8 *) D_80051AF6 + *gTextBankOffsets), 0x4C, 0x3C);
    func_80017A38(0, 0);
    base = (u8 *) D_8006C838;
    func_80024A10(temp_s2, 0xAC, 0x48, *(s16 *) (base + 0x5246));
    gOvl27TitleSummaryText = 0x71;
    *(u16 *) func_8001A970((*(s16 *) (base + 0x5248)) + 1, (&gOvl27TitleSummaryText + 1), 2) = 0xFFFF;
    func_80017B3C(temp_s2, (s32) &gOvl27TitleSummaryText, 0xBC, 0x48);
}

void Ovl27DrawTitleMenuBackdropFull(void) {
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    Ovl27DrawTiledBackdrop();
    Ovl27UpdateEffectSlot0();
    Ovl27UpdateEffectSlot1();
    Ovl27UpdateEffectSlot2();
    Ovl27UpdateEffectSlots3To5();
    Ovl27DrawBackdropAnim36();
    Ovl27DrawBackdropAnim34();
    Ovl27DrawBackdropAnim35();
}

void Ovl27DrawTitleMenuBackdropPreview(void) {
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    Ovl27DrawTiledBackdrop();
    Ovl27UpdateEffectSlot0();
    Ovl27UpdateEffectSlot1();
    Ovl27UpdateEffectSlot2();
    Ovl27UpdateEffectSlots3To5();
    Ovl27DrawBackdropAnim33(0);
}

void Ovl27DrawTitleMenuBackdrop(void) {
    Ovl27DrawTiledBackdrop();
    Ovl27UpdateEffectSlot0();
    Ovl27UpdateEffectSlot1();
    Ovl27UpdateEffectSlot2();
    Ovl27UpdateEffectSlots3To5();
}
