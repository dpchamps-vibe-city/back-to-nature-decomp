/*
 * record_scene — reconstructed translation unit.
 *
 * Horse-race scene: private phase and state dispatch, entrant simulation,
 * betting/results, queue, resource lifecycle and presentation.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl17_record_scene.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "pad_actuator_sequences.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl17_record_work_init.h"
#include "ovl17_sprite_resources.h"
#include "ovl17_record_sprite_animation.h"
#include "ovl17_record_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl17_record_value_increments.h"
#include "ovl17_record_backdrop.h"
#include "ovl17_timer_hud.h"
#include "ovl17_display_sprite_frame.h"
#include "ovl17_record_threshold_state.h"
#include "ovl17_record_decision_tests.h"
#include "random.h"
#include "ui_sprites.h"
#include "psyq/libapi.h"
#include "text_engine.h"

/* The original word-table ABI forwards the table address in a0. The five
 * lifecycle callbacks do not consume it; keep this calling convention. */

void Ovl17DispatchRecordSceneSubphase(void) {
    s32 *base = gOvl17RecordSceneSubphases;

    ((void (*)(s32 *))base[gSceneSubphase])(base);
}

void Ovl17UpdateRecordSceneSetup(void) {
    switch (gOvl17SceneLoadStep) {
    case 0:
        Ovl17LoadSharedSpriteBank();
        gOvl17SceneLoadStep++;
        break;
    case 1:
        Ovl17LoadSpriteImageBank();
        gOvl17SceneLoadStep++;
        break;
    case 2:
        Ovl17LoadRecordSpriteBank();
        gOvl17SceneLoadStep++;
        break;
    case 3:
        Ovl17QueueSoundBankLoads();
        gOvl17SceneLoadStep++;
        break;
    case 4:
        if (IsSoundTransferComplete() != 0) {
            gOvl17SceneLoadStep = 5;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x15, 0x7F);
        }
        break;
    }
    if (gOvl17SceneLoadStep == 5) {
        gOvl17RecordSceneStageIndex = 0;
        Ovl17InitRecordWork();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        gOvl17SceneLoadStep = 0;
        ((Ovl17CompletionView *)D_8006C838)->state++;
    }
}

void Ovl17EnterRecordScene(void) {
    s32 var_s0;
    s32 var_s0_2;
    u8 *var_s1;

    Ovl17DrawRecordBackdrop(0x40000);
    Ovl17SelectAllRecordAnimations2();
    var_s1 = gOvl17AnimatedRecordBase;
    for (var_s0 = 0; var_s0 < 6; var_s0++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *) var_s1, 0x40000);
        var_s1 += 0x50;
    }
    var_s0_2 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0_2 = (var_s0_2 + 1) & 0xFF;
    }
    if (var_s0_2 == 2) {
        ((Ovl21MainStateView *) D_8006C838)->state++;
    }
}

void Ovl17DispatchRecordSceneStage(void) {
    UpdateMessageWindow();
    gOvl17RecordSceneStages[gOvl17RecordSceneStageIndex]();
    Ovl17UpdateTimerHud(1);
}

s32 Ovl17BeginRecordSceneExitFade(void) {
    s32 *base;
    s32 v;

    BeginScreenFadeFeedback(10);
    BeginMasterVolumeFadeOut(10);
    base = (s32 *)&D_8006C838;
    v = base[0x448C >> 2] + 1;
    base[0x448C >> 2] = v;
    return v;
}

void SsSeqClose (short);
void SsSeqSetVol (short, short, short);
void SsVabClose (short);

void Ovl17FinishRecordSceneExit(void) {
    u8 var_s0 = 0;

    Ovl17DrawRecordBackdrop(0x800000);
    if (IsScreenFadeComplete() == 1) {
        var_s0++;
    }
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        if (gOvl17SoundEffectVoice != -1) {
            StopSoundEffectVoice((s32) gOvl17SoundEffectVoice);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        if (((Ovl17CompletionView *) D_8006C838)->mode == 1) {
            Ovl17RaiseRecordByteRangeValue(1);
            Ovl17RaiseRecordByteRangeValue(2);
            Ovl17RaiseRecordByteRangeValue(4);
            Ovl17RaiseRecordByteRangeValue(6);
            Ovl17RaiseRecordByteRangeValue(0xF);
            Ovl17RaiseRecordByteRangeValue(0x10);
            Ovl17RaiseRecordByteRangeValue(0x11);
            Ovl17RaiseRecordByteRangeValue(0x13);
            Ovl17RaiseRecordByteRangeValue(0x15);
            Ovl17RaiseRecordByteRangeValue(0x16);
            Ovl17RaiseRecordByteRangeValue(0x1A);
            Ovl17RaiseRecordByteRangeValue(0x1C);
            Ovl17RaiseRecordHalfwordRangeValue(3);
            Ovl17RaiseRecordHalfwordRangeValue(7);
            Ovl17RaiseRecordHalfwordRangeValue(0x14);
        }
        ((Ovl17CompletionView *) D_8006C838)->state = 0xFF;
        ((Ovl17CompletionView *) D_8006C838)->flags |= 0x80000;
        RequestSceneChange(6);
    }
}

void Ovl17InitRecordStageRows (void);

void Ovl17EnterRecordSceneStages(void) {
    s32 var_s0;
    u8 *var_s1;

    Ovl17DrawRecordBackdrop(0x40000);
    Ovl17SelectAllRecordAnimations2();
    var_s1 = gOvl17AnimatedRecordBase;
    for (var_s0 = 0; var_s0 < 6; var_s0++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *) var_s1, 0x40000);
        var_s1 += 0x50;
    }
    Ovl17InitRecordStageRows();
    if (((Ovl17ModeView *) D_8006C838)->mode == 3) {
        OVL17_RECORD_WORK.phase = 2;
    } else {
        OVL17_RECORD_WORK.phase = 1;
    }
}

void Ovl17ResetRecordTrails(void);

void Ovl17InitRecordStageRows(void) {
    s16 temp_v0;
    Ovl11HorseRaceView *work;
    s16 *var_a2;
    s16 *entries;
    s16 var_a1;
    s32 var_a3;
    s32 offset;
    u8 *var_a0;
    u8 *var_a1_3;
    s32 *names;

    Ovl17ResetHorseRaceEntrants();
    work = (Ovl11HorseRaceView *)D_8006C838;
    var_a1 = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        var_a1 = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    names = gOvl17RecordRowNameOffsets;
    offset = var_a1 * 0x54;
    entries = (s16 *)((u8 *)work + 0xE516);
    var_a2 = (s16 *)((u8 *)entries + offset);
    var_a0 = gOvl17RowSlotTargets;
    for (var_a3 = 0; var_a3 < 6; var_a3++) {
        temp_v0 = *var_a2;
        var_a2 += 7;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[var_a3].data = var_a0;
        var_a0 += 6;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[var_a3].flags = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[var_a3].nameOffset = names[temp_v0];
    }
    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        var_a1_3 = gOvl17SpriteHeaderVariantA;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].flags |= 1;
    } else {
        var_a1_3 = gOvl17SpriteHeaderVariantB;
    }
    InitSpriteSourceData((SpriteSourceData *)D_800BD930, (SpriteDataHeader *) var_a1_3);
    (*(s16 *) (D_800BD930 + 0x1E4)) = -1;
    Ovl17ResetRecordTrails();
}

void Ovl17ShowRecordRowNamesPage3(void) {
    s32 i = 0;
    u8 *base;
    s32 data = (s32)D_800556A8;
    u8 *work;
    u8 *state;

    do {
        base = gOvl17RecordWorkBase;
        SetTextSourcePointer((u16)i, *(u16 *)(base + i * 0x50 + 0x40) + data);
        i++;
    } while (i < 6);
    Ovl17SelectAllRecordAnimations2();
    work = gOvl17AnimatedRecordBase;
    for (i = 0; i < 6; i++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)work, 0x40000);
        work += 0x50;
    }
    Ovl17DrawRecordBackdrop(0x40000);
    RequestMessageWindow(4, 3);
    if (GetMessageWindowStatus() == 2) {
        state = gOvl17RecordWorkBase;
        *(s16 *)(state + 0x2CE) = 0;
        state[4] = i;
    }
}

