/*
 * scene_state_machine — phase/mode dispatch and the scene-record lifecycle.
 *
 * The five-phase engine callback drives loading, fades and the three-mode
 * event/dialogue/active protocol. Its record initializers and frame helpers
 * operate on six scene and two auxiliary records at 0x78-byte strides.
 * Mode and phase callback tables, randomized record-state decisions and their
 * threshold/weight tables are closed over this unit. Event scripts and the
 * shared state/timer setter remain external dependencies.
 *
 * All timers count update calls. Selectors, record indices and tuning
 * divisors are unchecked; the shared scene/CD/render state is non-reentrant.
 * Public contracts are in ovl25_scene_state_machine.h. The choice helpers keep
 * external linkage but are called only inside this unit. Their record IDs,
 * choice indices and signed weight-category indexing are unchecked.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl25_scene_state_machine.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "sprite_source.h"
#include "ovl25_sprite_set_service.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl25_vram_animation_slots.h"
#include "ovl25_background_color_cycle.h"
#include "ovl25_scene_display_pipeline.h"
#include "ovl25_fixed_point_tier_lookup.h"
#include "random.h"
#include "ovl25_record_value_increments.h"
#include "ovl25_record_state_timer.h"
#include "ovl25_record_tier_update.h"
#include "psyq/libapi.h"
#include "text_engine.h"

/* Internal helpers retain external linkage and the original callback ABI. */
void Ovl25ResetSceneState(void);
void Ovl25InitSceneRecordState(void);
void Ovl25DrawSceneSprites(void);

void Ovl25DispatchScenePhase(void) {
    s32 *work;
    s32 *phaseTable;
    u32 previousTextDrawMode;

    previousTextDrawMode = GetTextPalette();
    SetTextPalette(3U);
    phaseTable = gOvl25ScenePhaseTable;
    work = (s32 *) &D_8006C838;
    /* Keep the original word-view dispatch and forwarded table argument;
     * every phase handler ignores that argument. */
    ((void (*)(s32 *)) phaseTable[work[0x448C >> 2]])(phaseTable);
    SetTextPalette(previousTextDrawMode);
    if (work[0xC >> 2] & 0x08000000) {
        work[0xC >> 2] &= 0xF7FFFFFF;
    }
}

void Ovl25LoadSceneStageAndInit(void) {
    char *far_base;
    s32 *base;

    switch (*(u8 *)gOvl25SceneAssetLoadStep) {
    case 0:
        Ovl25LoadSharedSpriteArchive();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 1:
        Ovl25UploadCommonSpriteImages();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 2:
        Ovl25LoadObjectSpriteSet();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 3:
        Ovl25LoadSceneGeometry();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 4:
        Ovl25QueueSceneSoundLoads();
        (*(u8 *)gOvl25SceneAssetLoadStep)++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            *(u8 *)gOvl25SceneAssetLoadStep = 6;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)gOvl25SceneAssetLoadStep == 6) {
        Ovl25InitVramAnimationSlots();
        far_base = (char *)&D_8007AFF0;
        *(Ovl25SetupStatePair *)(far_base + 0x25474) =
            *(Ovl25SetupStatePair *)(far_base + 0x2479C);
        Ovl25RequestVramAnimations((s32)gLoadedAreaHeaderCopy);
        Ovl25ResetSceneState();
        Ovl25InitSceneCamera();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        *(u8 *)gOvl25SceneAssetLoadStep = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}

void Ovl25UpdateSceneTransition(void) {
    u8 var_s0;

    Ovl25UpdateSceneCameraOffset();
    Ovl25ApplySceneCamera();
    ApplyGteSceneState();
    Ovl25DrawSceneGeometry();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    Ovl25ResetSceneRecords();
    Ovl25DrawSceneSprites();
    if (var_s0 == 2) {
        s32 *base;

        base = (s32 *)&D_8006C838;
        base[0x448C >> 2]++;
    }
    Ovl25SetBackgroundColor(0, 0xA5, 0xD5);
}

void Ovl25UpdateSceneMode(void) {
    UpdateMessageWindow();
    Ovl25UpdateSceneCameraOffset();
    Ovl25ApplySceneCamera();
    ApplyGteSceneState();
    Ovl25DrawSceneGeometry();
    Ovl25UpdateVramAnimations();
    gOvl25SceneModeHandlers[D_800BFE44_view.selector]();
    Ovl25StepBackgroundColorCycle();
}

