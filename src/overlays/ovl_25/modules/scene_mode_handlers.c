/*
 * scene_mode_handlers — reconstructed translation unit.
 *
 * Closed three-mode scene protocol and its input, record, target-motion,
 * timer, per-record update, completion-scan and draw helpers. The frame mode
 * owns the update chain; the shared renderer helper owns the final record
 * drawer.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl25_scene_mode_handlers.h"
#include "screen_fade.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl25_sprite_set_service.h"
#include "game_types.h"
#include "ovl25_fixed_point_tier_lookup.h"
#include "random.h"
#include "ovl25_record_value_increments.h"
#include "ovl25_scene_record_seeders.h"
#include "ovl25_record_state_timer.h"
#include "ovl25_record_tier_update.h"

void Ovl25DrawSceneSprites(void);

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

void SetVal8005E2BC(s32 arg0);

void Ovl25RunDialogueMode(void) {
    char *ref;
    char *base;
    char *p;

    Ovl25DrawSceneSprites();
    ref = (char *)&gOvl25SceneWorkWords;
    if (*(s16 *)(ref + 0x4CC) == 0) {
        CloseMessageWindow();
        SetVal8005E2BC(0);
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

s32 ovl_25_func_800B9758(Ovl25StateSelectorView *arg0);
/* The state update has a byte-exact void body as well as the existing
 * value-returning reconstruction. This caller requires the void contract;
 * none of its calls consumes a result. */
void func_8001AC10 (u32 *arg0, s32 arg1, s32 arg2);

void Ovl25UpdatePrimaryRecordFromInput(void) {
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_v0;
    u8 *temp_s1;
    u8 *temp_v0_2;
    Ovl25StateView *state;

    if (gOvl25SceneRecordBase.unk2 != 8) {
        temp_v0 = ovl_25_func_800B9758((Ovl25StateSelectorView *) &gOvl25SceneRecordBase);
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
                func_8001AC10((u32 *)(temp_a1 + 0xC), temp_a1 + 8, (s32) D_800BCB34);
                return;
            }
            if (state->unk4CC == -1) {
                temp_a1_2 = gActiveRenderContext->field_D8;
                func_8001AC10((u32 *)(temp_a1_2 + 0xC), temp_a1_2 + 8, (s32) D_800BCB8E);
            }
        }
    }
}

s32 ovl_25_func_800B977C(Ovl25RecordIdView *record, s16 choice);
void ovl_25_func_800B95B0(s16 *arg0);

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
            if (ovl_25_func_800B977C((Ovl25RecordIdView *)rec, 0) == 1) {
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
                            ovl_25_func_800B95B0(rec);
                        } while (0);
                    } while (0);
                } while (0);
            }
        } else if (*(s32 *)&rec[6] > 0x1DFFF) {
            if (ovl_25_func_800B977C((Ovl25RecordIdView *)rec, 1) == 1) {
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
                            ovl_25_func_800B95B0(rec);
                        } while (0);
                    } while (0);
                } while (0);
            }
        } else if (*(s32 *)&rec[6] > 0x13FFF) {
            if (ovl_25_func_800B977C((Ovl25RecordIdView *)rec, 2) == 1) {
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
                            ovl_25_func_800B95B0(rec);
                        } while (0);
                    } while (0);
                } while (0);
            }
        } else if (*(s32 *)&rec[6] > 0x9FFF) {
            if (ovl_25_func_800B977C((Ovl25RecordIdView *)rec, 3) == 1) {
                Ovl25SetRecordState(rec, 4, state[0x26A]);
            } else if (ovl_25_func_800B977C((Ovl25RecordIdView *)rec, 4) == 1) {
                ovl_25_func_800B95B0(rec);
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

void ovl_25_func_800B9480 (s16 *arg0);

void Ovl25UpdateSceneRecord(s16 *record) {
    s16 voiceHandle;
    s16 stateCase;
    s16 primaryStateCase;

    stateCase = (*(u16 *) ((u8 *) record + 2)) - 2;
    switch (stateCase) {
    case 5:
        if ((record[0] != 0) && ((*(s16 *) ((u8 *) record + 4)) <= 0)) {
            ovl_25_func_800B9480(record);
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