void Ovl17ShowRecordScrollBackdropPage0(void) {
    s32 i;
    u8 *base;
    u8 *work;

    gOvl17BackdropScrollTick++;
    if (gOvl17BackdropScrollTick >= 30) {
        gOvl17BackdropScrollTick = 0;
        work = gOvl17RecordWorkBase;
        *(s16 *)(work + 0xCA) -= 100;
        if (*(s16 *)(work + 0xCA) < 0) {
            *(s16 *)(work + 0xCA) = 255;
        }
    }
    Ovl17SelectAllRecordAnimations2();
    base = gOvl17AnimatedRecordBase;
    for (i = 0; i < 6; i++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        base += 0x50;
    }
    Ovl17DrawRecordBackdrop(0x40000);
    RequestMessageWindow(4, 0);
    if (GetMessageWindowStatus() == 2) {
        gOvl17RecordSceneStageIndex = 3;
    }
}

void Ovl17ShowRecordChoicePage1(void) {
    s32 i;
    s32 state;
    s32 result;
    s32 skip;
    u8 *base;
    u8 *record;
    u8 *origin;

    i = 0;
    skip = 2;
    record = gOvl17AnimatedRecordBase;
    origin = record;
    for (; i < 6; i++) {
        if (i == skip) {
            record = origin + 0xF0;
            i = 3;
        }
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)record, 0x40000);
        record += 0x50;
    }
    base = D_800BD910;
    Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
    Ovl17DrawRecordBackdrop(0x40000);
    RequestMessageWindow(4, 1);
    state = GetMessageWindowStatus();
    if (state == 2) {
        result = GetMessageWindowChoice();
        if (result == 1) {
            base[-0xC4] = 4;
        } else if (result == state) {
            Ovl17ResetHorseRaceEntrants();
            base[-0xC4] = result;
        }
    }
}

void Ovl17ShowRecordConfirmPage2(void) {
    s32 i;
    u8 *base;
    u8 *record;

    for (i = 0; i < 6; i++) {
        base = gOvl17AnimatedRecordBase + i * 0x50;
        if (i != 2) {
            Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        }
    }
    record = D_800BD910;
    Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)record, 0x40000);
    Ovl17DrawRecordBackdrop(0x40000);
    RequestMessageWindow(4, 2);
    if (GetMessageWindowStatus() == 2) {
        Ovl17ResetHorseRaceEntrants();
        record[-0xC4] = 5;
    }
}

void Ovl17ShowRecordRowNamesPage4(void) {
    s32 i = 0;
    u8 *base;
    s32 data = (s32)D_800556A8;
    u8 *work;
    u8 *state;

    do {
        base = gOvl17RecordWorkBase;
        SetTextSourcePointer((u16)i, *(u16 *)(base + i * 0x50 + 0x40) + data);
        i++;
    } while (i < 6);
    Ovl17SelectAllRecordAnimations2();
    work = gOvl17AnimatedRecordBase;
    for (i = 0; i < 6; i++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)work, 0x40000);
        work += 0x50;
    }
    Ovl17DrawRecordBackdrop(0x40000);
    RequestMessageWindow(4, 4);
    if (GetMessageWindowStatus() == 2) {
        state = gOvl17RecordWorkBase;
        *(s16 *)(state + 0x2CE) = 0;
        state[4] = i;
    }
}

/* Original overlay imports for SetVal8005E2BC and SetMessagePortraitEnabled. */
s16 Ovl17EvaluateRecordValueCurve(s16 arg0);

void Ovl17SeedRecordRowValuesPage5(void) {
    s32 i;
    u8 *base;
    u8 *work;

    Ovl17SelectAllRecordAnimations2();
    base = gOvl17AnimatedRecordBase;
    for (i = 0; i < 6; i++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        base += 0x50;
    }
    Ovl17DrawRecordBackdrop(0x40000);
    SetTextInputEnabled(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 5);
    if (GetMessageWindowStatus() == 5) {
        u8 *counter = gOvl17RecordWorkBase;
        (*(s16 *)(counter + 0x2CE))++;
        if (*(s16 *)(counter + 0x2CE) >= 60) {
            for (i = 0; i < 6; i++) {
                base = gOvl17RecordWorkBase;
                *(s16 *)(base + i * 0x50 + 0x2E) = *(s16 *)(base + i * 0x50 + 0x2C) =
                    Ovl17EvaluateRecordValueCurve(*(s16 *)(base + i * 0x50 + 0x34));
            }
            work = gOvl17RecordWorkBase;
            CloseMessageWindow();
            *(s16 *)(work + 0x2C8) = PlaySoundEffect(0x38);
            Ovl17SelectAllRecordAnimations0();
            work[4] = 7;
            *(s16 *)(work + 0x2CC) = -1;
            Ovl17UpdateTimerHud(-1);
        }
    }
}

/* 8001FAD0 forwards the s32 voice result returned by func_800212A8. */
void Ovl17DrawRecordTrails(void);
void Ovl17DrawPausedRecordSceneFrame(void);
void Ovl17RollRecordDecisionResults(void);
void Ovl17AdvanceRecordDecisionProgress(void);
void Ovl17DecayRecordCountdown(void);
s32 Ovl17ClampRecordPositionsAndTestComplete(void);
void Ovl17RaiseRecordCountdownSteps(void);
void Ovl17UpdateRecordSelectionList(void);
void Ovl17UpdateRecordScrollSpeed(void);

void Ovl17RunRecordSceneStage(void) {
    s16 temp_v0_2;
    s32 temp_v1;
    s16 var_a1_2;
    s32 temp_v0;
    s32 var_s1;
    u32 var_a0;
    u8 *var_a0_2;
    u8 *var_a1;
    u8 *var_s0;
    u8 *var_s0_2;
    u8 *base;
    u8 *threshold;
    u8 *motion;
    s32 displacement;

    Ovl17DrawRecordTrails();
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x800) {
        var_s0 = gOvl17RecordWorkBase;
        temp_v0 = (*(s32 *) (var_s0 + 0)) != 1;
        (*(s32 *) (var_s0 + 0)) = temp_v0;
        if (temp_v0 == 1) {
            (*(s16 *) (var_s0 + 0x2C8)) = PlaySoundEffect(0x36);
            goto block_3;
        }
        goto block_6;
    }
block_3:
    if ((*(s32 *) gOvl17RecordWorkBase) == 1) {
        Ovl17DrawPausedRecordSceneFrame();
        return;
    }
