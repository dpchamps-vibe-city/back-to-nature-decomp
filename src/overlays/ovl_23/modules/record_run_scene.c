/*
 * Complete record-run scene: resource bring-up, five-subphase/seven-stage
 * dispatch, message prompts, pad-driven simulation, movement and composition.
 * Six 0x50-byte actors and six 0x44-byte decision records share one work
 * aggregate with the trail rings. Positions and movement use Q12 units;
 * stage timers count update calls, not elapsed milliseconds.
 *
 * ovl23_record_run.h publishes the scene callback. The helpers below retain
 * external linkage and original entry identities, without becoming separate
 * public services. All 55 entries and their three switch tables are one TU;
 * notes/file-groupings.md records membership and the code/data outer edges.
 */

#include "common.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "game_types.h"
#include "random.h"
#include "psyq/abs.h"
#include "psyq/stddef.h"
#include "psyq/sys/types.h"
#include "psyq/libgte.h"
#include "psyq/stdlib.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl23_record_run.h"
#include "ui_sprites.h"
#include "psyq/libapi.h"
#include "text_engine.h"

/* Forward interfaces within this unit, not a claim of original static linkage. */
void Ovl23LoadSharedSpriteBank(void);
void Ovl23LoadSpriteImageBank(void);
void Ovl23LoadActorSpriteBank(void);
void Ovl23QueueSoundBankLoads(void);
void Ovl23PauseActorSprites(void);
void Ovl23ResumeActorSprites(void);
void Ovl23DrawPausedSpriteFrame(void);
void Ovl23ResetRecordTrails(void);
void Ovl23PushDecisionTrailNode(s32 xQ12, s16 laneY, s16 state);
void Ovl23PushActorTrailNode(s32 xQ12, s16 laneY, s16 state);
void Ovl23DrawRecordTrails(void);
s32 Ovl23TestRecordRandomThreshold(s16 trialRow, s16 recordIndex);
s32 Ovl23ChooseRecordWeightedResult(s16 recordIndex, s16 weightSet);
void Ovl23StepRecordInDirection(Ovl23PositionedRecord *record);
s32 Ovl23DirectionFromRecordAnchor(Ovl23PositionedRecord *record);
s32 Ovl23ChooseRecordDirection(Ovl23PositionedRecord *record);
void Ovl23SetActorControl(Ovl23Func800BB0C8Arg *record,
                          s32 state, s32 direction, s32 controlValue);
void Ovl23SetAnchorRecordControl(Ovl23Func800BB0C8Arg *record,
                                 s32 state, s32 direction, s32 controlValue);
void Ovl23AdvanceDecisionRecordPosition(Ovl23Func800BB0C8Arg *record);
void Ovl23DrawActorRecord(Ovl23ActorRecord *record);
void Ovl23DrawDecisionRecord(Ovl23DecisionRecord *record);
void Ovl23DrawScrollingRecordBackdrop(void);
void Ovl23RaiseRecordByteRangeValue(s16 recordIndex);
void Ovl23RaiseRecordHalfwordRangeValue(s16 recordIndex);

void Ovl23DispatchRecordRunSubphase(void) {
    s32 *base;
    u32 savedDrawMode;

    savedDrawMode = GetTextPalette();
    SetTextPalette(3U);
    base = &gOvl23RecordRunSubphases;
    ((void (*)(s32 *))base[gSceneSubphase])(base);
    SetTextPalette(savedDrawMode);
}

void Ovl23InitRecordRunBounds (void);

void Ovl23UpdateRecordRunSetup(void) {
    s32 *base;

    switch (*(u8 *)&gOvl23RecordRunSetupStage) {
    case 0:
        Ovl23LoadSharedSpriteBank();
        ++*(u8 *)&gOvl23RecordRunSetupStage;
        break;
    case 1:
        Ovl23LoadSpriteImageBank();
        ++*(u8 *)&gOvl23RecordRunSetupStage;
        break;
    case 2:
        Ovl23LoadActorSpriteBank();
        ++*(u8 *)&gOvl23RecordRunSetupStage;
        break;
    case 3:
        Ovl23QueueSoundBankLoads();
        ++*(u8 *)&gOvl23RecordRunSetupStage;
        break;
    case 4:
        if (IsSoundTransferComplete() != 0) {
            *(u8 *)&gOvl23RecordRunSetupStage = 5;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)&gOvl23RecordRunSetupStage == 5) {
        Ovl23InitRecordRunBounds();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        *(u8 *)&gOvl23RecordRunSetupStage = 0;
        base = (s32 *)&D_8006C838;
        ++base[0x448C >> 2];
    }
}

void Ovl23ResetRecordRunState(void);
void Ovl23UpdateRecordRunFrame(void);

void Ovl23WaitRecordRunReady(void) {
    s32 *base;
    u8 var_s0;

    Ovl23ResetRecordRunState();
    Ovl23UpdateRecordRunFrame();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        base = (s32 *)&D_8006C838;
        base[0x448C >> 2] += 1;
    }
}

void Ovl23DispatchRecordRunStage(void) {
    s32 *base;

    UpdateMessageWindow();
    base = gOvl23RecordRunStages;
    ((void (*)(s32 *))base[Ovl23_D_800BF87C_header.unk0])(base);
}

s32 Ovl23BeginRecordRunExitFade(void) {
    s32 *base;
    s32 v;

    BeginScreenFadeFeedback(10);
    BeginMasterVolumeFadeOut(10);
    base = (s32 *)&D_8006C838;
    v = base[0x448C >> 2] + 1;
    base[0x448C >> 2] = v;
    return v;
}

void SsSeqClose(short);
void SsSeqSetVol(short, short, short);
void SsVabClose(short);

void Ovl23FinishRecordRunExit(void) {
    s32 *base;
    s32 var_s0;

    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0 = (var_s0 + 1) & 0xFF;
    }
    if (var_s0 == 2) {
        if (D_800BFC90 != -1) {
            StopSoundEffectVoice((s32) D_800BFC90);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        base = (s32 *)&D_8006C838;
        base[0x448C >> 2] = 0xFF;
        RequestSceneChange(6);
        base[0xC >> 2] |= 0x80000;
    }
}

void Ovl23InitRecordChoices(void);

void Ovl23InitRecordRunBounds(void) {
    Ovl23_D_800BF87C_bounds.unk0 = 0;
    Ovl23_D_800BF87C_bounds.unk39C = 0x800;
    Ovl23_D_800BF87C_bounds.unk39E = 0x20;
    Ovl23_D_800BF87C_bounds.unk3A0 = 0x23;
    Ovl23_D_800BF87C_bounds.unk3A2 = 0x41;
    Ovl23_D_800BF87C_bounds.unk3AC = 2;
    Ovl23_D_800BF87C_bounds.unk3AE = 0x1E;
    Ovl23_D_800BF87C_bounds.unk3B2 = 0xA;
    Ovl23_D_800BF87C_bounds.unk3B0 = 4;
    Ovl23_D_800BF87C_bounds.unk3A4 = 0xA0;
    Ovl23_D_800BF87C_bounds.unk3A6 = 0x50;
    Ovl23_D_800BF87C_bounds.unk3A8 = -0x50;
    Ovl23_D_800BF87C_bounds.unk3AA = -0xA0;
    Ovl23ResetRecordRunState();
    Ovl23InitRecordChoices();
}

void Ovl23InitDecisionRecords(void);
void Ovl23InitActorRecords(void);

void Ovl23ResetRecordRunState(void) {
    struct Ovl23ScreenHeader *state = &Ovl23_D_800BF87C_header;

    state->unk4 = 0;
    state->unk8 = 0;
    state->unkC = 0;
    state->unk414 = -1;
    state->cameraXQ12 = 0x40000;
    state->unk14 = 0;
    Ovl23InitDecisionRecords();
    Ovl23InitActorRecords();
    Ovl23ResetRecordTrails();
}

void Ovl23InitDecisionRecords(void) {
    s32 i = 0;
    s32 one = 1;
    u16 *src = gOvl23DecisionRecordInitialValues;
    u8 *p = Ovl23_D_800BF87C_draw;
    u16 temp;

    for (; i < 6; i++, p += 0x44) {
        temp = *src++;
        *(u16 *)(p + 0x204) = i;
        *(u16 *)(p + 0x206) = one;
        *(u16 *)(p + 0x208) = one;
        *(u16 *)(p + 0x20A) = 2;
        *(u16 *)(p + 0x20C) = 0;
        *(u32 *)(p + 0x210) = 0x17000;
        *(u16 *)(p + 0x244) = temp;
        *(u16 *)(p + 0x20E) = D_800BBA88[Rand(4)];
    }
}