s32 Ovl25BeginSceneExit(void) {
    s32 *work;
    s32 nextPhase;

    Ovl25DrawSceneSprites();
    BeginScreenFadeFeedback(10);
    BeginMasterVolumeFadeOut(10);
    work = (s32 *)&D_8006C838;
    nextPhase = work[0x448C >> 2] + 1;
    work[0x448C >> 2] = nextPhase;
    return nextPhase;
}

void Ovl25TeardownScene(void) {
    s32 var_s0;
    u8 *base;

    Ovl25DrawSceneSprites();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0 = (var_s0 + 1) & 0xFF;
    }
    if (var_s0 == 2) {
        if (gOvl25SceneEffectVoice != -1) {
            StopSoundEffectVoice((s32) gOvl25SceneEffectVoice);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        base = (u8 *)D_8006C838;
        *(s32 *)(base + 0x448C) = 0xFF;
        RequestSceneChange(6);
        *(s32 *)(base + 0xC) |= 0x80000;
        Ovl25SetBackgroundColor(0, 0, 0);
    }
}

/* Scene selector/tuning defaults and the two initial-record banks. The
 * initializer uses the second table banks and sets record flag halfword 1;
 * the reset twin uses the first banks and clears it, preserving two fields
 * the initializer clears. These are intentionally distinct operations.
 * Public record-bank contract: ovl25_scene_state_machine.h.
 */
void Ovl25ResetSceneState(void) {
    char *work;

    work = (char *)&gOvl25SceneWorkWords;
    *(s16 *)(work + 0x0) = 0;
    *(s16 *)(work + 0x2) = 0;
    Ovl25InitSceneRecordState();
    *(s16 *)(work + 0x4D4) = 5;
    *(s16 *)(work + 0x4D6) = 2;
    *(s16 *)(work + 0x4D8) = 5;
    *(s16 *)(work + 0x4DA) = 1;
    *(s16 *)(work + 0x4DC) = 3;
    *(s16 *)(work + 0x4D2) = 0xFA0;
    *(s16 *)(work + 0x4D0) = 0x82;
    *(s16 *)(work + 0x4E4) = 0x46;
    *(s16 *)(work + 0x4E2) = 0x140;
    *(s16 *)(work + 0x4DE) = 0x2AD;
    *(s16 *)(work + 0x4E0) = 0xA;
    *(s16 *)(work + 0x4CE) = 0xC8;
}

void Ovl25SeedInitialSceneRecords(void);

void Ovl25InitSceneRecordState(void) {
    char *work;
    s16 *anchor;

    work = (char *)&gOvl25SceneWorkWords;
    anchor = (s16 *)(work + 0x3D0);
    *(s16 *)(work + 0x4C8) = -1;
    *(s16 *)(work + 0x3D0) = 0x1320;
    *(s32 *)(work + 4) = 0;
    *(s32 *)(work + 8) = 0;
    anchor[1] = -0xA;
    anchor[2] = 0xA4;
    *(s16 *)(work + 0xE) = 0;
    *(s16 *)(work + 0x4CA) = -1;
    *(s16 *)(work + 0x4CC) = 0;
    *(s16 *)(work + 0xC) = 0;
    Ovl25SeedInitialSceneRecords();
}

void Ovl25SeedInitialSceneRecords(void) {
    s32 recordIndex;
    s32 initialFixedValue;
    s32 enabled;
    s16 *record;
    Ovl25SeedRecord *sceneSource;
    Ovl25SeedRecord *auxSource;
    u8 *sceneTable;
    u8 *auxTable;

    recordIndex = 0;
    enabled = 1;
    initialFixedValue = 0x32000;
    sceneTable = (u8 *)gOvl25InitialSceneRecords;
    sceneSource = (Ovl25SeedRecord *)(sceneTable + 0x30);
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[8] = recordIndex;
        record[9] = enabled;
        record[10] = 0;
        record[11] = 0;
        record[12] = 0;
        *(s32 *)(record + 14) = initialFixedValue;
        *(Ovl25SeedRecord *)(record + 16) = *sceneSource;
        sceneSource++;
        recordIndex++;
    } while (recordIndex < 6);

    recordIndex = 0;
    auxTable = (u8 *)gOvl25InitialAuxRecords;
    auxSource = (Ovl25SeedRecord *)(auxTable + 0x10);
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[0x170] = recordIndex;
        *(Ovl25SeedRecord *)(record + 0x178) = *auxSource;
        auxSource++;
        recordIndex++;
    } while (recordIndex < 2);

    record = gOvl25SceneWorkWords;
    record[0x171] = 0xA;
    record[0x1AD] = 0xB;
}