block_6:
    base = gOvl17RecordWorkBase;
    displacement = *(s16 *)(base + 0x2BC) * -0x50;
    if (*(s32 *)(base - displacement + 0x38) <= 0x800000) {
        Ovl17UpdateTimerHud(2);
    }
    (*(u32 *) (base + 0x1C))++;
    for (var_s1 = 0; var_s1 < 6; var_s1++) {
        var_s0 = gOvl17RecordWorkBase;
        temp_v0_2 = Ovl17EvaluateRecordValueCurve(*(s16 *) (var_s0 + var_s1 * 0x50 + 0x34));
        (*(s16 *) (var_s0 + var_s1 * 0x50 + 0x2C)) = temp_v0_2;
        (*(s16 *) (var_s0 + var_s1 * 0x50 + 0x2E)) = temp_v0_2;
    }
    if ((((Ovl17ModeView *)D_8006C838)->mode == 3) && ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x40)) {
        u8 *work;
        work = gOvl17RecordWorkBase;
        (*(s32 *) (work + 0xD0)) = 1;
        RequestPadActuatorSequence(0, 0);
        (*(s16 *) (work + 0x2C8)) = PlaySoundEffect(0x3D);
    }
    var_s0 = gOvl17RecordWorkBase;
    if (*(u32 *) (var_s0 + 0x1C) == ((*(u32 *) (var_s0 + 0x1C) / (u32) (*(s16 *) (var_s0 + 6))) * (*(s16 *) (var_s0 + 6)))) {
        Ovl17RaiseRecordCountdownSteps();
        Ovl17RollRecordDecisionResults();
        Ovl17AdvanceRecordDecisionProgress();
        Ovl17DecayRecordCountdown();
        var_a0 = (u32) ((Ovl17ModeView *)D_8006C838)->mode;
        if (var_a0 == 3) {
            (*(s32 *) (var_s0 + 0xD0)) = 0;
        }
    }
    if (((Ovl17RecordWorkInitView *)gOvl17RecordWorkBase)->scrollPosition <= 0x7FFFFF) {
        Ovl17UpdateRecordScrollSpeed();
    }
    for (var_s1 = 0; var_s1 < 6; var_s1++) {
        var_a1 = gOvl17RecordWorkBase;
        (*(s32 *) (var_a1 + var_s1 * 0x50 + 0x38)) += (((*(s16 *) (var_a1 + var_s1 * 0x50 + 0x2E)) << 0xC) / 30);
    }
    Ovl17UpdateRecordSelectionList();
    motion = gOvl17RecordWorkBase;
    (*(s32 *) (motion + 0x24)) += (((*(s16 *) (motion + 0x20)) << 0xC) / 30);
    for (var_s1 = 0; var_s1 < 6; var_s1++) {
        var_s0_2 = gOvl17RecordWorkBase;
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *) (var_s0_2 + 0x28 + var_s1 * 0x50), *(s32 *) (var_s0_2 + 0x24));
    }
    threshold = gOvl17RecordWorkBase;
    if ((((*(s16 *) (threshold + 0x2D0)) << 0x17) / 100) < (*(s32 *) (threshold + 0x24))) {
        if ((*(s16 *) (threshold + 0x2CC)) == -1) {
            (*(s16 *) (threshold + 0x2CA)) = PlaySoundEffect(0x43);
            (*(s16 *) (threshold + 0x2CC)) = 0;
        }
    }
    var_a0_2 = gOvl17RecordWorkBase;
    if ((*(s32 *) (var_a0_2 + 0x24)) > 0x7FFFFF) {
        (*(s32 *) (var_a0_2 + 0x24)) = 0x800000;
        (*(s16 *) (var_a0_2 + 0x20)) = 0;
        (*(s16 *) (var_a0_2 + 0x22)) = 0;
        temp_v1 = (u16) (*(s16 *) (var_a0_2 + 0x2CC)) + 1;
        var_a1_2 = 0x64 - temp_v1;
        (*(s16 *) (var_a0_2 + 0x2CC)) = temp_v1;
        if (var_a1_2 < 0x21) {
            var_a1_2 = 0x21;
        }
        SetSoundVoiceVolume(*(s16 *) (var_a0_2 + 0x2CA), var_a1_2, var_a1_2);
    }
    if (Ovl17ClampRecordPositionsAndTestComplete() == 1) {
        u8 *state;
        state = gOvl17RecordWorkBase;
        (*(s16 *) (state + 0x2CC)) = 0;
        (*(s8 *) (state + 4)) = 8;
    }
    Ovl17DrawRecordBackdrop(((Ovl17RecordWorkInitView *)gOvl17RecordWorkBase)->scrollPosition);
}

void Ovl17ShowSelectedRecordNamePage6(void) {
    s32 i;
    s32 selected;
    s32 data;
    s32 name;
    s32 entry;
    s32 chosen;
    u8 *base;
    u8 *state;
    u8 *current;

    Ovl17DrawRecordBackdrop(0x800000);
    selected = 0;
    Ovl17DrawRecordTrails();
    for (i = 0; i < 6; i++) {
        base = gOvl17RecordWorkBase;
        entry = ((Ovl17RecordWorkInitView *)base)->selectionIndices[i];
        if (entry == 2) {
            selected = i;
        } else {
            data = (s32)D_800556A8;
            SetTextSourcePointer((u16)i, *(u16 *)(base + entry * 0x50 + 0x40) + data);
        }
    }
    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        name = (s32)&D_80075AD8;
    } else {
        current = gOvl17RecordWorkBase;
        chosen = ((Ovl17RecordWorkInitView *)current)->selectionIndices[selected];
        name = (s32)D_800556A8 + *(u16 *)(current + chosen * 0x50 + 0x40);
    }
    SetTextSourcePointer((u16)selected, name);
    RequestMessageWindow(4, 6);
    if (GetMessageWindowStatus() == 2) {
        state = gOvl17RecordWorkBase;
        StopSoundEffectVoice(*(s16 *)(state + 0x2CA));
        state[4] = 9;
    }
}

s16 Ovl17DrawHorseRaceBetsPanel(s32 arg0);

void Ovl17ShowRecordTotalPage7(void) {
    u32 *boxes;
    s32 text;
    u16 mode;
    s32 total;
    Ovl17SummaryView *work;

    boxes = (u32 *)(gActiveRenderContext->field_D8 + 0xC);
    text = gActiveRenderContext->field_D8 + 8;
    work = (Ovl17SummaryView *)D_8006C838;
    mode = 2;
    if (work->mode != 3) {
        mode = (u16)work->mode;
    }
    Ovl17DrawRecordTrails();
    Ovl17DrawRecordBackdrop(0x800000);
    DrawScreenSubtractOverlay(0x40, 0x40, 0x40);
    SetTextSourcePointer(0, (s32)&gOvl17RecordModeHeadingText[mode * 2]);
    DrawMenuPanel(boxes, 1, 0x28, 0x30, 0xF4, 0x72);
    DrawTextLine(text, (s32)gOvl17RecordPanelLabelText, 0x3D, 0x32);
    total = Ovl17DrawHorseRaceBetsPanel(1);
    DrawMenuPanel(boxes, 1, 0xC3, 0x10, 0x6F, 0x1E);
    DrawTextLine(text, (s32)(gOvl17RecordPanelLabelText - 0x17), 0xC7, 0x12);
    *(u16 *)WriteDecimalText(work->value, (s16 *)gOvl17NumberTextBuffer, 7) = 0xFFFF;
    DrawTextLine(text, (s32)gOvl17NumberTextBuffer, 0xC5, 0x1E);
    RequestMessageWindow(4, 7);
    if (GetMessageWindowStatus() == 2) {
        if (total != 0) {
            OVL17_RECORD_WORK.soundVoice = PlaySoundEffect(0x3E);
        }
        OVL17_RECORD_WORK.phase = 0xA;
    }
}

void Ovl17ShowRecordResultPage8(void) {
    u32 *boxes;
    s32 text;
    s32 total;
    Ovl17SummaryUpperView *work;
    Ovl17SummaryResultView *base;

    boxes = (u32 *)(gActiveRenderContext->field_D8 + 0xC);
    text = gActiveRenderContext->field_D8 + 8;
    Ovl17DrawRecordTrails();
    Ovl17DrawRecordBackdrop(0x800000);
    DrawScreenSubtractOverlay(0x40, 0x40, 0x40);
    DrawMenuPanel(boxes, 1, 0x28, 0x30, 0xF4, 0x72);
    DrawTextLine(text, (s32)gOvl17RecordPanelLabelText, 0x3D, 0x32);
    total = Ovl17DrawHorseRaceBetsPanel(0);
    base = (Ovl17SummaryResultView *)D_8006C838;
    total += base->value;
    if (total > 99999) {
        total = 99999;
    }
    DrawMenuPanel(boxes, 1, 0xC3, 0x10, 0x6F, 0x1E);
    DrawTextLine(text, (s32)(gOvl17RecordPanelLabelText - 0x17), 0xC7, 0x12);
    *(u16 *)WriteDecimalText(total, (s16 *)gOvl17NumberTextBuffer, 7) = 0xFFFF;
    DrawTextLine(text, (s32)gOvl17NumberTextBuffer, 0xC5, 0x1E);
    work = (Ovl17SummaryUpperView *)((u8 *)base + 0x8000);
    if (work->mode == 3 && D_800BDB04 == 2) {
        work->enabled = 1;
    } else {
        work->enabled = 0;
    }
    ((Ovl17SummaryResultView *)D_8006C838)->total = total;
    RequestMessageWindow(4, 8);
    if (GetMessageWindowStatus() == 2) {
        ((Ovl17SummaryResultView *)D_8006C838)->state++;
    }
}