void Ovl23InitActorRecords(void) {
    s32 i;
    s32 pairIndex;
    u8 *base;
    u8 *rec2;
    u8 *work;
    u8 *rec4;
    u16 *src;

    i = 0;
    base = Ovl23_D_800BF87C_draw;
    rec2 = base + 0xA0;
    pairIndex = 0;
    for (; i < 6; i++) {
        Ovl23D87CEntry50 *pr = &((Ovl23D87CView50 *)base)->unk0[i];

        work = D_80074838;

        pr->unk24 = i;
        pr->unk26 = 1;
        pr->unk28 = 2;
        pr->unk2A = 2;
        pr->unk2C = 0;
        ((Ovl23D87CView50 *)base)->unk0[i].unk38 = 0xFFFE5000;
        ((Ovl23D87CView50 *)base)->unk0[i].unk3C = ((s16 *)&gOvl23RecordAnchorY)[i] << 12;
        ((struct Ovl23ScreenInitTail *)pr)->unk70 = gOvl23ActorScreenOffsetPairs[pairIndex];
        ((struct Ovl23ScreenInitTail *)pr)->unk72 = gOvl23ActorScreenOffsetPairs[pairIndex + 1];
        if (i == 4) {
            rec4 = base + 0x140;
            *(u16 *)(rec4 + 0x30) = 0xFF;
            *(u16 *)(rec4 + 0x32) = 0xFF;
        } else {
            if (i != 2) {
                pr->unk30 = Rand(0x60) + 0xA0;
                pr->unk32 = Rand(0x9C) + 0x64;
            } else {
                *(u16 *)(rec2 + 0x30) = *(u16 *)(work + 0x11C2);
                *(u16 *)(rec2 + 0x32) = *(u16 *)(work + 0x1258);
            }
        }
        pr->unk2E = gOvl23ActorDistanceThresholds[(s16)pr->unk32 / 50];
        ((Ovl23D87CView50 *)base)->unk0[i].unk34 =
            (s16)*(u16 *)(base + 0x3B0) * ((s16)pr->unk30 + 0x80) << 4;
        pairIndex += 2;
    }
}

void Ovl23EnterRecordRunStages(void) {
    Ovl23ResetRecordRunState();
    Ovl23UpdateRecordRunFrame();
    Ovl23_D_800BF87C_bounds.unk0 = 1;
}

void Ovl23UpdateRecordStage1(void) {
    s32 state;

    Ovl23UpdateRecordRunFrame();
    RequestMessageWindow(4, 0x2A);
    state = GetMessageWindowStatus();
    if (state == 2) {
        Ovl23_D_800BF87C_bounds.unk0 = state;
    }
}

void Ovl23UpdateRecordStage2(void) {
    Ovl23UpdateRecordRunFrame();
    RequestMessageWindow(4, 0x2B);
    if (GetMessageWindowStatus() == 2) {
        if (GetMessageWindowChoice() == 1) {
            Ovl23_D_800BF87C_header.unk414 = PlaySoundEffect(0x41);
            Ovl23_D_800BF87C_header.unk0 = 3;
        } else {
            Ovl23_D_800BF87C_header.unk0 = 1;
        }
    }
}

void Ovl23UpdateRecordStage3(void) {
    Ovl23UpdateRecordRunFrame();
    RequestMessageWindow(4, 0x2C);
    if (GetMessageWindowStatus() == 2) {
        Ovl23_D_800BF87C_bounds.unk0 = 4;
    }
}

void Ovl23UpdateRecordStage4(void) {
    Ovl23UpdateRecordRunFrame();
    SetTextSourcePointer(0, (s32)D_800BB860 + gOvl23RecordMessageTextOffsets[Ovl23_D_800BF87C_header.unk16[0]]);
    SetTextSourcePointer(1, (s32)D_800BB860 + gOvl23RecordMessageTextOffsets[Ovl23_D_800BF87C_header.unk16[1]]);
    SetTextSourcePointer(2, (s32)D_800BB860 + gOvl23RecordMessageTextOffsets[Ovl23_D_800BF87C_header.unk16[3]]);
    SetTextSourcePointer(3, (s32)D_800BB860 + gOvl23RecordMessageTextOffsets[Ovl23_D_800BF87C_header.unk16[5]]);
    RequestMessageWindow(4, 0x2D);
    if (GetMessageWindowStatus() == 2) {
        CloseMessageWindow();
        SetTextInputEnabled(0);
        SetMessagePortraitEnabled(0);
        Ovl23_D_800BF87C_header.unk0 = 5;
        Ovl23_D_800BF87C_header.unk416 = 0;
    }
}

/* Executable imports: SetVal8005E2BC and SetMessagePortraitEnabled. */

void Ovl23UpdateRecordStage5(void) {
    Ovl23UpdateRecordRunFrame();
    if (Ovl23_D_800BF87C_header.unk416 == 0) {
        CloseMessageWindow();
        SetTextInputEnabled(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0x2E);
    }
    Ovl23_D_800BF87C_header.unk416++;
    if (Ovl23_D_800BF87C_header.unk416 >= 60) {
        Ovl23_D_800BF87C_header.unk414 = PlaySoundEffect(0x38);
        CloseMessageWindow();
        Ovl23_D_800BF87C_header.unk0 = 6;
        Ovl23_D_800BF87C_header.unk416 = 0;
    }
}

void Ovl23DrawPausedRecordFrame(void);
void Ovl23ApplyPadToAnchorRecord(void);
void Ovl23StepRecordWindows(void);
void Ovl23StepActorDecisions(void);

void Ovl23UpdateRecordStage6(void) {
    struct Ovl23ScreenHeader *state = &Ovl23_D_800BF87C_header;
    struct Ovl23ActorDecisionView *race;
    u8 *systemData;
    u8 *save;
    s16 readyCount;
    s32 paused;
    s32 result;
    s32 i;
    s32 offset;
    s16 *actorState;
    s16 *recordState;

    if (state->unkC == 0) {
        if (*(s32 *)((u8 *)gPad0Input + 8) & 0x800) {
            paused = state->unk4 != 1;
            state->unk4 = paused;
            if (paused == 1) {
                Ovl23PauseActorSprites();
                state->unk414 = PlaySoundEffect(0x36);
            } else {
                Ovl23ResumeActorSprites();
            }
        }
    }
    state = &Ovl23_D_800BF87C_header;
    readyCount = 0;
    if (state->unk4 == 1) {
        Ovl23DrawPausedRecordFrame();
        return;
    }
    state->unk14 = (u16)(state->unk14 + 1);
    i = 0;
    race = (struct Ovl23ActorDecisionView *)state;
    recordState = &race->records[0].unk2;
    actorState = &race->actors[0].unk2;
    offset = 0;
    Ovl23ApplyPadToAnchorRecord();
    Ovl23StepRecordWindows();
    Ovl23StepActorDecisions();
    systemData = (u8 *)D_8006C838;
    save = D_80074838;
    for (; i < 6; i++) {
        if (((struct Ovl23DecisionView *)((u8 *)race + offset))->actors[0].unk38 >=
            (race->unk39C << 12)) {
            *recordState = *actorState = 3;
            result = ((struct Ovl23ScreenHeader *)race)->unk8;
            if (result == 0) {
                ((struct Ovl23ScreenHeader *)race)->unk8 = 1;
                ((struct Ovl23ScreenHeader *)race)->unk414 = PlaySoundEffect(0x43);
                if (i == 2) {
                    *(s32 *)(save + 0x6630) = 1;
                } else {
                    u16 choice = Ovl23_D_800BF87C_header.unk16[i];

                    *(s32 *)(save + 0x6630) = 0;
                    *(u16 *)(save + 0x6634) = choice;
                }
            }
            if (i == 2) {
                state = &Ovl23_D_800BF87C_header;
                if (((struct Ovl23ActorDecisionView *)state)->actors[2].xQ12 ==
                    ((((struct Ovl23ActorDecisionView *)state)->unk39C + 0x37) << 12)) {
                    if (state->unkC == 0) {
                        state->unkC = 1;
                        BeginScreenFadeFeedback(10);
                        BeginMasterVolumeFadeOut(10);
                    } else {
                        result = IsScreenFadeComplete();
                        if (result == 1) {
                            readyCount += 1;
                        }
                        result = GetMasterVolumeRampState();
                        if (result == 0) {
                            readyCount += 1;
                        }
                        if (readyCount == 2) {
                            if (state->unk414 != -1) {
                                StopSoundEffectVoice((s32)state->unk414);
                            }
                            StopSoundSequence(0);
                            RewindSoundLoadSlots(2, 0);
                            RewindSoundLoadSlots(1, 0);
                            *(s32 *)(systemData + 0x448C) = 0xFF;
                            RequestSceneChange(6);
                            *(s32 *)(systemData + 0xC) |= 0x80000;
                            if (*(s32 *)(save + 0x6630) == 1) {
                                Ovl23RaiseRecordByteRangeValue(1);
                                Ovl23RaiseRecordByteRangeValue(2);
                                Ovl23RaiseRecordByteRangeValue(4);
                                Ovl23RaiseRecordByteRangeValue(6);
                                Ovl23RaiseRecordByteRangeValue(0xF);
                                Ovl23RaiseRecordByteRangeValue(0x10);
                                Ovl23RaiseRecordByteRangeValue(0x11);
                                Ovl23RaiseRecordByteRangeValue(0x13);
                                Ovl23RaiseRecordByteRangeValue(0x15);
                                Ovl23RaiseRecordByteRangeValue(0x16);
                                Ovl23RaiseRecordByteRangeValue(0x1A);
                                Ovl23RaiseRecordByteRangeValue(0x1C);
                                Ovl23RaiseRecordHalfwordRangeValue(3);
                                Ovl23RaiseRecordHalfwordRangeValue(7);
                                Ovl23RaiseRecordHalfwordRangeValue(0x14);
                            }
                        }
                    }
                }
            }
        }
        actorState += 0x28;
        offset += 0x50;
        recordState += 0x22;
    }
    {
        struct Ovl23ScreenHeader *camera = &Ovl23_D_800BF87C_header;

        if (((struct Ovl23ActorDecisionView *)camera)->records[2].xQ12 > 0x3FFFF) {
            camera->cameraXQ12 = ((struct Ovl23ActorDecisionView *)camera)->records[2].xQ12;
        }
        if (camera->cameraXQ12 <= 0x40000) {
            camera->cameraXQ12 = 0x40000;
        } else {
            result = ((struct Ovl23ActorDecisionView *)camera)->unk39C << 12;
            if (camera->cameraXQ12 >= result) {
                camera->cameraXQ12 = result;
            }
        }
        Ovl23UpdateRecordRunFrame();
    }
}