void Ovl25ResetSceneRecords(void) {
    s32 recordIndex;
    s32 initialFixedValue;
    s16 *record;
    Ovl25SeedRecord *sceneSource;
    Ovl25SeedRecord *auxSource;

    recordIndex = 0;
    initialFixedValue = 0x32000;
    sceneSource = (Ovl25SeedRecord *)gOvl25InitialSceneRecords;
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[8] = recordIndex;
        record[9] = 0;
        record[10] = 0;
        *(s32 *)(record + 14) = initialFixedValue;
        *(Ovl25SeedRecord *)(record + 16) = *sceneSource;
        sceneSource++;
        recordIndex++;
    } while (recordIndex < 6);

    recordIndex = 0;
    auxSource = (Ovl25SeedRecord *)gOvl25InitialAuxRecords;
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[0x170] = recordIndex;
        *(Ovl25SeedRecord *)(record + 0x178) = *auxSource;
        auxSource++;
        recordIndex++;
    } while (recordIndex < 2);

    record = gOvl25SceneWorkWords;
    record[0x171] = 0xA;
    record[0x1AD] = 0xB;
}

/* The three-mode protocol: event phase 6 waits for the fade before entering
 * dialogue mode. Dialogue waits 60 calls (not message completion), then seeds
 * records and enters active mode. Active mode can pause its update chain;
 * rendering still runs, including the stateful sequence-sprite service.
 */

void Ovl25DispatchEventPhase(void) {
    s32 temp_v0;
    s32 q;
    s32 temp;

    Ovl25DrawSceneSprites();
    if (gOvl25SceneWorkWords[1] == 6) {
        temp_v0 = IsScreenFadeComplete();
        if (temp_v0 == 1) {
            gOvl25SceneWorkWords[1] = 0;
            gOvl25SceneWorkWords[0] = (s16) temp_v0;
        }
    } else {
        gOvl25EventPhaseHandlers[gOvl25SceneWorkWords[1]]();
        __asm__ volatile("" ::: "memory");
        q = (gOvl25SceneWorkWords[6] + 1) / 10000;
        temp = q * 0x2710 - 1;
        gOvl25SceneWorkWords[6] = (u16) gOvl25SceneWorkWords[6] - temp;
    }
}

void Ovl25RunDialogueMode(void) {
    char *ref;
    char *base;
    char *p;

    Ovl25DrawSceneSprites();
    ref = (char *)&gOvl25SceneWorkWords;
    if (*(s16 *)(ref + 0x4CC) == 0) {
        CloseMessageWindow();
        SetTextInputEnabled(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0x35);
        Ovl25SetRecordState((s16 *)(ref + 0x2E0), 9, 0x3B);
    }
    *(u16 *)(ref + 0x4CC) = *(u16 *)(ref + 0x4CC) + 1;
    if (*(s16 *)(ref + 0x4CC) >= 0x3C) {
        Ovl25InitSceneRecordState();
        *(s16 *)(ref + 0x4C8) = PlaySoundEffect(0x38);
        CloseMessageWindow();
        base = (char *)&D_8007AFF0;
        p = base + 0x20000;
        gOvl25SceneWorkWords[0] = 2;
        *(u16 *)(ref + 0x4CC) = 0;
        *(u16 *)(ref + 0xC) = 0;
        *(u16 *)(p + 0x53B6) = 0;
        *(u16 *)(p + 0x53B4) = *(u16 *)(ref + 0x20);
        *(u16 *)(p + 0x53B8) = *(u16 *)(ref + 0x24);
        Ovl25SetRecordState((s16 *)(ref + 0x2E0), 0xA, 0);
        gOvl25SequenceCyclePhase = 1;
        gOvl25SequenceAnchor4[0] = 0x85A;
        gOvl25SequenceAnchor4[1] = 0;
        gOvl25SequenceAnchor4[2] = -0x30A;
    }
}

void Ovl25DrawPausedScene (void);
void Ovl25UpdatePrimaryRecordFromInput (void);
void Ovl25AdvanceSceneTarget (void);
void Ovl25TickSceneRecords (void);
void Ovl25DrawSceneSprites (void);
void Ovl25ScanSceneCompletion (void);
void Ovl25UpdateOtherRecordChoices(void);

