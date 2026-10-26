/*
 * Record-run scene: staged resource setup, a five-subphase/seven-stage
 * dispatch protocol, message prompts, pad-driven simulation and composition.
 * Six 0x50-byte actors and six 0x44-byte decision records share one work
 * aggregate with the trail rings. Positions and movement use Q12 units;
 * stage timers count update calls, not elapsed milliseconds.
 *
 * This is the scene/control half of a larger supported unit. A four-byte
 * unrepresented rodata gap at overlay file offset 0x18 prevents pooling its
 * first switch table with direction_movement's tables. This mechanical cut
 * at 800B9B18 is NOT evidence of an original file boundary. The remaining
 * helpers and resource/movement modules belong to the same closed scene;
 * notes/file-groupings.md records the whole-unit evidence and merge blockers.
 */

#include "common.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl23_record_value_increments.h"
#include "ovl23_sprite_resources.h"
#include "ovl23_record_trails.h"
#include "game_types.h"
#include "random.h"
#include "psyq/abs.h"
#include "psyq/stddef.h"
#include "psyq/sys/types.h"
#include "psyq/libgte.h"
#include "psyq/stdlib.h"
#include "ovl23_record_random_selection.h"
#include "ovl23_direction_movement.h"
#include "ovl23_record_control_fields.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl23_record_run.h"

void func_80017A48(u32 arg0);
u32 func_80017A64(void);

void Ovl23DispatchRecordRunSubphase(void) {
    s32 *base;
    u32 savedDrawMode;

    savedDrawMode = func_80017A64();
    func_80017A48(3U);
    base = &gOvl23RecordRunSubphases;
    ((void (*)(s32 *))base[gSceneSubphase])(base);
    func_80017A48(savedDrawMode);
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
    state->unk10 = 0x40000;
    state->unk14 = 0;
    Ovl23InitDecisionRecords();
    Ovl23InitActorRecords();
    Ovl23ResetRecordTrails();
}

void Ovl23InitDecisionRecords(void) {
    s32 i = 0;
    s32 one = 1;
    u16 *src = D_800BBA7C;
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
        ((struct Ovl23ScreenInitTail *)pr)->unk70 = D_800BBA64[pairIndex];
        ((struct Ovl23ScreenInitTail *)pr)->unk72 = D_800BBA64[pairIndex + 1];
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
        pr->unk2E = D_800BBA90[(s16)pr->unk32 / 50];
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

void func_80017A08(s32 arg0, s32 arg1);
/* Executable imports SetVal8005E2BC and SetMessagePortraitEnabled. */
void SetVal8005E2BC(s32 arg0);

void Ovl23UpdateRecordStage4(void) {
    Ovl23UpdateRecordRunFrame();
    func_80017A08(0, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[0]]);
    func_80017A08(1, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[1]]);
    func_80017A08(2, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[3]]);
    func_80017A08(3, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[5]]);
    RequestMessageWindow(4, 0x2D);
    if (GetMessageWindowStatus() == 2) {
        CloseMessageWindow();
        SetVal8005E2BC(0);
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
        SetVal8005E2BC(0);
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
                if (((struct Ovl23ActorDecisionView *)state)->actors[2].unk14 ==
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

        if (((struct Ovl23ActorDecisionView *)camera)->records[2].unkC > 0x3FFFF) {
            camera->unk10 = ((struct Ovl23ActorDecisionView *)camera)->records[2].unkC;
        }
        if (camera->unk10 <= 0x40000) {
            camera->unk10 = 0x40000;
        } else {
            result = ((struct Ovl23ActorDecisionView *)camera)->unk39C << 12;
            if (camera->unk10 >= result) {
                camera->unk10 = result;
            }
        }
        Ovl23UpdateRecordRunFrame();
    }
}

void ovl_23_func_800BA610 (Ovl23Func800BB0C8Arg *arg0);

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
    ovl_23_func_800BA610(&D_800BFB08);
}
/* Warning: struct GfxObj is not defined (only forward-declared) */

void ovl_23_func_800BA610(Ovl23Func800BB0C8Arg *arg0);
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
                    diff = base->actors[i].unk38 / 4096 - base->records[i].unkC / 4096;
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
                ovl_23_func_800BA610((Ovl23Func800BB0C8Arg *)var_s1);
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
            if (actor->unk8 > 0) {
                actor->unk8--;
            } else if (base->unk14 == (base->unk14 / base->unk3AC) * base->unk3AC) {
                dx = base->actors[i].unk14 / 4096 - base->records[i].unkC / 4096;
                dy = base->actors[i].unk18 / 4096 - table[i];
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
        if (window->actors[0].unk4 == walkingState &&
            window->actors[0].unk8 == (window->actors[0].unk8 / base2->unk3AE) * base2->unk3AE) {
            sampledDirection = Rand(5) + 4;
            direction = sampledDirection;
            sampledDirection = 8;
            direction = direction == sampledDirection ? 0 : direction;
            Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)decisionActor, window->actors[0].unk4,
                direction, window->actors[0].unk8);
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

void ovl_23_func_800BABD8(void);
void ovl_23_func_800BA73C(Ovl23BA73CArg *arg0);
void ovl_23_func_800BAA60(Ovl23BAA60Arg *arg0);

void Ovl23UpdateRecordRunFrame(void) {
    Ovl23BA73CArg *actor;
    Ovl23BAA60Arg *record;
    Ovl23BA73CArg *base;
    s32 i;

    ovl_23_func_800BABD8();
    i = 0;
    base = (Ovl23BA73CArg *)D_800BF8A0;
    record = (Ovl23BAA60Arg *)(base + 6);
    actor = base;
    do {
        ovl_23_func_800BA73C(actor);
        ovl_23_func_800BAA60(record);
        record++;
        i++;
        actor++;
    } while (i < 6);
    Ovl23DrawRecordTrails();
}

/*
 * record_trails — reconstructed translation unit.
 *
 * Two bounded record-trail rings, with reset, append and drawing services.
 * The 54 decision-record nodes and 12 actor nodes have private cursors and
 * share the same marker-sprite protocol.
 *
 * X positions are Q12; lane Y is a pixel coordinate and -1 disables a node.
 * Drawing uses the scene camera and the shared marker sprite without
 * advancing either ring. Interface: ovl23_record_trails.h.
 * Membership and boundaries: notes/file-groupings.md.
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