void Ovl17ResetRecordTrails(void) {
    s16 i;

    for (i = 0; i < 90; i++) {
        ((Ovl17QueueView *)gOvl17RecordWorkBase)->entries[i].positionQ12 = -0x20000;
        ((Ovl17QueueView *)gOvl17RecordWorkBase)->entries[i].y = -1;
        if (i < 6) {
            ((Ovl17QueueView *)gOvl17RecordWorkBase)->entries[i].y = i * 0x1A + 0x4D;
        }
    }
    ((Ovl17QueueView *)gOvl17RecordWorkBase)->cursor = 6;
}

/* Wrap before insertion, not after it. Preserve entry-member lvalues: the
 * word and halfword accesses have distinct address-expression shapes. */
void Ovl17PushRecordTrailNode(s32 positionQ12, s16 y) {
    Ovl17QueueView *queue;

    queue = (Ovl17QueueView *)gOvl17RecordWorkBase;
    if (queue->cursor >= 90) {
        queue->cursor = 0;
    }
    queue->entries[queue->cursor].positionQ12 = positionQ12;
    queue->entries[queue->cursor].y = y;
    queue->cursor++;
}

void Ovl17DrawRecordTrails(void) {
    s16 y;
    s16 i;
    s32 screenX;
    s16 mirror;
    u8 *base;
    s16 *yFields;

    base = gOvl17RecordWorkBase;
    i = 0;
    do {
        yFields = (s16 *)(base + 0x2DC);
        y = yFields[i * 4];
        if (y != -1) {
            mirror = (i - (s16)(i / 12) * 12) >= 6;
            screenX = 0xA0 -
                (((Ovl17QueueView *)base)->entries[i].positionQ12 -
                 *(s32 *)(base + 0x24)) / 0x1000;
            DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x54,
                         (s32)(base + 0x22C), 3, mirror, (s16)screenX, y);
        }
        i++;
    } while (i < 90);
}

void Ovl17DrawPausedRecordSceneFrame(void) {
    s32 i;
    u8 *record;
    u8 *base;
    u8 *work;
    u8 *display;

    Ovl17DrawDisplaySpriteFrameRects();
    base = gOvl17AnimatedRecordBase;
    work = base - 0x28;
    record = base;
    for (i = 0; i < 6; i++) {
        Ovl17DrawRecordSpriteRects((Recon_ovl_17_func_800BAD9C_A0View *) record,
                            *(s32 *) (work + 0x24));
        record += 0x50;
    }
    display = gOvl17RecordWorkBase;
    Ovl17DrawRecordBackdrop(*(s32 *) (display + 0x24));
}

void Ovl17RollRecordDecisionResults(void) {
    s32 i;
    s32 ready;
    s32 group;
    s32 recordOffset;
    s32 entryOffset;
    s32 column;
    u8 *table;
    u8 *work;
    u8 *settings;
    u8 *resultBase;
    u8 *cursor;

    i = 0;
    work = D_80074838;
    /* D_800BD870 is the work area's record alias at +0x28. */
    table = (u8 *)((u32)gOvl17AnimatedRecordBase - 0x28);
    resultBase = table + 0x30;
    recordOffset = 0;
    cursor = resultBase;
    do {
        if (i != 2 || *(s16 *)(work + 0x6514) != 3) {
            settings = gOvl17RecordWorkBase;
            if (*(s32 *)cursor == 1) {
                *(s32 *)cursor = 0;
            } else {
                group = Ovl17EvaluateRecordThresholdState(i);
                ready = Ovl17IsRecordDecisionReady(i);
                if (Ovl17IsRecordAboveDecisionThreshold(i, *(s16 *)(settings + 0x16)) != 0) {
                    column = 0;
                } else if (Ovl17IsRecordAboveDecisionThreshold(i, *(s16 *)(settings + 0x18)) != 0) {
                    column = 2;
                } else {
                    column = 4;
                }
                if (ready == 0) {
                    column++;
                }
                entryOffset = column * 2;
                entryOffset += group * 0xC;
                *(s32 *)(recordOffset + (u32)resultBase) =
                    Ovl17RollAboveDecisionThreshold(*(s16 *)(table + entryOffset + 0x208));
            }
        }
        recordOffset += 0x50;
        i++;
        cursor += 0x50;
    } while (i < 6);
}

void Ovl17AdvanceRecordDecisionProgress(void) {
    u8 *base;
    u8 *work;
    u8 *p;
    s32 i;
    s32 off;
    s32 flag;
    s16 q;

    base = gOvl17RecordWorkBase;
    flag = 1;
    work = D_80074838;
    p = base;
    off = 0;
    for (i = 0; i < 6; i++) {
        if (*(u32 *)(base + off + 0x30) != 0) {
            if (*(s16 *)(p + 0x2A) >= *(s16 *)(base + 0x12)) {
                *(u16 *)(p + 0x2A) = *(u16 *)(p + 0x2A) - *(u16 *)(base + 0x12);
                q = (s16)((*(s16 *)(p + 0x2E) * (*(s16 *)(p + 0x36) + *(s16 *)(base + 0x0E))) / (*(s16 *)(base + 0x10) * 0xFF));
                *(u16 *)(p + 0x2E) = q + *(s16 *)(p + 0x2E);
                if (flag == 1) {
                    if (*(s16 *)(work + 0x6514) != 3) {
                        if (Rand(0x64) < 0x22) {
                            *(u16 *)(base + 0x2C8) = PlaySoundEffect(0x3D);
                        }
                    }
                    flag = 0;
                }
            }
        }
        p += 0x50;
        off += 0x50;
    }
}

void Ovl17DecayRecordCountdown(void) {
    u8 *base;
    u8 *p;
    s32 i;
    s16 q;

    base = gOvl17RecordWorkBase;
    p = base;
    for (i = 5; i >= 0; i--) {
        q = (s16) ((*(s16 *) (p + 0x2E) - *(s16 *) (p + 0x2C)) * 0xFF / (((*(s16 *) (p + 0x2A)) + 1) * (*(s16 *) (base + 0x14))));
        *(s16 *) (p + 0x2E) = (u16) (*(s16 *) (p + 0x2E)) - q;
        if (*(s16 *) (p + 0x2C) >= *(s16 *) (p + 0x2E)) {
            *(u16 *) (p + 0x2E) = *(u16 *) (p + 0x2C);
        }
        p += 0x50;
    }
}

void Ovl17UpdateRecordSelectionList(void) {
    Ovl17SelView *v;
    Ovl17Rec *records;
    s16 best;
    s32 i;
    s32 j;
    s16 *selection;
    s32 selectionIndex;
    Ovl17SelView *lookup;
    s16 *cursor;

    v = (Ovl17SelView *) gOvl17RecordWorkBase;
    if (v->count == 0) {
        v->sel[0] = -1;
    }
    i = 0;
    records = v->recs;
    for (; i < 6; i++) {
        if (records[i].field0 > 0x7FFFFF) {
            j = 0;
            if (v->sel[0] != i) {
                lookup = (Ovl17SelView *)gOvl17RecordWorkBase;
                cursor = lookup->sel;
                do {
                    j++;
                    cursor++;
                    if (j >= 6) {
                        break;
                    }
                } while (*cursor != i);
            }
            if (j == 6) {
                selection = v->sel;
                selectionIndex = v->count;
                *(s16 *)((u32)(selectionIndex * 2) + (u32)selection) = i;
                v->count = v->count + 1;
            }
        }
    }
    {
        u8 *b;
        s32 *p;
        u8 *active;
        u8 *destination;

        b = gOvl17RecordWorkBase;
        if (*(s16 *) (b + 0x2D4) == 0) {
            best = 0;
            i = 0;
            active = gOvl17RecordWorkBase;
            p = (s32 *) (active + 0x38);
            for (; i < 6; i++) {
                for (j = 0; j < 6; j++) {
                    b = active + 0x38 + j * 0x50;
                    if (*(s32 *) ((u8 *) p + best * 0x50) < *(s32 *)b) {
                        best = j;
                    }
                }
            }
            destination = gOvl17RecordWorkBase;
            *(s16 *) (destination + 0x2BC) = best;
        }
    }
}