void Ovl23ApplyPadToAnchorRecord(void) {
    s16 var_a2;
    s32 temp_v1;
    s32 var_a1;

    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 4);
    var_a1 = 2;
    if (temp_v1 & 0x20) {
        var_a1 = 3;
    }
    var_a2 = 2;
    if (!(temp_v1 & 0x8000)) {
        var_a2 = 6;
        if (!(temp_v1 & 0x2000)) {
            if (var_a1 == 2) {
                var_a1 = 1;
            }
            var_a2 = D_800BFB0E;
        }
    }
    Ovl23SetAnchorRecordControl(&D_800BFB08, var_a1, (s32) var_a2, 0);
    Ovl23AdvanceDecisionRecordPosition(&D_800BFB08);
}

void Ovl23ReturnDecisionRecordToAnchor(Ovl23Func800BB0C8Arg *arg0);

void Ovl23StepRecordWindows(void) {
    s32 i;
    s32 excluded;
    s32 offset;
    s16 temp_a0;
    s32 test;
    struct Ovl23DecisionRecord *var_s1;
    struct Ovl23DecisionRecord *recordsBase;
    struct Ovl23DecisionView *base2;
    s16 diff;
    s32 distance;
    struct Ovl23DecisionView *base;
    struct Ovl23DecisionWindow *record;

    for (i = 0; i < 6; i++) {
        if (i != 2) {
            base = (struct Ovl23DecisionView *)Ovl23_D_800BF87C_draw;
            record = (struct Ovl23DecisionWindow *)((u8 *)base + i * 0x44);
            if (record->unk206 == 1) {
                if (record->unk20C > 0) {
                    record->unk20C--;
                } else {
                    record->unk20C = record->unk20E;
                    diff = base->actors[i].unk38 / 4096 - base->records[i].xQ12 / 4096;
                    distance = abs(diff);
                    if (distance < base->actors[i].unk2E) {
                        switch (Ovl23ChooseRecordWeightedResult(i, 0)) {
                        case 1:
                            if (diff > 0) {
                                Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 2, record->unk20C);
                            } else {
                                Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 6, record->unk20C);
                            }
                            break;
                        case 2:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(0, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(1, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                }
                            }
                            break;
                        case 3:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(0, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(1, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                }
                            }
                            break;
                        }
                    } else {
                        switch (Ovl23ChooseRecordWeightedResult(i, 1)) {
                        case 1:
                            if (diff > 0) {
                                Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 2, record->unk20C);
                            } else {
                                Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 6, record->unk20C);
                            }
                            break;
                        case 2:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(2, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(3, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                }
                            }
                            break;
                        case 3:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(2, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(3, i) == 1) {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                } else {
                                    Ovl23SetAnchorRecordControl((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    i = 0;
    excluded = 2;
    var_s1 = D_800BFA80;
    do {
        recordsBase = D_800BFA80;
        if (i != excluded) {
            offset = i * 0x44;
            base2 = (struct Ovl23DecisionView *)((u8 *)recordsBase - 0x204);
            temp_a0 = ((struct Ovl23DecisionWindow *)((u8 *)base2 + offset))->unk206;
            offset = 1;
            test = temp_a0;
            if (test == offset) {
                Ovl23AdvanceDecisionRecordPosition((Ovl23Func800BB0C8Arg *)var_s1);
            } else {
                test = 3;
                if (temp_a0 == test) {
                    Ovl23ReturnDecisionRecordToAnchor((Ovl23Func800BB0C8Arg *)&recordsBase[i]);
                }
            }
        }
        var_s1++;
        i += 1;
    } while (i < 6);
}

void Ovl23ReturnActorToAnchor(Ovl23Func800BB0C8Arg *arg0);

void Ovl23StepActorDecisions(void) {
    s32 i;
    s16 dx;
    s16 dy;
    s16 distance;
    s16 state;
    s16 direction;
    s16 mode;
    struct Ovl23ActorDecisionView *base;
    struct Ovl23ActorDecision *actor;
    struct Ovl23ActorDecision *actors;
    struct Ovl23ActorDecisionView *stateBase;
    u16 *table;
    struct Ovl23ActorDecisionView *base2;
    struct Ovl23ActorDecision *actor2;
    s32 offset;
    struct Ovl23ActorDecisionView *window;
    s32 walkingState;
    struct Ovl23ActorDecisionView *modeRoot;
    struct Ovl23ActorDecision *decisionActor;
    s16 sampledDirection;

    i = 0;
    table = (u16 *)&gOvl23RecordAnchorY;
    base = &Ovl23_D_800BF87C_decision;
    actor = base->actors;
    for (; i < 6; i++) {
        if (actor->unk2 == 1) {
            if (actor->timer > 0) {
                actor->timer--;
            } else if (base->unk14 == (base->unk14 / base->unk3AC) * base->unk3AC) {
                dx = base->actors[i].xQ12 / 4096 - base->records[i].xQ12 / 4096;
                dy = base->actors[i].yQ12 / 4096 - table[i];
                distance = abs(SquareRoot0(dx * dx + dy * dy));
                if (abs(dx) < actor->unkA) {
                    if (distance <= base->unk39E) {
                        Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)actor, 2,
                            Ovl23DirectionFromRecordAnchor((Ovl23PositionedRecord *)actor), base->unk3AC);
                    } else {
                        Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)actor, 1,
                            Ovl23ChooseRecordDirection((Ovl23PositionedRecord *)actor), base->unk3AC);
                    }
                } else {
                    state = Rand(8) + 3;
                    if (state != 3) {
                        direction = (Rand(4) << 1) | 1;
                    } else {
                        direction = Rand(5) + 4;
                        direction = direction == 8 ? 0 : direction;
                    }
                    Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)actor, state, direction, 60);
                }
            }
        }
        actor++;
    }
    walkingState = 3;
    base2 = (struct Ovl23ActorDecisionView *)Ovl23_D_800BF87C_draw;
    actor2 = base2->actors;
    actors = actor2;
    offset = 0;
    decisionActor = actor2;
    window = base2;
    modeRoot = window;
    for (i = 0; i < 6; i++, window = (struct Ovl23ActorDecisionView *)((u8 *)window + 0x50)) {
        if (window->actors[0].state == walkingState &&
            window->actors[0].timer == (window->actors[0].timer / base2->unk3AE) * base2->unk3AE) {
            sampledDirection = Rand(5) + 4;
            direction = sampledDirection;
            sampledDirection = 8;
            direction = direction == sampledDirection ? 0 : direction;
            Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)decisionActor, window->actors[0].state,
                direction, window->actors[0].timer);
        }
        stateBase = (struct Ovl23ActorDecisionView *)(offset + (u32)modeRoot);
        mode = stateBase->actors[0].unk2;
        stateBase = (struct Ovl23ActorDecisionView *)actors;
        if (mode == 1) {
            Ovl23StepRecordInDirection((Ovl23PositionedRecord *)actor2);
        } else if (mode == walkingState) {
            Ovl23ReturnActorToAnchor((Ovl23Func800BB0C8Arg *)(offset + (u32)stateBase));
        }
        actor2++;
        offset += 0x50;
        decisionActor++;
    }
}