void Ovl25UpdateActiveSceneMode(void) {
    s32 temp_v0;
    s32 q;
    s32 decrement;

    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x800) {
        temp_v0 = D_800BFE44_view.paused != 1;
        D_800BFE44_view.paused = temp_v0;
        if (temp_v0 == 1) {
            Ovl25PauseSpriteSets();
            D_800BFE44_view.sceneEffectVoice = PlaySoundEffect(0x36);
        } else {
            Ovl25ResumeSpriteSets();
        }
    }
    if (D_800BFE44_view.paused == 1) {
        Ovl25DrawPausedScene();
        return;
    }
    Ovl25UpdatePrimaryRecordFromInput();
    Ovl25AdvanceSceneTarget();
    Ovl25UpdateOtherRecordChoices();
    Ovl25TickSceneRecords();
    Ovl25DrawSceneSprites();
    Ovl25ScanSceneCompletion();
    q = (D_800BFE44_view.unkC + 1) / 10000;
    decrement = q * 10000 - 1;
    D_800BFE44_view.unkC = (u16) D_800BFE44_view.unkC - decrement;
}

s32 Ovl25CanUpdateRecordChoice(Ovl25StateSelectorView *arg0);

void Ovl25UpdatePrimaryRecordFromInput(void) {
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_v0;
    u8 *temp_s1;
    u8 *temp_v0_2;
    Ovl25StateView *state;

    if (gOvl25SceneRecordBase.unk2 != 8) {
        temp_v0 = Ovl25CanUpdateRecordChoice((Ovl25StateSelectorView *) &gOvl25SceneRecordBase);
        if (temp_v0 == 1) {
            if ((*(s32 *) ((u8 *) gPad0Input + 4)) & 0x10) {
                (*(s16 *) ((u8 *) &gOvl25SceneRecordBase + -2)) = 0;
                Ovl25SetRecordState(&gOvl25SceneRecordBase.unk0, 5, 1);
            } else {
                if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x40) {
                    temp_s1 = (u8 *) &gOvl25SceneRecordBase - 0x10;
                    *(s16 *)(temp_s1 + 0xE) = 0;
                    if ((*(s32 *) ((u8 *) &gOvl25SceneRecordBase + 0xC)) <= 0x9FFF) {
                        Ovl25SetRecordState(&gOvl25SceneRecordBase.unk0, 7, 0x3C);
                        gOvl25SequenceCyclePhase = temp_v0;
                        gOvl25SequenceAnchor4[1] = 0;
                        gOvl25SequenceAnchor4[0] = gOvl25SceneRecordBase.source[0] - 0x320;
                        gOvl25SequenceAnchor4[2] = gOvl25SceneRecordBase.source[2];
                    } else {
                        Ovl25SetRecordState(&gOvl25SceneRecordBase.unk0, 4, *(s16 *)(temp_s1 + 0x4D4));
                        *(s16 *)(temp_s1 + 0x18) = Ovl25LookupTierValue(*(s32 *)(temp_s1 + 0x1C));
                    }
                } else {
                    temp_v0_2 = (u8 *) &gOvl25SceneRecordBase - 0x10;
                    (*(u16 *)(temp_v0_2 + 0xE))++;
                    if ((u32) ((u16) gOvl25SceneRecordBase.unk2 - 4) < 2U) {
                        if ((s16) gOvl25SceneRecordBase.unk4 == 0) {
                            Ovl25SetRecordState(&gOvl25SceneRecordBase.unk0, 2, 1);
                        }
                    }
                }
            }
        }
        state = &D_800BFE44_view;
        if (state->unk4CC != 0) {
            if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x50) {
                state->unk4CC = 0;
                return;
            }
            if (state->unk4CC == 1) {
                temp_a1 = gActiveRenderContext->field_D8;
                DrawMenuPanelWithText((u32 *)(temp_a1 + 0xC), temp_a1 + 8, (s32) D_800BCB34);
                return;
            }
            if (state->unk4CC == -1) {
                temp_a1_2 = gActiveRenderContext->field_D8;
                DrawMenuPanelWithText((u32 *)(temp_a1_2 + 0xC), temp_a1_2 + 8, (s32) D_800BCB8E);
            }
        }
    }
}

s32 Ovl25PassRecordChoiceThreshold(Ovl25RecordIdView *record, s16 choice);
void Ovl25ChooseWeightedState5Timer(s16 *arg0);