void Ovl17UpdateRecordScrollSpeed(void) {
    s32 selected;
    s32 scaled;
    s32 position;
    s32 positionPixels;
    s32 origin;
    u16 value;
    u8 *base;
    u8 *rows;

    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        selected = 2;
    } else {
        selected = ((Ovl17RecordWorkInitView *)gOvl17RecordWorkBase)->selectionIndices[0];
    }
    base = gOvl17RecordWorkBase;
    scaled = selected * 0x50;
    rows = base + 0x28;
    value = *(u16 *)(base + scaled + 0x2C);
    ((Ovl17RecordWorkInitView *)base)->targetScrollSpeed = value;
    selected = *(s32 *)(rows + scaled + 0x10);
    position = selected;
    positionPixels = position / 0x1000;
    origin = ((Ovl17RecordWorkInitView *)base)->scrollPosition;
    if (positionPixels > origin / 0x1000 + 0x50) {
        ((Ovl17RecordWorkInitView *)base)->scrollSpeed += ((Ovl17RecordWorkInitView *)base)->scrollSpeed / 8;
        return;
    }
    if (position / 0x1000 < origin / 0x1000 - 0x3C) {
        ((Ovl17RecordWorkInitView *)base)->scrollSpeed -= ((Ovl17RecordWorkInitView *)base)->scrollSpeed / 8;
        return;
    }
    ((Ovl17RecordWorkInitView *)base)->scrollSpeed = value;
}

s32 Ovl17ClampRecordPositionsAndTestComplete(void) {
    s32 temp_a2;
    s32 i;
    s32 count;
    u8 *base;
    u8 *temp_s0;
    u8 *p;
    u8 *obj;
    u8 *arg;

    base = gOvl17RecordWorkBase;
    temp_a2 = (*(s16 *) (base + 0x2BC)) * 0x50;
    temp_s0 = base + 0x38;
    if (*(s32 *)(temp_s0 + temp_a2) > 0x800000) {
        arg = base + 0x28;
        Ovl17SelectRecordAnimation1(arg + temp_a2);
    }
    for (i = 0; i < 6; i++) {
        p = temp_s0 + i * 0x50;
        if ((*(s32 *) p) > 0x93FFFF) {
            (*(s32 *) p) = 0x940000;
        }
    }
    count = 0;
    for (i = 0; i < 6; i++) {
        obj = gOvl17RecordWorkBase;
        p = obj + 0x38 + i * 0x50;
        if ((*(s32 *) p) <= 0x93FFFF) {
            count = (count + 1) & 0xFF;
        }
    }
    return count == 0;
}

/* Draws the six-entrant betting table and returns the selected entrant's
 * odds * ticketCount winnings (shown only when arg0 is set).
 *
 * `text` is reused as a scratch string pointer, and it is bound to the row
 * record before the name is chosen. All four sets are dead. cse forwards the
 * three string addresses into the call arguments, and both arms overwrite the
 * row binding. Because `text` has live uses, the sets survive cse's
 * trivially-dead sweep and count as real loop insns until flow deletes them.
 * The re-run loop pass needs at least 117 such insns to judge the
 * loop-invariant &D_800556A8 high/lo pair "not desirable"
 * (threshold 29 * savings 2 * lifetime 2 = 116 < insn_count). The pair then
 * stays local and takes $v0 ahead of the nameOffset load, as in the target.
 * Removing any one of the four sets leaves 116 insns, and the pair is hoisted
 * and rematerialised by reload instead (186/194). See
 * notes/research/harness-improvement-case-studies/ovl_11_func_80110944-dead-bindings-and-compiler-history.md
 * for the dead-binding mechanism. */