void Ovl23DrawPausedRecordFrame(void) {
    Ovl23DrawPausedSpriteFrame();
    Ovl23UpdateRecordRunFrame();
}

void Ovl23UpdateRecordRunFrame(void) {
    Ovl23ActorRecord *actor;
    Ovl23DecisionRecord *record;
    Ovl23ActorRecord *base;
    s32 i;

    Ovl23DrawScrollingRecordBackdrop();
    i = 0;
    base = (Ovl23ActorRecord *)D_800BF8A0;
    record = (Ovl23DecisionRecord *)(base + 6);
    actor = base;
    do {
        Ovl23DrawActorRecord(actor);
        Ovl23DrawDecisionRecord(record);
        record++;
        i++;
        actor++;
    } while (i < 6);
    Ovl23DrawRecordTrails();
}

/*
 * Two bounded record-trail rings, with reset, append and drawing services.
 * The 54 decision-record nodes and 12 actor nodes have private cursors and
 * share the same marker-sprite protocol.
 *
 * X positions are Q12; lane Y is a pixel coordinate and -1 disables a node.
 * Drawing uses the scene camera and the shared marker sprite without
 * advancing either ring. Push cursors must be initialized and nonnegative;
 * upper-bound wrap happens before writing, with no lower-bound check.
 */

void Ovl23ResetRecordTrails(void) {
    s16 i;

    for (i = 0; i < 0x36; i++) {
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNodes[i].xQ12 = 0x17000;
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNodes[i].laneY = -1;
        if (i < 6) {
            ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNodes[i].laneY = ((u16 *)&gOvl23RecordAnchorY)[i];
        }
    }
    ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNext = 6;
    for (i = 0; i < 0xC; i++) {
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNodes[i].xQ12 = 0xFFFE5000;
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNodes[i].laneY = -1;
        if (i < 6) {
            ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNodes[i].laneY = ((u16 *)&gOvl23RecordAnchorY)[i];
        }
    }
    ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNext = 6;
}

void Ovl23PushDecisionTrailNode(s32 xQ12, s16 laneY, s16 state) {
    if (gOvl23RecordTrails.decisionNext >= 0x36) {
        gOvl23RecordTrails.decisionNext = 0;
    }
    gOvl23RecordTrails.decisionNodes[gOvl23RecordTrails.decisionNext].xQ12 = xQ12;
    gOvl23RecordTrails.decisionNodes[gOvl23RecordTrails.decisionNext].laneY = laneY;
    gOvl23RecordTrails.decisionNodes[gOvl23RecordTrails.decisionNext].state = state;
    gOvl23RecordTrails.decisionNext++;
}

void Ovl23PushActorTrailNode(s32 xQ12, s16 laneY, s16 state) {
    if (gOvl23RecordTrails.actorNext >= 0xC) {
        gOvl23RecordTrails.actorNext = 0;
    }
    gOvl23RecordTrails.actorNodes[gOvl23RecordTrails.actorNext].xQ12 = xQ12;
    gOvl23RecordTrails.actorNodes[gOvl23RecordTrails.actorNext].laneY = laneY;
    gOvl23RecordTrails.actorNodes[gOvl23RecordTrails.actorNext].state = state;
    gOvl23RecordTrails.actorNext++;
}

/* Cull outside half the screen width and truncate signed Q12 X toward zero.
 * Preserve the repeated frame product and the zero-valued comparison term:
 * CSE/flow/combine see them at different stages, so simplifying them changes
 * operand order and live ranges even though the final arithmetic is equal.
 * Mechanism: notes/research/ovl_23_func_800B954C-variable-partition.md. */
void Ovl23DrawRecordTrails(void) {
    s16 i;
    s32 frame;
    s32 kind;
    s16 x;
    s32 position;
    s32 roundedCamera;
    s32 distance;
    s32 test;
    s32 cam;
    s32 delta;
    ObjectState *sprite;
    u8 *base;
    Ovl23TrailWork *walk;

    i = 0;
    base = Ovl23_D_800BF87C_draw;
    sprite = (ObjectState *)(base + 0x3B4);
    walk = (Ovl23TrailWork *)base;
    do {
        kind = i * 8;
        if (walk->actorNodes[0].laneY != -1) {
            position = ((Ovl23TrailWork *)(base + kind))->actorNodes[0].xQ12;
            kind = walk->actorNodes[0].state;
            frame = kind;
            test = (s16)(i / 12);
            roundedCamera = i - test * 12;
            roundedCamera = roundedCamera < 6;
            roundedCamera ^= 1;
            test = roundedCamera * 8;
            test = frame + roundedCamera * 8;
            test = (u32)test << 16;
            frame = test >> 16;
            kind = position;
            if (position < 0) {
                kind = position + 4095;
            }
            distance = kind >> 12;
            cam = *(s32 *)(base + 0x10);
            delta = distance - cam / 4096;
            distance = abs(delta);
            distance = HWD0 / 2 < distance;
            if (!distance) {
                distance = (position | cam) < 0;
                x = HWD0 / 2 - delta + (distance >> 1);
                SelectSpriteAnimation(sprite, 4);
                SelectSpriteAnimationFrame(sprite, frame & 0xFF);
                DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0xD0, (SpriteSourceData *)sprite, x, walk->actorNodes[0].laneY);
            }
        }
        walk = (Ovl23TrailWork *)((u8 *)walk + 8);
        i++;
        test = i < 12;
    } while (test);
    i = 0;
    base = Ovl23_D_800BF87C_draw;
    sprite = (ObjectState *)(base + 0x3B4);
    walk = (Ovl23TrailWork *)base;
    do {
        distance = i * 8;
        if (walk->decisionNodes[0].laneY != -1) {
            position = ((Ovl23TrailWork *)(base + distance))->decisionNodes[0].xQ12;
            kind = walk->decisionNodes[0].state;
            kind ^= 6;
            frame = kind == 0;
            test = (s16)(i / 12);
            roundedCamera = i - test * 12;
            roundedCamera = roundedCamera < 6;
            roundedCamera ^= 1;
            test = roundedCamera * 2;
            frame |= test;
            kind = position;
            if (position < 0) {
                kind = position + 4095;
            }
            distance = kind >> 12;
            cam = *(s32 *)(base + 0x10);
            delta = distance - cam / 4096;
            distance = abs(delta);
            distance = HWD0 / 2 < distance;
            if (!distance) {
                distance = (position | cam) < 0;
                x = HWD0 / 2 - delta + (distance >> 1);
                SelectSpriteAnimation(sprite, 3);
                SelectSpriteAnimationFrame(sprite, frame & 0xFF);
                DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0xD0, (SpriteSourceData *)sprite, x, walk->decisionNodes[0].laneY);
            }
        }
        walk = (Ovl23TrailWork *)((u8 *)walk + 8);
        i++;
        test = i < 54;
    } while (test);
}

/* Keep the original retry-index update: a collision resets it to zero,
 * then the loop increments it before the next comparison. Do not replace
 * this with a shuffled permutation or promise six distinct outputs. */
void Ovl23InitRecordChoices(void) {
    s32 i;
    s32 a0;
    s32 a1;

    for (i = 0; i < 6; i++) {
        D_800BF892[i] = 0xFF;
        a0 = Rand(10);
        a1 = 0;
        while (a1 <= i) {
            if (D_800BF892[a1] == a0) {
                a0++;
                if (a0 >= 10) {
                    a0 = 0;
                }
                a1 = 0;
            }
            a1++;
        }
        D_800BF892[i] = a0;
    }
}