void Ovl25UpdateOtherRecordChoices(void) {
    s16 *state;
    s16 *rec;
    s16 random;
    s32 value;
    s32 i;
    s16 *ref;
    s16 *objects;

    state = (s16 *)&D_800BFE44_view;
    if (state[0xF9] != 8) {
        Ovl25SetRecordState(&state[0xF8], 4, 1);
    }
    if (state[0x135] != 8) {
        Ovl25SetRecordState(&state[0x134], 4, 1);
    }
    objects = state + 8;
    for (i = 1; i < 4; i++) {
        s16 *state;

        state = (s16 *)&D_800BFE44_view;
        rec = &objects[i * 0x3C];
        if (rec[1] == 8) {
            continue;
        }
        if (rec[2] != 0 && (rec[1] == 4 || rec[1] == 5 || rec[1] == 7 || rec[1] == 6)) {
            continue;
        }
        if (rec[1] == 3) {
            continue;
        }
        /* Retain the inherited scopes around these six calls for code layout. */
        if (*(s32 *)&rec[6] > 0x27FFF) {
            if (Ovl25PassRecordChoiceThreshold((Ovl25RecordIdView *)rec, 0) == 1) {
                do {
                    do {
                        do {
                            Ovl25SetRecordState(rec, 4, state[0x26A]);
                        } while (0);
                    } while (0);
                } while (0);
            } else {
                do {
                    do {
                        do {
                            Ovl25ChooseWeightedState5Timer(rec);
                        } while (0);
                    } while (0);
                } while (0);
            }
        } else if (*(s32 *)&rec[6] > 0x1DFFF) {
            if (Ovl25PassRecordChoiceThreshold((Ovl25RecordIdView *)rec, 1) == 1) {
                do {
                    do {
                        do {
                            Ovl25SetRecordState(rec, 4, state[0x26A]);
                        } while (0);
                    } while (0);
                } while (0);
            } else {
                do {
                    do {
                        do {
                            Ovl25ChooseWeightedState5Timer(rec);
                        } while (0);
                    } while (0);
                } while (0);
            }
        } else if (*(s32 *)&rec[6] > 0x13FFF) {
            if (Ovl25PassRecordChoiceThreshold((Ovl25RecordIdView *)rec, 2) == 1) {
                do {
                    do {
                        do {
                            Ovl25SetRecordState(rec, 4, state[0x26A]);
                        } while (0);
                    } while (0);
                } while (0);
            } else {
                do {
                    do {
                        do {
                            Ovl25ChooseWeightedState5Timer(rec);
                        } while (0);
                    } while (0);
                } while (0);
            }
        } else if (*(s32 *)&rec[6] > 0x9FFF) {
            if (Ovl25PassRecordChoiceThreshold((Ovl25RecordIdView *)rec, 3) == 1) {
                Ovl25SetRecordState(rec, 4, state[0x26A]);
            } else if (Ovl25PassRecordChoiceThreshold((Ovl25RecordIdView *)rec, 4) == 1) {
                Ovl25ChooseWeightedState5Timer(rec);
            } else {
                ref = objects - 8;
                random = Rand((u16)(120 / ref[0x26C])) + 1;
                value = ref[0x267] * 30;
                if (random >= value / (ref[0x26C] * 100)) {
                    Ovl25SetRecordState(rec, 3, 60);
                    value = random - 2;
                    rec[3] = value * 30 / ref[0x26A];
                } else {
                    Ovl25SetRecordState(rec, 2, random * (u16)ref[0x26A]);
                }
            }
        } else {
            Ovl25SetRecordState(rec, 7, 60);
        }
    }
}

void Ovl25AdvanceSceneTarget(void) {
    char *far_base = (char *)&D_8007AFF0;
    char *ref;
    s16 value;

    value = *(s16 *)(far_base + 0x253B4);
    ref = (char *)&gOvl25SceneWorkWords;
    if (value < *(s16 *)(ref + 0x20)) {
        *(u16 *)(far_base + 0x253B4) = *(u16 *)(far_base + 0x253B4) +
            ((*(s16 *)(ref + 0x20) - value) * 9) / 10;
    }
    if (*(s16 *)(far_base + 0x253B4) >= 0xCE5) {
        *(s16 *)(far_base + 0x253B4) = 0xCE4;
    }
    *(s16 *)(far_base + 0x253B6) = 0;
    *(s16 *)(far_base + 0x253B8) = *(u16 *)(ref + 0x24);
}

void Ovl25UpdateSceneRecord(s16 *arg0);