s16 Ovl17DrawHorseRaceBetsPanel(s32 arg0) {
    s32 surface;
    u16 total;
    u16 row;
    s16 y;
    s32 i;
    u8 *text;

    surface = gActiveRenderContext->field_D8 + 8;
    row = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        row = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    total = 0;
    y = 0x40;
    for (i = 0; i < 6; i++) {
        if (((Ovl17RowsView *)gOvl17RecordWorkBase)->selected == i) {
            gOvl17NumberTextBuffer[0] = 0x90;
            gOvl17NumberTextBuffer[1] = 0xFFFF;
            text = (u8 *)gOvl17NumberTextBuffer;
            DrawTextLine(surface, (s32)text, 0x2E, y);
        }
        gOvl17NumberTextBuffer[1] = 0xFFFF;
        gOvl17NumberTextBuffer[0] = gOvl17RecordRowLabelText[i];
        text = (u8 *)gOvl17NumberTextBuffer;
        DrawTextLine(surface, (s32)text, 0x38, y);
        text = (u8 *)&((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[i];
        if (((Ovl17ModeView *)D_8006C838)->mode == 3 && i == 2) {
            text = (u8 *)&D_80075AD8;
        } else {
            text = &D_800556A8[((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[i].nameOffset];
        }
        DrawTextLine(surface, (s32)text, 0x46, y);
        DrawTextLine(surface, (s32)gOvl17RecordColumnLabelText, 0x8E, y);
        *(u16 *)WriteDecimalText(((Ovl17ModeView *)D_8006C838)->entries[row][i].odds, (s16 *)gOvl17NumberTextBuffer, 2) = 0xFFFF;
        DrawTextLine(surface, (s32)gOvl17NumberTextBuffer, 0xB2, y);
        *(u16 *)WriteDecimalText(((Ovl17ModeView *)D_8006C838)->entries[row][i].ticketCount, (s16 *)gOvl17NumberTextBuffer, 2) = 0xFFFF;
        DrawTextLine(surface, (s32)gOvl17NumberTextBuffer, 0xCA, y);
        text = &gOvl17RecordColumnLabelText[5];
        DrawTextLine(surface, (s32)text, 0xDE, y);
        if (((Ovl17RowsView *)gOvl17RecordWorkBase)->selected == i) {
            total += (u16)((Ovl17ModeView *)D_8006C838)->entries[row][i].odds * (u16)((Ovl17ModeView *)D_8006C838)->entries[row][i].ticketCount;
        }
        y += 13;
    }
    DrawTextLine(surface, (s32)&gOvl17RecordColumnLabelText[13], 0x4E, 0x90);
    *(u16 *)WriteDecimalText(arg0 != 0 ? total : 0, (s16 *)gOvl17NumberTextBuffer, 4) = 0xFFFF;
    DrawTextLine(surface, (s32)gOvl17NumberTextBuffer, 0xB6, 0x90);
    return total;
}

s16 Ovl17EvaluateRecordValueCurve(s16 arg0) {
    u8 *base;
    s16 a;
    s16 b;
    s16 c;

    base = gOvl17RecordWorkBase;
    a = *(s16 *)(base + 0x8);
    b = *(s16 *)(base + 0xA);
    c = *(s16 *)(base + 0xC);

    return (s16)(*(u16 *)(base + 0x8) + (a * (arg0 + b) * c) / 25500);
}

void Ovl17RaiseRecordCountdownSteps(void) {
    u8 *base;
    s32 i;

    base = gOvl17RecordWorkBase;
    for (i = 5; i >= 0; i--) {
        if (*(s16 *)(base + 0x2A) < 0xFF) {
            *(s16 *)(base + 0x2A) = *(u16 *)(base + 0x2A) + 5;
        }
        base += 0x50;
    }
}

/*
 * Threshold-roll classification of six 0x50-byte record views. The selected
 * comparison record chooses the primary roll's orientation; a nearby-value
 * record enables the secondary roll. This service consumes shared RNG state.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
s32 Ovl17HasRecordInValueWindow(s16 recordIndex);
s32 Ovl17RollSecondaryThreshold(s16 threshold);
s32 Ovl17RollPrimaryThreshold(s16 threshold);
s32 Ovl17RollComplementThreshold(s16 threshold);
s32 Ovl17RollFraction127(s32 numerator);

s32 Ovl17EvaluateRecordThresholdState(s16 recordIndex) {
    s32 result;
    s32 primaryState;
    s32 (*rollPrimary)(s16);
    u8 *work;
    u8 *recordBase;
    u8 *values;
    u8 *record;

    work = gOvl17RecordWorkBase;
    values = work + 0x38;
    if (*(s32 *)(values + *(s16 *)(work + 0x2BC) * 0x50) >= (*(s16 *)(work + 0x2D2) << 12)) {
        rollPrimary = Ovl17RollPrimaryThreshold;
    } else {
        rollPrimary = Ovl17RollComplementThreshold;
    }
    recordBase = gOvl17RecordWorkBase;
    record = recordBase + recordIndex * 0x50;
    primaryState = (rollPrimary(*(s16 *)(record + 0x42)) != 0) ? 1 : 2;
    result = primaryState;
    if (Ovl17HasRecordInValueWindow(recordIndex) != 0) {
        if (Ovl17RollSecondaryThreshold(*(s16 *)(record + 0x44)) != 0) {
            result = primaryState - 1;
        }
    }
    return result;
}

s32 Ovl17RollPrimaryThreshold(s16 threshold) {
    return Ovl17RollFraction127(threshold + 1);
}

s32 Ovl17RollComplementThreshold(s16 threshold) {
    return Ovl17RollFraction127(0x7F - threshold);
}

/* Separate original entry for the second field, despite the identical body. */
s32 Ovl17RollSecondaryThreshold(s16 threshold) {
    return Ovl17RollFraction127(threshold + 1);
}

s32 Ovl17RollFraction127(s32 numerator) {
    u16 percent = (numerator * 100) / 127;

    return Rand(0x64) < percent;
}

s32 Ovl17HasRecordInValueWindow(s16 recordIndex) {
    u8 *work;
    s32 *values;
    s32 *selectedValue;
    s32 i;
    s32 difference;

    i = 0;
    work = gOvl17RecordWorkBase;
    values = (s32 *)(work + 0x38);
    selectedValue = (s32 *)((u8 *)values + recordIndex * 0x50);
    for (; i < 6; i++) {
        if (i == recordIndex) {
            continue;
        }
        difference = *(s32 *)((u8 *)values + i * 0x50) - *selectedValue;
        if (difference < 0) {
            continue;
        }
        if (difference > 0x59FFF) {
            continue;
        }
        return 1;
    }
    return 0;
}

/*
 * Record decision predicates and threshold rolls.
 *
 * Decision-table inputs: compare signed per-record fields with thresholds and
 * roll the selected signed threshold against a 0..126 RNG sample. These
 * three helpers share one decision-hub client; indices and thresholds are
 * unchecked.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

s32 Ovl17IsRecordAboveDecisionThreshold(s16 recordIndex, s16 threshold) {
    u8 *base;
    s32 scaled;

    base = gOvl17RecordWorkBase;
    scaled = recordIndex * 0x50;
    return (s16)threshold < *(s16 *)(base + scaled + 0x2A);
}

s32 Ovl17IsRecordDecisionReady(s16 recordIndex) {
    u8 *base;
    s32 scaled;

    base = gOvl17RecordWorkBase;
    scaled = recordIndex * 0x50;
    return *(s16 *)(base + scaled + 0x36) >= 0x51;
}

s32 Ovl17RollAboveDecisionThreshold(s16 threshold) {
    s32 sample;

    sample = Rand(0x7F);
    return threshold < sample;
}

/*
 * Staged sprite/audio resources, ovl_17:800B9FA8..800BA54C.
 * The scene initializer calls the three blocking archive stages in order,
 * then queues the audio bank. Retained bank storage outlives heap scratch;
 * other scene code consumes the published header variants and sprite sources.
 * See ovl17_sprite_resources.h for byte windows, side effects and constraints.
 */
void Ovl17LoadSharedSpriteBank(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl17SharedSpriteBank, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData((SpriteSourceData *)gOvl17SharedBankSpriteSource, (SpriteDataHeader *)gOvl17SharedSpriteBank);
}

void Ovl17LoadSpriteImageBank(void) {
    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {

    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}

typedef struct {
    u8 bytes[0xC28];
} Ovl17RecordSpriteBankBlob;

void Ovl17LoadRecordSpriteBank(void) {
    s32 record;

    DrawSync(0);
    ClearOTagR((u_long *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x12, 0, 0x1E0A8, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x12, 0, 0x1E0A8, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4EB8));
    /* An aggregate copy retains the bank after scratch is reused. */
    *(Ovl17RecordSpriteBankBlob *) gOvl17RecordSpriteBank = *(Ovl17RecordSpriteBankBlob *)(gGraphicsHeapBase + 0x4290);
    gOvl17SpriteHeaderVariantA = gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[1];
    gOvl17SpriteHeaderVariantB = gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[2];
    InitSpriteSourceData(&gOvl17SceneSpriteSource, (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[4]));
    InitSpriteSourceData((SpriteSourceData *) gOvl17DisplaySpriteSource, (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[0]));
    InitSpriteSourceData((SpriteSourceData *)(gOvl17DisplaySpriteSource + 0x60), (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[3]));
    for (record = 0; record < 6; record++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl17DisplaySpriteSource + record * 0x50 - 0x1E4), (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[2]));
    }
}

void Ovl17QueueSoundBankLoads(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x15, 1);
}

/*
 * Record work-area initialization, ovl_17:800BA54C..800BA808.
 * The terminal scene-load phase seeds controls, roll thresholds and row
 * targets, then loads six entrants from the shared horse-race table. Row reset
 * is also a public operation during the scene. No sprite archive is loaded
 * here; the resource and rendering sections below consume the same rows.
 */
#define RECORD_WORK OVL17_RECORD_WORK

void Ovl17InitRecordRollThresholds(void);
void Ovl17InitRowSlotTargets(void);

void Ovl17InitRecordWork(void) {
    s32 slot;

    RECORD_WORK.toggle = 0;
    RECORD_WORK.phase = 0;
    RECORD_WORK.tickPeriod = 6;
    RECORD_WORK.valueBase = 0x80;
    RECORD_WORK.valueBias = 0;
    RECORD_WORK.valueGain = 4;
    RECORD_WORK.field_E = 0x96;
    RECORD_WORK.field_10 = 1;
    RECORD_WORK.field_12 = 0x28;
    RECORD_WORK.field_14 = 0x12;
    RECORD_WORK.field_16 = 0xAA;
    RECORD_WORK.field_18 = 0x64;
    RECORD_WORK.tickCounter = 0;
    RECORD_WORK.scrollSpeed = 0;
    RECORD_WORK.targetScrollSpeed = 0;
    RECORD_WORK.scrollPosition = 0x40000;
    for (slot = 0; slot < 6; slot++) {
        RECORD_WORK.selectionIndices[slot] = -1;
    }
    RECORD_WORK.soundVoice = -1;
    RECORD_WORK.soundTriggerPercent = 0x32;
    RECORD_WORK.primaryValueThreshold = 0x400;
    RECORD_WORK.selectionCount = 0;
    Ovl17InitRecordRollThresholds();
    Ovl17InitRowSlotTargets();
    Ovl17ResetHorseRaceEntrants();
    Ovl17UpdateTimerHud(-1);
}

/* Expand the packed 3x6 byte thresholds to the work area's signed halves.
 * The decision hub uses these as inputs to its Rand(127) comparison. */
void Ovl17InitRecordRollThresholds(void) {
    s32 group;
    s32 column;
    u8 *packed;
    u8 *base;
    s16 *thresholds;

    group = 0;
    base = gOvl17RecordWorkBase;
    packed = gOvl17RecordRollThresholds;
    thresholds = (s16 *)(base + 0x208);
    for (; group < 3; group++) {
        for (column = 0; column < 6; column++) {
            thresholds[group * 6 + column] = packed[group * 6 + column];
        }
    }
}

/* Six triples with a fixed 0..5 permutation in the middle halfword.
 * Rows retain pointers to these triples; their ultimate reader is unknown. */
void Ovl17InitRowSlotTargets(void) {
    s16 *targets;
    s32 slot;
    s32 x;

    targets = (s16 *)gOvl17RowSlotTargets;
    targets[1] = 5;
    targets[4] = 0;
    targets[7] = 4;
    targets[10] = 3;
    targets[13] = 1;
    targets[16] = 2;
    for (slot = 0; slot < 6; slot++) {
        x = 0x4D + slot * 0x1A;
        targets[slot * 3] = x;
        targets[slot * 3 + 2] = 0x14 - slot;
    }
}

/* Mode 3 uses race 2 and substitutes the player's horse parameters into
 * entrant 2. Other modes directly index the race table without a bounds check. */
void Ovl17ResetHorseRaceEntrants(void) {
    u16 row;
    s32 slot;

    row = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        row = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    for (slot = 0; slot < 6; slot++) {
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].flags = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk2A = 0xFF;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk2C = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk2E = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk30 = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].parameter0 =
            ((Ovl17ModeView *)D_8006C838)->entries[row][slot].parameter0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].parameter1 =
            ((Ovl17ModeView *)D_8006C838)->entries[row][slot].parameter1;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].attr[0] =
            ((Ovl17ModeView *)D_8006C838)->entries[row][slot].attr[0];
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].attr[1] =
            ((Ovl17ModeView *)D_8006C838)->entries[row][slot].attr[1];
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk38 = -0x20000;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].data = &gOvl17RowSlotTargets[slot * 6];
    }
    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].flags = 1;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].parameter0 =
            ((Ovl17ModeView *)D_8006C838)->unk9348;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].parameter1 =
            ((Ovl17ModeView *)D_8006C838)->unk92B2;
    }
}