void Ovl23ReturnActorToAnchor(Ovl23Func800BB0C8Arg *arg0) {
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_a0;
    s32 temp_a2;

    temp_s2 = (Ovl23_D_800BF87C_bounds.unk3B2 << 0xC) / 10;
    temp_s1 = (Ovl23_D_800BF87C_bounds.unk39C + 0x37) << 0xC;
    temp_a0 = *(s32 *)((u8 *)arg0 + 0x14);
    if (temp_a0 < temp_s1) {
        Ovl23SetActorControl(arg0, 1, 2, 0);
        *(s32 *)((u8 *)arg0 + 0x14) += temp_s2;
        if (temp_s1 < *(s32 *)((u8 *)arg0 + 0x14)) {
            *(s32 *)((u8 *)arg0 + 0x14) = temp_s1;
        }
    } else if (temp_s1 < temp_a0) {
        Ovl23SetActorControl(arg0, 1, 6, 0);
        *(s32 *)((u8 *)arg0 + 0x14) -= temp_s2;
        if (*(s32 *)((u8 *)arg0 + 0x14) < temp_s1) {
            *(s32 *)((u8 *)arg0 + 0x14) = temp_s1;
        }
    }
    temp_a2 = *((s16 *)((u8 *)&gOvl23RecordAnchorY + (*(s16 *)((u8 *)arg0 + 0) * 2))) << 0xC;
    if (*(s32 *)((u8 *)arg0 + 0x18) < temp_a2) {
        *(s32 *)((u8 *)arg0 + 0x18) += temp_s2;
        if (temp_a2 < *(s32 *)((u8 *)arg0 + 0x18)) {
            *(s32 *)((u8 *)arg0 + 0x18) = temp_a2;
        }
    } else if (temp_a2 < *(s32 *)((u8 *)arg0 + 0x18)) {
        *(s32 *)((u8 *)arg0 + 0x18) -= temp_s2;
        if (*(s32 *)((u8 *)arg0 + 0x18) < temp_a2) {
            *(s32 *)((u8 *)arg0 + 0x18) = temp_a2;
        }
    }
    if ((*(s32 *)((u8 *)arg0 + 0x18)) == temp_a2 && (*(s32 *)((u8 *)arg0 + 0x14)) == temp_s1) {
        Ovl23SetActorControl(arg0, 2, 2, 0);
    }
}

void Ovl23ReturnDecisionRecordToAnchor(Ovl23Func800BB0C8Arg *arg0) {
    s32 temp_a0;
    s32 temp_s0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0xC);
    temp_s0 = (Ovl23_D_800BF87C_bounds.unk39C + 0x77) << 12;
    if (temp_a0 < temp_s0) {
        Ovl23SetAnchorRecordControl(arg0, 2, 2, 0);
        temp_v1 = (*(s32 *)((u8 *)arg0 + 0xC)) + ((Ovl23_D_800BF87C_bounds.unk3A0 << 12) / 10);
        *(s32 *)((u8 *)arg0 + 0xC) = temp_v1;
        if (temp_s0 < temp_v1) {
            *(s32 *)((u8 *)arg0 + 0xC) = temp_s0;
        }
    } else if (temp_s0 < temp_a0) {
        Ovl23SetAnchorRecordControl(arg0, 2, 6, 0);
        temp_v1_2 = (*(s32 *)((u8 *)arg0 + 0xC)) - ((Ovl23_D_800BF87C_bounds.unk3A0 << 12) / 10);
        *(s32 *)((u8 *)arg0 + 0xC) = temp_v1_2;
        if (temp_v1_2 < temp_s0) {
            *(s32 *)((u8 *)arg0 + 0xC) = temp_s0;
        }
    }
    if ((*(s32 *)((u8 *)arg0 + 0xC)) == temp_s0) {
        Ovl23SetAnchorRecordControl(arg0, 1, 6, 0);
    }
}

/*
 * Eight-direction movement for the six-record arena. Clearance projects only
 * Y against per-record bounds, then checks angular sectors of nearby anchors
 * and actors using the CURRENT position, not the projected position. This is
 * not a swept collision test. Direction codes: +Y, +Y+X, +X, -Y+X, -Y, -Y-X,
 * -X, +Y-X. Membership and boundaries: notes/file-groupings.md.
 */
s32 Ovl23CheckDirectionalClearance(Ovl23PositionedRecord *record, s32 directionWord, s32 distanceWord);
s32 Ovl23PlanarDistance(s16 x0, s16 y0, s16 x1, s16 y1);
s32 Ovl23AngleDegreesFromVector(s32 dx, s32 dy);
s32 Ovl23DirectionFromAngleDegrees(s32 degrees);

void Ovl23StepRecordInDirection(Ovl23PositionedRecord *record) {
    s16 state;
    s16 direction;
    s32 x;
    s32 diagonal;
    s32 limit;
    s32 dy;
    s32 dx;
    s32 speed;

    state = *(s16 *)((u8 *)record + 4);
    switch (state) {
    case 1:
        speed = *(s32 *)((u8 *)record + 0x10);
        break;
    case 3:
        speed = (Ovl23_D_800BF87C_bounds.unk3B2 << 12) / 10;
        break;
    default:
        return;
    }
    diagonal = (speed * 1000) / 1414;
    if (Ovl23CheckDirectionalClearance(record, *(s16 *)((u8 *)record + 6),
            Ovl23_D_800BF87C_bounds.unk39E) == 1) {
        dx = 0;
        direction = *(s16 *)((u8 *)record + 6);
        dy = 0;
        switch (direction) {
        case 0:
            dy = speed;
            break;
        case 1:
            dy = diagonal;
            dx = dy;
            break;
        case 2:
            dx = speed;
            break;
        case 3:
            dy = -diagonal;
            dx = diagonal;
            break;
        case 4:
            dy = -speed;
            break;
        case 5:
            dy = -diagonal;
            dx = dy;
            break;
        case 6:
            dx = -speed;
            break;
        case 7:
            dy = diagonal;
            dx = -dy;
            break;
        }
        x = record->xQ12 + dx;
        record->xQ12 = x;
        record->yQ12 += dy;
        limit = (Ovl23_D_800BF87C_bounds.unk39C + 0x37) << 12;
        if (x >= limit) {
            record->xQ12 = limit;
        }
        if (record->xQ12 <= -0x30000) {
            record->xQ12 = -0x30000;
        }
    }
}

s32 Ovl23ChooseRecordDirection(Ovl23PositionedRecord *record) {
    s32 x;
    s32 dx;
    s32 dy;
    s32 direction;
    Ovl23D87CView210 *arena;

    x = record->xQ12 / 4096;
    arena = (Ovl23D87CView210 *)Ovl23_D_800BF87C_draw;
    dx = x - arena->unk210[record->index].unk0 / 4096;
    dy = record->yQ12 / 4096 - ((s16 *)&gOvl23RecordAnchorY)[record->index];
    /* The discarded square-root call is present in the original. */
    SquareRoot0(dx * dx + dy * dy);
    direction = Ovl23DirectionFromRecordAnchor(record);
    if (Ovl23CheckDirectionalClearance(record, direction, (s16)(*(s16 *)((u8 *)arena + 0x39E) * 3 / 2)) != 1) {
        direction = (s16)(direction + 2 + Rand(2) * 2);
        direction = (s16)(direction % 8);
    }
    return direction;
}