void Ovl25TickSceneRecords(void) {
    s16 *record;
    u8 *base;
    s32 i;
    s32 offset;

    offset = 0;
    record = (s16 *) &D_800BFE44_view;
    base = (u8 *) record + 0x10;
    i = 5;
    do {
        if (record[0xA] > 0) {
            record[0xA]--;
            if (record[0xA] < 0) {
                record[0xA] = 0;
            }
        }
        Ovl25UpdateSceneRecord((s16 *) (offset + (u32) base));
        offset += 0x78;
        i--;
        record += 0x3C;
    } while (i >= 0);
}

void Ovl25ChooseRecordStateAndTimer (s16 *arg0);

void Ovl25UpdateSceneRecord(s16 *record) {
    s16 voiceHandle;
    s16 stateCase;
    s16 primaryStateCase;

    stateCase = (*(u16 *) ((u8 *) record + 2)) - 2;
    switch (stateCase) {
    case 5:
        if ((record[0] != 0) && ((*(s16 *) ((u8 *) record + 4)) <= 0)) {
            Ovl25ChooseRecordStateAndTimer(record);
        }
        break;
    case 3:
        (*(s32 *) ((u8 *) record + 0xC)) = (s32) ((*(s32 *) ((u8 *) record + 0xC)) + ((s32) (D_800BFE44_view.unk4D6 << 0xC) / (s16) D_800BFE44_view.tierUpdateDivisor));
    default:
        break;
    case 0:
        (*(s32 *) ((u8 *) record + 0xC)) = (s32) ((*(s32 *) ((u8 *) record + 0xC)) - ((s32) (D_800BFE44_view.unk4DA << 0xC) / (s16) D_800BFE44_view.tierUpdateDivisor));
        break;
    case 4:
        (*(s32 *) ((u8 *) record + 0xC)) = (s32) ((*(s32 *) ((u8 *) record + 0xC)) + ((s32) (D_800BFE44_view.unk4D6 * (D_800BFE44_view.unk4D0 << 0xC)) / (s32) (D_800BFE44_view.tierUpdateDivisor * 0x64)));
        break;
    case 2:
        Ovl25AccumulateRecordTierValue(record);
        break;
    case 1:
        (*(s32 *) ((u8 *) record + 0xC)) = (s32) ((*(s32 *) ((u8 *) record + 0xC)) - ((s32) (D_800BFE44_view.unk4DA << 0xC) / (s16) D_800BFE44_view.tierUpdateDivisor));
        if ((*(s16 *) ((u8 *) record + 4)) <= 0) {
            Ovl25SetRecordState(record, 6, *(s16 *) ((u8 *) record + 6));
            (*(s16 *) ((u8 *) record + 6)) = 0;
        }
        break;
    }
    if ((*(s32 *) ((u8 *) record + 0xC)) > 0x31FFF) {
        (*(s32 *) ((u8 *) record + 0xC)) = 0x32000;
    }
    if ((*(s32 *) ((u8 *) record + 0xC)) < 0) {
        (*(s32 *) ((u8 *) record + 0xC)) = 0;
    }
    if (record[0] == 0) {
        primaryStateCase = (*(u16 *) ((u8 *) record + 2)) - 2;
        switch (primaryStateCase) {
        case 0:
            if (D_800BFE44_view.unkE >= ((D_800BFE44_view.unk4CE * 0x1E) / 100)) {
                Ovl25SetRecordState(record, 6, *(s16 *) ((u8 *) record + 4));
            } else {
                if ((*(s32 *) ((u8 *) record + 0xC)) <= 0x9FFF) {
                    Ovl25SetRecordState(record, 7, 0x3C);
                }
            }
            /* Fall through to ensure the state-2 effect voice is running. */
        case 2:
        case 3:
            if (D_800BFE44_view.recordEffectVoice == -1) {
                D_800BFE44_view.recordEffectVoice = PlaySoundEffect(0x37);
            }
            break;
        case 4:
            if (D_800BFE44_view.unkE >= (D_800BFE44_view.unk4DC * 0x1E)) {
                D_800BFE44_view.unk4CC = 1;
            }
            /* Fall through: both cases release the saved effect handle. */
        case 5:
            voiceHandle = D_800BFE44_view.recordEffectVoice;
            if (voiceHandle != -1) {
                StopSoundEffectVoice((s32) voiceHandle);
                D_800BFE44_view.recordEffectVoice = -1;
            }
            if ((*(s16 *) ((u8 *) record + 4)) == 0) {
                Ovl25SetRecordState(record, 6, 0);
            }
            break;
        case 6:
            if (D_800BFE44_view.recordEffectVoice != -1) {
                StopSoundEffectVoice((s32) D_800BFE44_view.recordEffectVoice);
                D_800BFE44_view.recordEffectVoice = -1;
            }
            break;
        }
        if ((s16) (*(u16 *) ((u8 *) record + 2)) == 5) {
            if (D_800BFE44_view.recordEffectVoice != -1) {
                SetSoundVoiceVolume(D_800BFE44_view.recordEffectVoice, 0x46, 0x46);
            }
        } else if (D_800BFE44_view.recordEffectVoice != -1) {
            SetSoundVoiceVolume(D_800BFE44_view.recordEffectVoice, 0x7F, 0x7F);
        }
        if ((D_800BFE44_view.unk8 == 1) && (D_800BFE44_view.unk20 == D_800BCC40)) {
            D_800BFE44_view.unk4CC = -1;
        }
    }
}