/*
 * Scrolling backdrop and its private prop/animation tables.
 *
 * Draw two scrolling tile layers, edge markers and table-driven decorations
 * using the record scene display and prop sprite sources.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

static __inline__ void drawTile(s32 ot, u8 *sprite, s16 x) {
    u8 *work = sprite - 0x22C;
    DrawSpriteFrameRects(ot, (s32)sprite, work[0x230], work[0x231], x, 0);
}

void Ovl17DrawRecordBackdrop(s32 scrollQ12) {
    s16 x;
    s16 edgeX;
    s16 edgeX2;
    s16 edgeX3;
    s32 pixels;
    s32 propIndex;
    s32 ot;
    u8 *edgeBase;
    u8 *endBase;

    ot = gActiveRenderContext->field_D8 + 0x58;
    SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 0);
    SelectSpriteAnimationFrame((ObjectState *)gOvl17DisplaySpriteSource, 0);
    /* Signed divisions truncate toward zero, including negative scroll. */
    for (x = scrollQ12 / 4096 - (scrollQ12 / 262144) * 64 - 64; x < 384; x += 64) {
        drawTile(gActiveRenderContext->field_D8 + 0x5C, gOvl17DisplaySpriteSource, x);
    }
    SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 1);
    x = (scrollQ12 / 4096) * 2 - (scrollQ12 / 155648) * 76 - 76;
    for (; x < 396; x += 76) {
        drawTile(gActiveRenderContext->field_D8 + 0x18, gOvl17DisplaySpriteSource, x);
    }
    pixels = scrollQ12 / 4096;
    if ((u32)(pixels + 191) < 383) {
        SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 0);
        SelectSpriteAnimationFrame((ObjectState *)gOvl17DisplaySpriteSource, 1);
        edgeX = pixels + 160;
        edgeX -= 32;
        edgeBase = gOvl17DisplaySpriteSource - 0x22C;
        DrawSpriteFrameRects(ot, (s32)gOvl17DisplaySpriteSource, edgeBase[0x230], edgeBase[0x231], edgeX, 0);
        SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 2);
        edgeX2 = pixels + 224;
        edgeX2 -= 32;
        DrawSpriteFrameRects(ot, (s32)gOvl17DisplaySpriteSource, edgeBase[0x230], edgeBase[0x231], edgeX2, 0);
    }
    if ((u32)(pixels - 1857) < 383) {
        SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 0);
        SelectSpriteAnimationFrame((ObjectState *)gOvl17DisplaySpriteSource, 1);
        edgeX3 = pixels - 1888;
        edgeX3 -= 32;
        endBase = gOvl17DisplaySpriteSource - 0x22C;
        DrawSpriteFrameRects(ot, (s32)gOvl17DisplaySpriteSource, endBase[0x230], endBase[0x231], edgeX3, 0);
    }
    for (propIndex = 0; gOvl17BackdropProps[propIndex].spriteKind != 255; propIndex++) {
        if ((scrollQ12 / 4096) * 2 + HWD0 > gOvl17BackdropProps[propIndex].xQ12 / 4096 &&
            (scrollQ12 / 4096) * 2 - HWD0 < gOvl17BackdropProps[propIndex].xQ12 / 4096) {
            x = HWD0 / 2 + (scrollQ12 / 4096) * 2 - gOvl17BackdropProps[propIndex].xQ12 / 4096;
            SelectSpriteAnimation((ObjectState *)&gOvl17SceneSpriteSource, gOvl17BackdropPropAnimations[gOvl17BackdropProps[propIndex].spriteKind]);
            SelectSpriteAnimationFrame((ObjectState *)&gOvl17SceneSpriteSource, x < HWD0 / 2);
            DrawSpriteFrameQuads(gActiveRenderContext->field_D8 + 0x14, (s32)&gOvl17SceneSpriteSource,
                          gOvl17SceneSpriteSource.field_4, gOvl17SceneSpriteSource.field_5,
                          x, (s16)VWD0, 0x1000, 0, 0);
        }
    }
}

/*
 * Complementary record-sprite drawers and their shared threshold indicator.
 * Coordinates are 12-bit fixed point before projection; both paths preserve
 * signed truncation toward zero. See ovl17_record_sprite_draw.h for side effects.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */
void Ovl17SelectRecordIndicatorAnimation(void);
void Ovl17PushRecordTrailNode(s32 position, s16 y);

void Ovl17DrawRecordSpriteQuads(Recon_ovl_17_func_800BAD9C_A0View *record, s32 viewOriginX) {
    u8 *sprite;
    Recon_ovl_17_func_800BAD9C_AnimRef *display;
    s16 y;
    s16 x;
    s32 orderingTable;
    s32 delta;
    s32 pixelDelta;
    s32 hasIndicator;

    delta = record->field_10 - viewOriginX;
    pixelDelta = delta >> 0xC;
    if (delta < 0) {
        pixelDelta = (s32) (delta + 0xFFF) >> 0xC;
    }
    x = 0xA0 - pixelDelta;
    display = record->field_14;
    orderingTable = gActiveRenderContext->field_D8 + (display->field_4 * 4);
    hasIndicator = record->field_0 & 1;
    sprite = (u8 *) record + 0x20;
    y = display->field_0;
    if (!hasIndicator) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) sprite, 0, 0, 0, (s16) (s32) display->field_2);
    }
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *) sprite, x, y);
    DrawUiSprite18Frame(orderingTable, x, y, 1);
    if (record->field_0 & 1) {
        Ovl17SelectRecordIndicatorAnimation();
        DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x10, (SpriteSourceData *) gOvl17ThresholdIndicatorSprite, (s16) (x - 0x38), (s16) (y - 0x38));
    }
    /* Keep the unsigned quotient test: this is not a signed remainder. */
    if (gOvl17RecordDrawTick == ((((gOvl17RecordDrawTick + 1) / 10) * 0xA) - 1)) {
        Ovl17PushRecordTrailNode(record->field_10, y);
    }
}