s32 Ovl23CheckDirectionalClearance(Ovl23PositionedRecord *record, s32 directionWord, s32 distanceWord) {
    /* Callers pass promoted words; narrowing belongs here, not at every call. */
    s16 direction = directionWord;
    s16 distance = distanceWord;
    s16 y;
    s32 i;

    switch (direction) {
    case 3:
    case 5:
        y = record->yQ12 / 4096 - distance * 1000 / 1414;
        break;
    case 4:
        y = record->yQ12 / 4096 - distance;
        break;
    case 1:
    case 7:
        y = record->yQ12 / 4096 + distance * 1000 / 1414;
        break;
    case 0:
        y = distance + record->yQ12 / 4096;
        break;
    default:
        y = record->yQ12 / 4096;
        break;
    }
    if (y < ((s16 *)gOvl23RecordYBounds)[record->index * 2]) {
        return 0;
    }
    if (((s16 *)gOvl23RecordYBounds)[record->index * 2 + 1] < y) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        if (i != record->index) {
            if (distance >= Ovl23PlanarDistance(record->xQ12 / 4096,
                    record->yQ12 / 4096, Ovl23_D_800BF87C_movement.records[i].xQ12 / 4096,
                    ((s16 *)&gOvl23RecordAnchorY)[i])) {
                if (direction == Ovl23DirectionFromAngleDegrees(Ovl23AngleDegreesFromVector(
                        record->xQ12 / 4096 - Ovl23_D_800BF87C_movement.records[i].xQ12 / 4096,
                        record->yQ12 / 4096 - ((s16 *)&gOvl23RecordAnchorY)[i]))) {
                    return 0;
                }
            }
        }
    }
    for (i = 0; i < 6; i++) {
        if (i != record->index) {
            if (distance >= Ovl23PlanarDistance(record->xQ12 / 4096,
                    record->yQ12 / 4096,
                    Ovl23_D_800BF87C_movement.actors[i].unk38 / 4096,
                    Ovl23_D_800BF87C_movement.actors[i].unk3C / 4096)) {
                if (direction == Ovl23DirectionFromAngleDegrees(Ovl23AngleDegreesFromVector(
                        record->xQ12 / 4096 - Ovl23_D_800BF87C_movement.actors[i].unk38 / 4096,
                        record->yQ12 / 4096 - Ovl23_D_800BF87C_movement.actors[i].unk3C / 4096))) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

s32 Ovl23DirectionFromRecordAnchor(Ovl23PositionedRecord *record) {
    s16 index;
    s32 adjustedY;
    s32 adjustedX;
    s32 adjustedAnchorX;
    s32 x;
    s32 anchorX;
    s32 dx;

    adjustedX = record->xQ12;
    if (adjustedX < 0) {
        adjustedX += 0xFFF;
    }
    x = adjustedX >> 12;
    index = record->index;
    adjustedAnchorX = Ovl23_D_800BF87C_movement.records[index].xQ12;
    if (adjustedAnchorX < 0) {
        adjustedAnchorX += 0xFFF;
    }
    anchorX = adjustedAnchorX >> 12;
    dx = x - anchorX;
    adjustedY = record->yQ12;
    if (adjustedY < 0) {
        adjustedY += 0xFFF;
    }
    return Ovl23DirectionFromAngleDegrees(Ovl23AngleDegreesFromVector(dx,
        (adjustedY >> 12) - *(s16 *)((u8 *)&gOvl23RecordAnchorY + (index * 2))));
}

/* Seven 45-degree sectors starting at 23; the wraparound sector returns 4. */
s32 Ovl23DirectionFromAngleDegrees(s32 degrees) {
    if ((u32)(degrees - 0x17) < (u32)0x2D) {
        return 5;
    }
    if ((u32)(degrees - 0x44) < (u32)0x2D) {
        return 6;
    }
    if ((u32)(degrees - 0x71) < (u32)0x2D) {
        return 7;
    }
    if ((u32)(degrees - 0x9E) < (u32)0x2D) {
        return 0;
    }
    if ((u32)(degrees - 0xCB) < (u32)0x2D) {
        return 1;
    }
    if ((u32)(degrees - 0xF8) < (u32)0x2D) {
        return 2;
    }
    if ((u32)(degrees - 0x125) < (u32)0x2D) {
        return 3;
    }
    return 4;
}

s32 Ovl23PlanarDistance(s16 x0, s16 y0, s16 x1, s16 y1) {
    s32 distance;

    distance = SquareRoot0((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
    return ((s16)distance);
}

s32 Ovl23AngleDegreesFromVector(s32 dx, s32 dy) {
    s32 angle;

    angle = ratan2(dx, dy);
    if (angle < 0) {
        angle += 0x1000;
    }
    return (angle * 0x168) / 0x1000;
}

/*
 * Table-driven random decisions for overlay 23's record controller.
 * The two private probability tables serve its weighted result selection
 * and subsequent threshold-bucket trials. No record state is modified.
 * Trial rows 0..3, record indices 0..5 and weight sets 0..1 are unchecked.
 * Each decision consumes one Rand(100); cumulative weights narrow to s16,
 * need not sum to 100, and select only when strictly greater than the draw.
 */

s32 Ovl23TestRecordRandomThreshold(s16 trialRow, s16 recordIndex) {
    s16 difference;
    s16 threshold;
    s32 bucket;
    u8 *state;

    state = Ovl23_D_800BF87C_draw;
    difference = (s16)(*(u16 *)(state + recordIndex * 0x50 + 0x30) -
                       *(u16 *)(state + recordIndex * 0x50 + 0x32));
    if (*(s16 *)(state + 0x3A4) < difference) {
        bucket = 0;
    } else if (*(s16 *)(state + 0x3A6) < difference) {
        bucket = 1;
    } else if (*(s16 *)(state + 0x3A8) < difference) {
        bucket = 2;
    } else if (*(s16 *)(state + 0x3AA) < difference) {
        bucket = 3;
    } else {
        bucket = 4;
    }
    threshold = gOvl23RecordRandomTrialThresholds[trialRow * 5 + bucket];
    return threshold > (s32)Rand(0x64);
}

s32 Ovl23ChooseRecordWeightedResult(s16 recordIndex, s16 weightSet) {
    s32 row;
    s16 cumulativeWeight;
    s16 draw;
    s32 i;
    s32 weight;
    u8 *state;
    s32 recordPositionQ12;
    s32 actorPositionQ12;
    s32 distance;
    s32 recordPosition;
    s32 distanceThreshold;
    u8 *categoryState;

    cumulativeWeight = 0;
    draw = (s16)Rand(0x64);
    if (weightSet == 0) {
        categoryState = Ovl23_D_800BF87C_draw;
        row = (u16)(((Ovl23D87CView50 *)categoryState)->unk0[recordIndex].unk28 - 1) < 2;
    } else {
        state = Ovl23_D_800BF87C_draw;
        recordPositionQ12 = ((Ovl23D87CView210 *)state)->unk210[recordIndex].unk0;
        /* Add the fractional mask before shifting negative Q12 values:
         * these conversions truncate toward zero, not toward minus infinity. */
        if (recordPositionQ12 < 0) {
            recordPositionQ12 += 0xFFF;
        }
        recordPosition = recordPositionQ12 >> 12;
        distanceThreshold = recordIndex * 0x50;
        actorPositionQ12 = ((Ovl23D87CView50 *)state)->unk0[recordIndex].unk38;
        if (actorPositionQ12 < 0) {
            actorPositionQ12 += 0xFFF;
        }
        distance = recordPosition - (actorPositionQ12 >> 12);
        distanceThreshold = *(s16 *)((u8 *)state + distanceThreshold + 0x2E);
        if (distance < 0) {
            distance = -distance;
        }
        row = (distance - distanceThreshold) < 0x65;
    }
    for (i = 0; i < 3; i++) {
        weight = *(u16 *)((u8 *)gOvl23RecordRandomWeights + row * 6 + weightSet * 12 + i * 2);
        cumulativeWeight = (s16)(cumulativeWeight + weight);
        if (draw < cumulativeWeight) {
            break;
        }
    }
    if (i == 0) {
        return 3;
    }
    if (i == 1) {
        return 2;
    }
    if (i == 2) {
        return 1;
    }
    return -1;
}

/*
 * Advance a decision record along the track in signed Q12 coordinates.
 * State 2/3 selects one of two speeds; direction 6 moves toward the lower end.
 * The endpoint tests are deliberately independent, and record 2 keeps its
 * control fields when clamped. Endpoints are -48 and trackExtent+119 world
 * units. Arithmetic must remain in signed s32 range.
 */
void Ovl23AdvanceDecisionRecordPosition(Ovl23Func800BB0C8Arg *record) {
    s32 step12;
    s32 upperEndpoint12;

    switch (record->unk4) {
    case 0:
        return;
    case 2:
        step12 = (Ovl23_D_800BF87C_bounds.unk3A0 << 0xC) / 10;
        break;
    case 3:
        step12 = (Ovl23_D_800BF87C_bounds.unk3A2 << 0xC) / 10;
        break;
    default:
        return;
    }

    if (record->unk6 == 6) {
        *(s32 *)((u8 *)record + 0xC) -= step12;
    } else {
        *(s32 *)((u8 *)record + 0xC) += step12;
    }
    upperEndpoint12 = (Ovl23_D_800BF87C_bounds.unk39C + 0x77) << 0xC;
    if (*(s32 *)((u8 *)record + 0xC) >= upperEndpoint12) {
        *(s32 *)((u8 *)record + 0xC) = upperEndpoint12;
        if (*(s16 *)((u8 *)record + 0) != 2) {
            Ovl23SetAnchorRecordControl(record, 1, 6, record->unk8);
        }
    }
    if (*(s32 *)((u8 *)record + 0xC) <= -0x30000) {
        *(s32 *)((u8 *)record + 0xC) = -0x30000;
        if (*(s16 *)((u8 *)record + 0) != 2) {
            Ovl23SetAnchorRecordControl(record, 1, 2, record->unk8);
        }
    }
}

/*
 * Record-scene drawing: select actor/decision animations, submit their
 * sprites and marker, sample trails, and tile the scrolling backdrop.
 * Projection and ordering-table selection are shared by the two record
 * drawers. Signed Q12 division truncates toward zero; projected X narrows
 * to s16 before testing +/-screenWidth, mapping outside values to -width.
 * Drawing still emits packets for that sentinel. Initialized sprite sources,
 * render context and sufficient packet/OT storage are required.
 * Actor states 1..10 and decision states 1..3 draw; others do nothing.
 * Ordinary draws advance animation once; fixed-frame actor states do not.
 * Palette offsets change, but positions and the shared tick do not. Outside
 * mode 1, sample actor trails at tick%30==29 and decision trails at tick%10==9.
 */

/* Internal helpers retain external linkage; original static linkage is unknown. */
s32 Ovl23ProjectRecordX(s32 xQ12);
s32 Ovl23OrderingTableForRecordY(s32 yQ12);

void Ovl23DrawActorRecord(Ovl23ActorRecord *record) {
    ObjectState *sprite;
    s16 stateIndex;
    s16 animation;
    s32 orderingTable;
    s32 screenX;
    s32 screenY;
    s32 fixedFrame;

    stateIndex = record->state - 1;
    fixedFrame = -1;
    switch (stateIndex) {
    case 0:
        animation = record->direction + 0xD;
        break;
    case 1:
        animation = record->direction + 0x15;
        break;
    case 2:
        animation = record->direction + 1;
        break;
    case 3:
        if (record->timer >= 0x1F) {
            animation = 0x29;
            fixedFrame = record->direction / 2;
        } else {
            animation = record->direction / 2 + 9;
        }
        break;
    case 4:
        animation = record->direction / 2 + 0x1D;
        break;
    case 5:
        animation = record->direction / 2 + 0x21;
        break;
    case 6:
        animation = 0x29;
        fixedFrame = record->direction / 2;
        break;
    case 7:
        if (record->timer >= 0x1F) {
            animation = 0x29;
            fixedFrame = record->direction / 2;
        } else {
            animation = record->direction / 2 + 0x2A;
        }
        break;
    case 8:
        animation = 0x32;
        fixedFrame = record->direction / 2;
        break;
    case 9:
        if (record->timer >= 0x1F) {
            animation = 0x32;
            fixedFrame = record->direction / 2;
        } else {
            animation = record->direction / 2 + 0x33;
        }
        break;
    default:
        return;
    }
    sprite = (ObjectState *) ((u8 *) record + 0x1C);
    if (record->animation != animation) {
        SelectSpriteAnimation(sprite, animation & 0xFF);
    }
    if (fixedFrame != -1) {
        SelectSpriteAnimationFrame(sprite, fixedFrame & 0xFF);
    }
    screenX = Ovl23ProjectRecordX(record->xQ12);
    screenY = (s16) (record->yQ12 / 0x1000);
    SetSpriteSourceTextureOffsets((Struct_800154CC *) sprite, 0, 0, record->paletteX, record->paletteY);
    orderingTable = Ovl23OrderingTableForRecordY(record->yQ12);
    /* Fixed-frame states do not advance the sprite's animation clock. */
    if (fixedFrame == -1) {
        DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *) sprite, screenX, screenY);
    } else {
        DrawSpriteFrameRects(orderingTable, (s32) sprite, record->animation, record->frame, screenX, screenY);
    }
    DrawUiSprite18Frame(orderingTable, screenX, screenY, 0);
    if ((((Ovl23D87CViewBAA60 *) Ovl23_D_800BF87C_draw)->unk4 != 1) &&
        (((Ovl23D87CViewBAA60 *) Ovl23_D_800BF87C_draw)->unk14 ==
         (((((Ovl23D87CViewBAA60 *) Ovl23_D_800BF87C_draw)->unk14 + 1) / 30) * 30) - 1)) {
        Ovl23PushActorTrailNode(record->xQ12, screenY, record->direction);
    }
}

void Ovl23DrawDecisionRecord(Ovl23DecisionRecord *record) {
    ObjectState *sprite;
    s16 screenY;
    s32 orderingTable;
    s32 screenX;
    u8 animation;

    if (record->state == 1) {
        animation = 0;
    } else if (record->state == 2) {
        animation = 2;
    } else if (record->state == 3) {
        animation = 4;
    } else {
        return;
    }
    if (record->direction == 6) {
        animation = (animation + 1) & 0xFF;
    }
    sprite = (ObjectState *) ((u8 *) record + 0x10);
    if (record->animation != animation) {
        SelectSpriteAnimation(sprite, animation);
    }
    screenX = Ovl23ProjectRecordX(record->xQ12);
    screenY = *((s16 *) ((u8 *) &gOvl23RecordAnchorY + (record->lane * 2)));
    orderingTable = Ovl23OrderingTableForRecordY(screenY << 0xC);
    SetSpriteSourceTextureOffsets((Struct_800154CC *) sprite, 0, 0, 0, record->paletteY);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *) sprite, screenX, screenY);
    DrawUiSprite18Frame(orderingTable, screenX, screenY, 0);
    if ((((Ovl23D87CViewBAA60 *)Ovl23_D_800BF87C_draw)->unk4 != 1) &&
        (((Ovl23D87CViewBAA60 *)Ovl23_D_800BF87C_draw)->unk14 ==
         (((((Ovl23D87CViewBAA60 *)Ovl23_D_800BF87C_draw)->unk14 + 1) / 10) * 10) - 1)) {
        Ovl23PushDecisionTrailNode(record->xQ12, screenY, record->direction);
    }
}

void Ovl23DrawScrollingRecordBackdrop(void) {
    s16 x;
    s16 tileX64;
    s16 tileX76;
    s32 position;
    s32 screenWidth;
    s32 lowerOffset;
    s32 upperOffset;
    u8 *base;
    u8 *tiles64;
    u8 *tiles76;
    u8 *endBase;
    u8 *loopBase;
    ObjectState *sprite;
    ObjectState *sprite64;
    ObjectState *sprite76;
    ObjectState *endSprite;
    ObjectState *loopSprite;

    /* Keep the two tile walks separate: 64-pixel and 76-pixel layers use
     * different scroll rates and ordering-table slots. */
    sprite64 = (ObjectState *)gOvl23SceneSpriteSource;
    SelectSpriteAnimation(sprite64, 0);
    SelectSpriteAnimationFrame(sprite64, 0);
    tiles64 = (u8 *)sprite64 - 0x3B4;
    position = *(s32 *)(tiles64 + 0x10);
    tileX64 = position / 4096 - (position / 262144) * 64 - 64;
    if (tileX64 < HWD0 + 64) {
        do {
            s32 packet = gActiveRenderContext->field_D8 + 0xD8;
            loopSprite = sprite64;
            loopBase = tiles64;
            DrawSpriteFrameRects(packet, (s32)loopSprite,
                         loopBase[0x3B8], loopBase[0x3B9], tileX64, 0);
            tileX64 += 64;
        } while (tileX64 < HWD0 + 64);
    }
    sprite76 = (ObjectState *)gOvl23SceneSpriteSource;
    SelectSpriteAnimation(sprite76, 1);
    tiles76 = (u8 *)sprite76 - 0x3B4;
    tileX76 = (*(s32 *)(tiles76 + 0x10) / 4096 * 3) -
        (*(s32 *)(tiles76 + 0x10) / 4096 * 3 / 76) * 76 - 76;
    if (tileX76 < HWD0 + 76) {
        do {
            s32 packet = gActiveRenderContext->field_D8 + 0xCC;
            loopSprite = sprite76;
            loopBase = tiles76;
            DrawSpriteFrameRects(packet, (s32)loopSprite,
                         loopBase[0x3B8], loopBase[0x3B9], tileX76, 0);
            tileX76 += 76;
        } while (tileX76 < HWD0 + 76);
    }
    base = Ovl23_D_800BF87C_draw;
    screenWidth = HWD0;
    if (-screenWidth / 2 - 32 < *(s32 *)(base + 0x10) / 4096 &&
        *(s32 *)(base + 0x10) / 4096 < screenWidth / 2 + 32) {
        sprite = (ObjectState *)(base + 0x3B4);
        SelectSpriteAnimation(sprite, 0);
        SelectSpriteAnimationFrame(sprite, 1);
        /* Unlike record X, marker X narrows again after subtracting 32. */
        x = HWD0 / 2 + *(s32 *)(base + 0x10) / 4096;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0xD4, (s32)sprite,
                     base[0x3B8], base[0x3B9], (s16)(x - 32), 0);
        SelectSpriteAnimation(sprite, 2);
        x = HWD0 / 2 + *(s32 *)(base + 0x10) / 4096 + 64;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0xD4, (s32)sprite,
                     base[0x3B8], base[0x3B9], (s16)(x - 32), 0);
    }
    endBase = Ovl23_D_800BF87C_draw;
    screenWidth = HWD0;
    if (-screenWidth / 2 + (lowerOffset = ((Ovl23D87CView39C *)endBase)->unk39C - 32) <
        *(s32 *)(endBase + 0x10) / 4096 &&
        *(s32 *)(endBase + 0x10) / 4096 <
        screenWidth / 2 + (upperOffset = ((Ovl23D87CView39C *)endBase)->unk39C + 32)) {
        endSprite = (ObjectState *)(endBase + 0x3B4);
        SelectSpriteAnimation(endSprite, 0);
        SelectSpriteAnimationFrame(endSprite, 1);
        x = HWD0 / 2 + *(s32 *)(endBase + 0x10) / 4096 -
            ((Ovl23D87CView39C *)endBase)->unk39C;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0xD4, (s32)endSprite,
                     endBase[0x3B8], endBase[0x3B9], (s16)(x - 32), 0);
    }
}