void Ovl25ScanSceneCompletion(void) {
    char *base;
    s16 *rec;
    s32 var_s2;
    s32 var_s6;

    var_s6 = 1;
    var_s2 = 0;
    base = (char *) D_80074838;
    rec = (s16 *) ((u8 *) &gOvl25SceneWorkWords);
    do {
        rec = (s16 *) ((u8 *) &gOvl25SceneWorkWords + var_s2 * 0x78);
        if (rec[9] != 8) {
            if (rec[0x10] >= (*(s16 *) ((u8 *) &gOvl25SceneWorkWords + 0x4D2) - 0x140)) {
                Ovl25SetRecordState((s16 *) ((u8 *) &gOvl25SceneWorkWords + 0x10 + var_s2 * 0x78), 8, 0x1E);
                if (*(s32 *) ((u8 *) &gOvl25SceneWorkWords + 8) == 0) {
                    *(s32 *) ((u8 *) &gOvl25SceneWorkWords + 8) = 1;
                    *(s16 *) ((u8 *) &gOvl25SceneWorkWords + 0x4C8) = PlaySoundEffect(0x39);
                    Ovl25SetRecordState((s16 *) ((u8 *) &gOvl25SceneWorkWords + 0x358), 0xC, 0x1E);
                    if (var_s2 == 0) {
                        *(s32 *) (base + 0x6638) = 1;
                        *(s16 *) (base + 0x663C) = 0;
                        Ovl25RaiseRecordByteRangeValue(1);
                        Ovl25RaiseRecordByteRangeValue(2);
                        Ovl25RaiseRecordByteRangeValue(4);
                        Ovl25RaiseRecordByteRangeValue(5);
                        Ovl25RaiseRecordByteRangeValue(8);
                        Ovl25RaiseRecordByteRangeValue(0xB);
                        Ovl25RaiseRecordByteRangeValue(0x10);
                        Ovl25RaiseRecordByteRangeValue(0x11);
                        Ovl25RaiseRecordByteRangeValue(0x13);
                        Ovl25RaiseRecordByteRangeValue(0x19);
                        Ovl25RaiseRecordByteRangeValue(0x1C);
                        Ovl25RaiseRecordByteRangeValue(0x22);
                        Ovl25RaiseRecordHalfwordRangeValue(3);
                        Ovl25RaiseRecordHalfwordRangeValue(7);
                        Ovl25RaiseRecordHalfwordRangeValue(9);
                        Ovl25RaiseRecordHalfwordRangeValue(0xE);
                        Ovl25RaiseRecordHalfwordRangeValue(0x14);
                    } else {
                        *(s32 *) (base + 0x6638) = 0;
                        *(s16 *) (base + 0x663C) = var_s2;
                        goto block_8;
                    }
                } else {
                    goto block_8;
                }
            } else {
                var_s6 = 0;
                goto block_8;
            }
        } else {
block_8:
            ;
        }
        var_s2 += 1;
    } while (var_s2 < 6);
    if (var_s6 == 1) {
        char *p;

        p = (char *) D_8006C838;
        *(s32 *) (p + 0x448C) = *(s32 *) (p + 0x448C) + 1;
    }
}

void Ovl25DrawPausedScene(void) {
    Ovl25DrawPrimarySprite();
    Ovl25DrawSceneSprites();
}

void Ovl25DrawSceneObjectRecords(void);

void Ovl25DrawSceneSprites(void) {
    Ovl25DrawSceneObjectRecords();
    Ovl25DrawThresholdIndicator();
    Ovl25DrawPairedSceneSprites();
    Ovl25UpdateSequenceSprites();
}