void Ovl17DrawRecordSpriteRects(Recon_ovl_17_func_800BAD9C_A0View *record, s32 viewOriginX) {
    Struct_800154CC *sprite;
    Recon_ovl_17_func_800BAD9C_AnimRef *display;
    s16 y;
    s16 x;
    s32 orderingTable;
    s32 delta = record->field_10 - viewOriginX;
    s32 pixelDelta;
    s32 hasIndicator;
    u8 *work;
    u8 *indicator;

    pixelDelta = delta >> 0xC;
    if (delta < 0) {
        pixelDelta = (delta + 0xFFF) >> 0xC;
    }
    x = (s16)(0xA0 - pixelDelta);
    display = record->field_14;
    orderingTable = gActiveRenderContext->field_D8 + (display->field_4 * 4);
    hasIndicator = record->field_0 & 1;
    sprite = (Struct_800154CC *)((u8 *)record + 0x20);
    y = display->field_0;
    if (!hasIndicator) {
        SetSpriteSourceTextureOffsets(sprite, 0, 0, 0, display->field_2);
    }
    DrawSpriteFrameRects(orderingTable, (s32)sprite, (s32)record->field_24, (s32)record->field_25,
                        x, y);
    DrawUiSprite18Frame(orderingTable, x, y, 1);
    if (record->field_0 & 1) {
        Ovl17SelectRecordIndicatorAnimation();
        indicator = gOvl17ThresholdIndicatorSprite;
        work = indicator - 0x28C;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x10, (s32)indicator,
                             (s32)work[0x290], (s32)work[0x291],
                             (s16)(x - 0x38), (s16)(y - 0x38));
    }
}

void Ovl17SelectRecordIndicatorAnimation(void) {
    u8 *work;
    u8 *spriteWork;
    s32 animation;
    s16 value;

    work = gOvl17RecordWorkBase;
    value = *(s16 *)(work + 0xCA);
    if (value >= 0xAA) {
        animation = 0;
    } else if (value >= 3) {
        animation = 1;
    } else {
        animation = 2;
    }
    spriteWork = gOvl17RecordWorkBase;
    if (*(u8 *)(spriteWork + 0x290) != animation) {
        SelectSpriteAnimation((ObjectState *)(spriteWork + 0x28C), (s8)animation);
    }
}

/*
 * Fixed display-sprite frame selection.
 *
 * Select animation 4 on the scene display sprite and draw its current
 * rectangular frame at screen origin.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

void Ovl17DrawDisplaySpriteFrameRects(void) {
    u8 *workBase;

    SelectSpriteAnimation((ObjectState *)&gOvl17DisplaySpriteSource, 4);
    workBase = gOvl17DisplaySpriteSource - 0x22C;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl17DisplaySpriteSource, workBase[0x230],
                  workBase[0x231], 0, 0);
}

/*
 * Embedded record-sprite animation selection. Avoid reselecting the current
 * animation, which would reset its frame and timer. The bulk entry points
 * apply the same operation to all six 0x50-byte records.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */

/* Preserve the legacy return arm and fall-through call: the callers discard
 * the result, and the changed-animation path has no defined return value. */
s32 Ovl17SelectRecordAnimation2(Ovl17RecordAnimationView *record) {
    if (record->animationIndex == 2) {
        return 2;
    } else {
        SelectSpriteAnimation((ObjectState *)((u8 *)record + 0x20), 2);
    }
}

void Ovl17SelectAllRecordAnimations2(void) {
    u8 *record;
    s32 remaining;

    record = gOvl17AnimatedRecordBase;
    for (remaining = 5; remaining >= 0; remaining--) {
        Ovl17SelectRecordAnimation2((Ovl17RecordAnimationView *)record);
        record += 0x50;
    }
}

s32 Ovl17SelectRecordAnimation0(Ovl17RecordAnimationView *record) {
    if (record->animationIndex == 0) {
        return record->animationIndex;
    } else {
        SelectSpriteAnimation((ObjectState *)((u8 *)record + 0x20), 0);
    }
}

void Ovl17SelectAllRecordAnimations0(void) {
    u8 *record;
    s32 remaining;

    record = gOvl17AnimatedRecordBase;
    for (remaining = 5; remaining >= 0; remaining--) {
        Ovl17SelectRecordAnimation0((Ovl17RecordAnimationView *)record);
        record += 0x50;
    }
}

s32 Ovl17SelectRecordAnimation1(Ovl17RecordAnimationView *record) {
    if (record->animationIndex == 1) {
        return 1;
    } else {
        SelectSpriteAnimation((ObjectState *)((u8 *)record + 0x20), 1);
    }
}

/*
 * Scene timer and its HUD formatting.
 *
 * Private elapsed-tick counter and MM:SS:centisecond HUD using a nominal 30
 * ticks per second, with separate reset, advance and draw modes.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

void Ovl17UpdateTimerHud(s16 mode) {
    s32 textOrderingTable;

    textOrderingTable = gActiveRenderContext->field_D8 + 8;
    if (mode == -1) {
        gOvl17TimerTicks = 0;
    }
    if (mode == 0 || mode == 2) {
        gOvl17TimerTicks++;
    }
    if ((u16)mode < 2) {
        /* Unsigned divisions keep the nominal 30-tick time conversion and
         * minute-within-hour wrap, including the original u32 arithmetic. */
        DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0xC), 1, 0xA, 0x14, 0x6B, 0x11);
        DrawNumberText(gOvl17TimerTicks / 1800 - (gOvl17TimerTicks / 108000) * 60, 2, textOrderingTable, 0xC, 0x16);
        DrawTextLine(textOrderingTable, (s32)gOvl17TimerSeparatorText, 0x26, 0x16);
        DrawNumberText(gOvl17TimerTicks / 300 - (gOvl17TimerTicks / 1800) * 6, 1, textOrderingTable, 0x33, 0x16);
        DrawNumberText(gOvl17TimerTicks / 30 - (gOvl17TimerTicks / 300) * 10, 1, textOrderingTable, 0x40, 0x16);
        DrawTextLine(textOrderingTable, (s32)gOvl17TimerSeparatorText, 0x4D, 0x16);
        DrawNumberText(gOvl17TimerTicks / 3 - (gOvl17TimerTicks / 30) * 10, 1, textOrderingTable, 0x5A, 0x16);
        DrawNumberText((gOvl17TimerTicks * 100) / 30 - (gOvl17TimerTicks / 3) * 10, 1, textOrderingTable, 0x67, 0x16);
    }
}

/*
 * Bounded increments of two adjacent shared-record halfwords.
 * The byte-range path compares a signed value but reloads it unsigned for
 * the addition; this is not a general-purpose clamp for negative inputs.
 * Membership and outer boundaries: notes/file-groupings.md.
 */

void Ovl17RaiseRecordByteRangeValue(s16 recordIndex) {
    char *work;
    char *record;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    /* Keep the shared-work base and the two offsets separate: the target
     * forms the 0x8000 bias before accessing the record's halfword. */
    if (*(s16 *)(record + 0x8000 + 0x19EA) < 0xEB) {
        *(s16 *)(record + 0x8000 + 0x19EA) =
            (s16)(0x14 + *(u16 *)(record + 0x8000 + 0x19EA));
    } else {
        *(s16 *)(record + 0x8000 + 0x19EA) = 0xFF;
    }
}

void Ovl17RaiseRecordHalfwordRangeValue(s16 recordIndex) {
    char *work;
    char *record;
    u16 value;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    value = *(u16 *)(record + 0x8000 + 0x19EC);
    if (value <= 0xFE0Au) {
        *(u16 *)(record + 0x8000 + 0x19EC) = value + 0x1F4;
    } else {
        *(u16 *)(record + 0x8000 + 0x19EC) = 0xFFFF;
    }
}