/* Return an OT-slot address, not an index; valid Y/OT storage is required. */
s32 Ovl23OrderingTableForRecordY(s32 yQ12) {
    s32 byteOffset;

    byteOffset = (0x28 - (yQ12 / 4096 - 0x40) / 4) * 4;
    return gActiveRenderContext->field_D8 + byteOffset;
}

s32 Ovl23ProjectRecordX(s32 xQ12) {
    s32 screenX;

    screenX = (s16)((HWD0 / 2) - (xQ12 / 4096 - Ovl23_D_800BF87C_header.cameraXQ12 / 4096));
    if (screenX < -HWD0 || screenX > HWD0) {
        return (s16)(-(u16)HWD0);
    }
    return screenX;
}

/* Actor/anchor-record control-prefix writers. Both store state, direction
 * and a caller-defined timer/control value, but remain distinct external
 * entries for their different client families. No private linkage is inferred.
 * Inputs narrow to low 16 bits; record prefixes and other fields are untouched. */

void Ovl23SetActorControl(Ovl23Func800BB0C8Arg *record,
                          s32 state, s32 direction, s32 controlValue) {
    record->unk4 = state;
    record->unk6 = direction;
    record->unk8 = controlValue;
}

void Ovl23SetAnchorRecordControl(Ovl23Func800BB0C8Arg *record,
                                 s32 state, s32 direction, s32 controlValue) {
    record->unk4 = state;
    record->unk6 = direction;
    record->unk8 = controlValue;
}