void Ovl25DrawSceneObjectRecords(void) {
    Ovl25ObjectRecord *sceneRecord;
    Ovl25ObjectRecord *auxRecord;
    s32 sceneRemaining;
    s32 auxRemaining;

    sceneRecord = &gOvl25SceneRecordBase;
    sceneRemaining = 5;
    do {
        Ovl25DrawObjectRecordPair(sceneRecord);
        sceneRemaining -= 1;
        sceneRecord += 1;
    } while (sceneRemaining >= 0);
    auxRecord = &gOvl25AuxiliaryRecordBase;
    auxRemaining = 1;
    do {
        Ovl25DrawAuxiliaryRecord(auxRecord);
        auxRemaining -= 1;
        auxRecord += 1;
    } while (auxRemaining >= 0);
}

/* Choose state 2/3 using the scene's rate parameters, or fall back to state
 * 5. State 3 also writes record +6. Divisors and the Rand bound must be valid. */
void Ovl25ChooseRecordStateAndTimer(s16 *record) {
    s16 random;
    s16 *state;
    s32 value;

    if (Ovl25PassRecordChoiceThreshold((Ovl25RecordIdView *)record, 4) == 1) {
        state = (s16 *)&D_800BFE44_view;
        random = Rand((u16)(120 / state[0x26C])) + 1;
        value = state[0x267] * 30;
        if (random >= value / (state[0x26C] * 100)) {
            Ovl25SetRecordState(record, 3, 60);
            value = random - 2;
            record[3] = value * 30 / state[0x26A];
            return;
        } else {
            Ovl25SetRecordState(record, 2, random * (u16)state[0x26A]);
        }
    } else {
        Ovl25ChooseWeightedState5Timer(record);
    }
}

/* Select a cumulative-weight bucket and convert its distance from record
 * +0C (Q12 units) into a state-5 timer. Signed category -1 intentionally reads
 * the original words preceding the weight-table origin; do not clamp it.
 * If no bucket wins, bucket remains 4. Accumulation wraps to s16 each step. */
void Ovl25ChooseWeightedState5Timer(s16 *record) {
    s16 random;
    s16 cumulativeWeight;
    s16 category;
    struct { signed int value : 16; } initialClass;
    s32 targetValue;
    s32 bucket;
    s32 valueQ12;
    s32 timer;
    s32 weight;
    s16 *state;

    cumulativeWeight = 0;
    /* Keep the signed-width initialization boundary before Rand. A plain
     * scalar zero lets CSE erase the separate category-0 arm and its branch. */
    initialClass.value = 0;
    category = initialClass.value;
    random = Rand(0x64);
    valueQ12 = *(s32 *)&record[6];
    if (valueQ12 > 0x27FFF) {
        category = -1;
    } else if (valueQ12 > 0x1DFFF) {
        category = 0;
    } else if (valueQ12 > 0x13FFF) {
        category = 1;
    } else if (valueQ12 > 0x9FFF) {
        category = 3;
    }
    bucket = 0;
    for (; bucket < 4; bucket++) {
        weight = gOvl25State5ChoiceWeights[category][record[0] - 1][bucket];
        cumulativeWeight = (s16)(cumulativeWeight + weight);
        if (random < cumulativeWeight) {
            break;
        }
    }
    if (category == -1) {
        targetValue = 0x32;
    } else {
        targetValue = (s16)(0x31 - bucket * 10);
        if (bucket == 0) {
            targetValue = targetValue + 1;
        }
        targetValue = (s16)targetValue;
    }
    valueQ12 = *(s32 *)&record[6];
    timer = targetValue - valueQ12 / 4096;
    state = (s16 *)&D_800BFE44_view;
    Ovl25SetRecordState((s16 *)record, 5, (s16)(timer * state[0x26C] / state[0x26B]));
}

/* States 7 and 4 suppress choice updates; every other signed state passes. */
s32 Ovl25CanUpdateRecordChoice(Ovl25StateSelectorView *record) {
    if (record->state != 7) {
        if (record->state != 4) {
            return 1;
        }
    }
    return 0;
}

/* The comparison is inverted: pass when Rand(128) >= the signed threshold.
 * It always consumes one random draw, even for an extreme threshold. */
s32 Ovl25PassRecordChoiceThreshold(Ovl25RecordIdView *record, s16 choice) {
    s32 random;
    s32 recordIndex;
    random = Rand(0x80);
    recordIndex = record->id - 1;
    return (random < gOvl25RecordChoiceThresholds[recordIndex * 5 + choice]) ^ 1;
}