/*
 * Sprite-bank bring-up and pause services for this overlay's record arena.
 * Per-record sources live at arena+0x40 (six, stride 0x50) and arena+0x214
 * (six, stride 0x44); the scene source is arena+0x3B4. The loader and flag
 * walkers deliberately retain their different base-relative expressions.
 * Loads block on CD completion, clear the active OT and share graphics-heap
 * scratch at +0x4290. The scene source and both arrays must be initialized
 * before drawing or toggling pause. Mask 4 suppresses animation updates,
 * not drawing of already-selected frames; the scene source is not paused.
 */
void Ovl23PauseActorSprites(void) {
    u8 *sources44;
    u8 *sources50;
    u32 base;
    s32 i;

    base = (u32)gOvl23ActorSpriteSourceBase;
    sources50 = (u8 *)(base - 0x1D4);
    sources44 = (u8 *)base;
    for (i = 5; i >= 0; i--) {
        AddSpriteSourceFlags((u16 *)sources44, 4);
        AddSpriteSourceFlags((u16 *)sources50, 4);
        sources50 += 0x50;
        sources44 += 0x44;
    }
}

void Ovl23ResumeActorSprites(void) {
    u8 *sources44;
    u8 *sources50;
    u32 base;
    s32 i;

    base = (u32)gOvl23ActorSpriteSourceBase;
    sources50 = (u8 *)(base - 0x1D4);
    sources44 = (u8 *)base;
    for (i = 5; i >= 0; i--) {
        ClearSpriteSourceFlags((u16 *)sources44, 4);
        ClearSpriteSourceFlags((u16 *)sources50, 4);
        sources50 += 0x50;
        sources44 += 0x44;
    }
}

void Ovl23DrawPausedSpriteFrame(void) {
    u8 *arena;

    SelectSpriteAnimation((ObjectState *)&gOvl23SceneSpriteSource, 5);
    arena = gOvl23SceneSpriteSource - 0x3B4;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl23SceneSpriteSource, arena[0x3B8],
                  arena[0x3B9], 0, 0);
}

void Ovl23LoadSharedSpriteBank(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl23SharedSpriteBank, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData(&gOvl23SharedBankSpriteSource, (SpriteDataHeader *)gOvl23SharedSpriteBank);
}

void Ovl23LoadSpriteImageBank(void) {
    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}

/* Byte-aligned bank assignment preserves the original unaligned copy, unlike
 * the shared bank's two separately bounded memcpy operations above. */
typedef struct {
    u8 bytes[0x29A8];
} Ovl23SpriteBankBlob;

void Ovl23LoadActorSpriteBank(void) {
    s32 i;
    s32 headerOffsetIndex;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x18, 0, 0x1EC1C, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x18, 0, 0x1EC1C, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x6C38));
    *(Ovl23SpriteBankBlob *)gOvl23ActorSpriteBank = *(Ovl23SpriteBankBlob *)(gGraphicsHeapBase + 0x4290);
    InitSpriteSourceData((SpriteSourceData *)gOvl23SceneSpriteSource, (SpriteDataHeader *)(gOvl23ActorSpriteBank + gOvl23ActorSpriteHeaderOffsets[3]));
    for (i = 0; i < 6; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl23SceneSpriteSource + i * 0x50 - 0x374), (SpriteDataHeader *)(gOvl23ActorSpriteBank + gOvl23ActorSpriteHeaderOffsets[0]));
        headerOffsetIndex = 8;
        if (i == 2) {
            headerOffsetIndex = 4;
        }
        InitSpriteSourceData((SpriteSourceData *)(gOvl23SceneSpriteSource + i * 0x44 - 0x1A0), (SpriteDataHeader *)(gOvl23ActorSpriteBank + *(s32 *)((u8 *)gOvl23ActorSpriteHeaderOffsets + headerOffsetIndex)));
    }
}

void Ovl23QueueSoundBankLoads(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x12, 1);
}

/*
 * Bounded increments of adjacent halfwords in indexed 0x1D4-byte work
 * records. B87F8 calls both services for its two lists of record indices.
 * The gameplay meaning of the values is not established.
 *
 * At +0x19EA a signed value below 235 gains 20 (low 16 bits), otherwise
 * becomes 255: ordinary saturation assumes nonnegative byte-range input.
 * At +0x19EC an unsigned value <=65034 gains 500, otherwise becomes 65535.
 * Record indices are unchecked. The ovl_25 twins are separate identities.
 */

void Ovl23RaiseRecordByteRangeValue(s16 recordIndex) {
    char *work;
    char *record;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    if (*(s16 *)(record + 0x8000 + 0x19EA) < 0xEB) {
        *(s16 *)(record + 0x8000 + 0x19EA) = (s16)(0x14 + *(u16 *)(record + 0x8000 + 0x19EA));
    } else {
        *(s16 *)(record + 0x8000 + 0x19EA) = 0xFF;
    }
}

void Ovl23RaiseRecordHalfwordRangeValue(s16 recordIndex) {
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
