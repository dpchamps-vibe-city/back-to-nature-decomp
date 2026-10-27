/* Scene phases and the closed nineteen-state interaction/message protocol.
 * The phase machine loads resources, fades in, runs one state per frame,
 * then fades out and releases sound before returning to resident scene 6.
 * Six records form two groups of three; each record has three 0x4C state
 * slices. Voice-key polling and message-window completion are independent
 * gates, with a 150-update timeout on voice playback.
 *
 * The simulation/draw dependencies beyond this module remain separate for
 * unresolved per-function flags, NOT an evidenced original-file boundary.
 * Public contract: ovl21_scene_state_machine.h. Whole-unit membership,
 * pooled table evidence and the mechanical limit: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl21_scene_state_machine.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "gte_scene_state.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "psyq/libspu.h"
#include "sound_commands.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl21_scene_camera_geometry.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "ovl21_scene_resources.h"
#include "ovl21_entity_record_views.h"
#include "random.h"

void func_80017A48(u32 arg0);

u32 func_80017A64(void);

void Ovl21StepScene(void) {
    s32 *phaseTable;
    u32 previousTextDrawMode;

    previousTextDrawMode = func_80017A64();
    func_80017A48(3U);
    phaseTable = gOvl21ScenePhaseTable;
    /* The original dispatch forwards its table base; all five handlers
     * ignore that argument. Keep the word view and call boundary intact. */
    ((void (*)(s32 *))phaseTable[gSceneSubphase])(phaseTable);
    func_80017A48(previousTextDrawMode);
}

void Ovl21InitSceneState(void);

void Ovl21LoadScenePhase(void) {
    s32 *base;

    switch (gOvl21SceneLoadStep) {
    case 0:
        Ovl21LoadSceneSpriteArchive();
        gOvl21SceneLoadStep++;
        break;
    case 1:
        Ovl21UploadSceneSpriteChunk();
        gOvl21SceneLoadStep++;
        break;
    case 2:
        Ovl21LoadUiAndObjectSprites();
        gOvl21SceneLoadStep++;
        break;
    case 3:
        Ovl21LoadSceneGeometry();
        gOvl21SceneLoadStep++;
        break;
    case 4:
        Ovl21QueueSceneSound();
        gOvl21SceneLoadStep++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            gOvl21SceneLoadStep = 6;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (gOvl21SceneLoadStep == 6) {
        Ovl21InitSceneState();
        Ovl21InitSceneCamera();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        gOvl21SceneLoadStep = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}

void Ovl21ResetSceneState(void);
void Ovl21SetRecordGroupState(s16 arg0, s16 arg1);
void ovl_21_func_800BA4C0(void);

void Ovl21WaitSceneFadeIn(void) {
    u8 var_s0;

    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
    Ovl21ResetSceneState();
    Ovl21SetRecordGroupState(0, 0);
    Ovl21SetRecordGroupState(1, 0);
    ovl_21_func_800BA4C0();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        ((Ovl21MainStateView *) D_8006C838)->state++;
    }
}

void Ovl21UpdateScenePhase(void) {
    UpdateMessageWindow();
    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
    gOvl21SceneStateHandlers[(s16) *gOvl21SceneControl]();
}

void Ovl21BeginSceneExit(void) {
    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
    ovl_21_func_800BA4C0();
    BeginScreenFadeFeedback(0xA);
    BeginMasterVolumeFadeOut(0xA);
    ((Ovl21MainStateView *) D_8006C838)->state++;
}

void Ovl21FinishSceneExit(void) {
    s32 var_s0;

    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
    ovl_21_func_800BA4C0();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0 = (var_s0 + 1) & 0xFF;
    }
    if (var_s0 == 2) {
        if (((Ovl21ExitStateView *) gOvl21SceneControl)->handles[0] != -1) {
            StopSoundEffectVoice(((Ovl21ExitStateView *) gOvl21SceneControl)->handles[0]);
        }
        if (((Ovl21ExitStateView *) gOvl21SceneControl)->handles[1] != -1) {
            StopSoundEffectVoice(((Ovl21ExitStateView *) gOvl21SceneControl)->handles[1]);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        ((Ovl21MainStateView *) D_8006C838)->state = 0xFF;
        RequestSceneChange(6);
        ((Ovl21MainStateView *) D_8006C838)->flags |= 0x80000;
    }
}

void Ovl21InitSceneState(void) {
    gOvl21SceneControl[0] = 0;
    gOvl21SceneControl[1] = 0;
    *(s32 *) ((u8 *) gOvl21SceneControl + 0xC) = 0;
    Ovl21ResetSceneState();
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x646) = 0;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x648) = 1;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x64A) = 2;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x64C) = 0x1E;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x64E) = 5;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x650) = 1;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x652) = 7;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x654) = 0x32;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x656) = 0xA;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x658) = 0x28;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x65A) = 0x12C;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x65C) = 0xAA;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x65E) = 0x10E;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x660) = 0xFF;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x662) = 5;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x664) = 0x80;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x666) = 0xF;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x668) = 5;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x66A) = 0x73;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x66C) = 0x1E;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x66E) = 5;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x670) = 0x6E;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x672) = 5;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x674) = 0x64;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x676) = 0xA;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x678) = 0x46;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x67A) = 0x28;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x67C) = 0xA;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x67E) = 0x3C;
    *(s16 *) ((u8 *) gOvl21SceneControl + 0x680) = 0x28;
}

void Ovl21InitSceneRecords(void);

void Ovl21ResetSceneState(void) {
    u8 *state;

    state = (u8 *) gOvl21SceneControl;
    *(s32 *) (state + 4) = 0;
    *(s16 *) (state + 0x984) = -1;
    *(s16 *) (state + 0x986) = -1;
    *(s16 *) (state + 0x988) = 0;
    *(s16 *) (state + 8) = *(s16 *) (state + 0x64C) * 30;
    Ovl21InitSceneRecords();
}

void Ovl21InitSceneRecords(void) {
    s32 index;
    s32 j;
    s32 nextIndex;
    s32 offset;
    UnkStruct800C0448 *origin;
    UnkStruct800C0448 *record;
    UnkStruct800C0448 *state;
    SVECTOR *position;
    Ovl21InitSelectorView *selector;

    selector = (Ovl21InitSelectorView *) D_8006C838;
    if (selector->selector == 7) {
        index = 0;
    } else if (selector->selector == 9) {
        index = 1;
    } else if (selector->selector == 0x14) {
        index = 2;
    } else {
        index = 3;
    }
    D_800C0448_init.tableIndex = index;
    D_800C0448_init.variantIndex = D_800BCC04[index][((s16 *) &D_800C0448_init)[1]];
    D_800C0448_init.records[0].unk10 = 0;
    D_800C0448_init.records[1].unk10 = D_800BCBBC[index][1];
    D_800C0448_init.records[2].unk10 = D_800BCBBC[index][2];
    D_800C0448_init.records[3].unk10 = D_800BCBD4[index][((s16 *) &D_800C0448_init)[1]][0];
    D_800C0448_init.records[4].unk10 = D_800BCBD4[index][((s16 *) &D_800C0448_init)[1]][1];
    D_800C0448_init.records[5].unk10 = D_800BCBD4[index][((s16 *) &D_800C0448_init)[1]][2];
    for (index = 0; index < 6; index = nextIndex) {
        offset = index * sizeof(UnkStruct800C0448);
        record = (UnkStruct800C0448 *) (offset + (s32) &D_800C0448_init);
        record->unk12 = index;
        D_800C0448_init.records[index].unk14 = 1;
        record->unk18 = 0;
        position = (SVECTOR *) &D_800C0448_init.records[index].unk1A;
        position->vy = 0;
        position->vx = gOvl21SceneOffsetPairs[index].unk0;
        position->vz = gOvl21SceneOffsetPairs[index].unk2;
        record->unk22 = Ovl21ReadEntityRangeValue(record->unk10);
        record->unk24 = Ovl21GetEntityRangeConstant(record->unk10);
        D_800C0448_init.records[index].unk28 = 0;
        record->unk2C = 4;
        record->unk2E = 0;
        D_800C0448_init.records[index].unk30 = Ovl21GetEntityObjectRecord(record->unk10);
        nextIndex = index + 1;
        /* Walk the three state slices while retaining the position source. */
        state = record;
        origin = state;
        for (j = 0; j < 3; j++) {
            state->unk34 = 0;
            state->unk38 = 0;
            state->unk3A = 0;
            state->unk3C = 0;
            state->unk48 = 0;
            state->unk4A = 0;
            state->unk4C = 0;
            state->unk40 = origin->unk1A;
            state->unk42 = origin->unk1C;
            state->unk44 = origin->unk1E;
            state = (UnkStruct800C0448 *) ((u8 *) state + sizeof(Ovl21InitSubstate));
        }
    }
}

void Ovl21PromptSceneConfirm(void) {
    s32 ready;
    s32 state;

    Ovl21ResetSceneState();
    Ovl21SetRecordGroupState(0, 0);
    Ovl21SetRecordGroupState(1, 0);
    ovl_21_func_800BA4C0();
    ready = IsScreenFadeComplete();
    if (ready == 1) {
        if ((s16) D_800C0448_init.records[0].unk2 != 0) {
            RequestMessageWindow(4, 0x29);
            state = GetMessageWindowStatus();
            if (state == 2) {
                if (GetMessageWindowChoice() == ready) {
                    gOvl21SceneControl[0] = state;
                } else {
                    gOvl21SceneControl[0] = ready;
                }
            }
        } else {
            gOvl21SceneControl[0] = ready;
        }
    }
}

void Ovl21PromptMessageChoice(void) {
    s32 temp_v0;

    ovl_21_func_800BA4C0();
    RequestMessageWindow(4, 0x18);
    temp_v0 = GetMessageWindowStatus();
    if (temp_v0 == 2) {
        if (GetMessageWindowChoice() == 1) {
            gOvl21SceneControl[0] = temp_v0;
        } else {
            gOvl21SceneControl[0] = 1;
        }
    }
}

void Ovl21ShowOpeningMessage(void) {
    u8 *base;

    Ovl21ResetSceneState();
    ovl_21_func_800BA4C0();
    RequestMessageWindow(4, 0x19);
    if (GetMessageWindowStatus() == 2) {
        CloseMessageWindow();
        base = (u8 *)gOvl21SceneControl;
        *(s16 *)(base + 0x988) = 0;
        *(s16 *)base = 3;
    }
}

void SetVal8005E2BC(s32 arg0);

void Ovl21RunFirstGroupVoiceMessage(void) {
    u8 *base;
    u8 *p;
    u8 *q;
    u8 *r;

    ovl_21_func_800BA4C0();
    base = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(base + 0x988) == -1) {
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            *(s16 *)(base + 0x988) = 0;
            gOvl21SceneControl[0] = 4;
            return;
        }
    }
    p = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(p + 0x988) >= 2) {
        s32 status;

        *(s16 *)(p + 0x988) = *(u16 *)(p + 0x988) + 1;
        status = SpuGetKeyStatus(gOvl21SceneVoiceMasks[*(s16 *)(p + 0x986)]);
        if (status == 0 || *(s16 *)(p + 0x988) >= 0x96) {
            Ovl21SetRecordGroupState(0, 0);
            SetVal8005E2BC(1);
            SetMessagePortraitEnabled(1);
            *(s16 *)(p + 0x988) = -1;
            return;
        }
    }
    q = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(q + 0x988) == 1) {
        if (GetMessageWindowStatus() == 5) {
            *(s16 *)(q + 0x986) = PlaySoundEffect(0x45);
            Ovl21SetRecordGroupState(0, 0xB);
            *(s16 *)(q + 0x988) = 2;
        }
    }
    r = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(r + 0x988) == 0) {
        CloseMessageWindow();
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, gOvl21FirstGroupMessages[*(s16 *)(r + 0x640)]);
        *(s16 *)(r + 0x988) = 1;
    }
}

void Ovl21RunSecondGroupVoiceMessage(void) {
    u8 *base;
    u8 *p;
    u8 *q;
    u8 *r;

    ovl_21_func_800BA4C0();
    base = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(base + 0x988) == -1) {
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            *(s16 *)(base + 0x988) = 0;
            gOvl21SceneControl[0] = 5;
            return;
        }
    }
    p = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(p + 0x988) >= 2) {
        s32 status;

        *(s16 *)(p + 0x988) = *(u16 *)(p + 0x988) + 1;
        status = SpuGetKeyStatus(gOvl21SceneVoiceMasks[*(s16 *)(p + 0x986)]);
        if (status == 0 || *(s16 *)(p + 0x988) >= 0x96) {
            Ovl21SetRecordGroupState(1, 0);
            SetVal8005E2BC(1);
            SetMessagePortraitEnabled(1);
            *(s16 *)(p + 0x988) = -1;
            return;
        }
    }
    q = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(q + 0x988) == 1) {
        if (GetMessageWindowStatus() == 5) {
            *(s16 *)(q + 0x986) = PlaySoundEffect(0x45);
            Ovl21SetRecordGroupState(1, 0xB);
            *(s16 *)(q + 0x988) = 2;
        }
    }
    r = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(r + 0x988) == 0) {
        CloseMessageWindow();
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, gOvl21SecondGroupMessages[*(s16 *)(r + 0x642)]);
        *(s16 *)(r + 0x988) = 1;
    }
}

/* Executable aliases of SetVal8005E2BC and SetMessagePortraitEnabled. */

void Ovl21ShowPromptMessage(void) {
    u8 *base;

    Ovl21ResetSceneState();
    ovl_21_func_800BA4C0();
    CloseMessageWindow();
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 0x1A);
    base = (u8 *)gOvl21SceneControl;
    *(s16 *)(base + 0x988) = 0;
    *(s16 *)base = 6;
}

void Ovl21DelayMessageThenPlaySound(void) {
    u8 *base;
    s32 fill;
    s32 i;
    s32 tmp;

    fill = 2;
    for (i = 5; i >= 0; i--) {
        ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 = fill;
    }
    ovl_21_func_800BA4C0();
    if (GetMessageWindowStatus() == 5) {
        base = (u8 *)gOvl21SceneControl;
        tmp = *(u16 *)(base + 0x988);
        tmp++;
        *(s16 *)(base + 0x988) = tmp;
        if ((s16)tmp >= 0x3D) {
            CloseMessageWindow();
            tmp = PlaySoundEffect(0x39);
            *(s16 *)(base + 0x984) = tmp;
            *(s16 *)(base + 0x988) = 0;
            gOvl21SceneControl[0] = 7;
            *(s16 *)(base + 8) = *(s16 *)(base + 0x64C) * 30;
        }
    }
}

void ovl_21_func_800BA9F8(void);
void ovl_21_func_800BAA18(void);
void ovl_21_func_800BA4F0(void);
s32 ovl_21_func_800B98CC(Ovl21Func800B98CCArg *arg0);
void ovl_21_func_800B9A20(void);
void ovl_21_func_800B9CA4(void);
void ovl_21_func_800B9F3C(void);
void ovl_21_func_800B990C(void);
void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_21_func_800BA7F0(s16 arg0);
void ovl_21_func_800BA300(void);

void Ovl21UpdateSceneInteraction(void) {
    s32 i;
    s32 temp;
    s32 text;

    if (*(u32 *)((u8 *)gPad0Input + 8) & 0x800) {
        temp = ((Ovl21C0448View *)gOvl21SceneControl)->unk4 != 1;
        ((Ovl21C0448View *)gOvl21SceneControl)->unk4 = temp;
        if (temp == 1) {
            ovl_21_func_800BA9F8();
            ((Ovl21C0448View *)gOvl21SceneControl)->unk984 = PlaySoundEffect(0x36);
        } else {
            ovl_21_func_800BAA18();
        }
    }
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk4 == 1) {
        ovl_21_func_800BA4C0();
        ovl_21_func_800BA4F0();
        return;
    }
    if (((UnkStruct800C0448 *)gOvl21SceneControl)->unk14 == 1) {
        switch (*(u32 *)((u8 *)gPad0Input + 8)) {
        case 0x8000:
            ((UnkStruct800C0448 *)gOvl21SceneControl)->unk2C--;
            break;
        case 0x2000:
            ((UnkStruct800C0448 *)gOvl21SceneControl)->unk2C++;
            break;
        case 0x10:
            if (((u16 *)((UnkStruct800C0448 *)gOvl21SceneControl)->unk30)[1] & 0x100) {
                ((UnkStruct800C0448 *)gOvl21SceneControl)->unk18 = 6;
            }
            break;
        case 0x40:
            if (((u16 *)((UnkStruct800C0448 *)gOvl21SceneControl)->unk30)[1] & 0x100) {
                if (((UnkStruct800C0448 *)gOvl21SceneControl)->unk18 != 2) {
                    ((UnkStruct800C0448 *)gOvl21SceneControl)->unk18 = 2;
                } else {
                    ovl_21_func_800B98CC((Ovl21Func800B98CCArg *)((u8 *)gOvl21SceneControl + 0x10));
                }
            }
            break;
        }
        if (((UnkStruct800C0448 *)gOvl21SceneControl)->unk2C < 3) {
            ((UnkStruct800C0448 *)gOvl21SceneControl)->unk2C = 5;
        }
        if (((UnkStruct800C0448 *)gOvl21SceneControl)->unk2C >= 6) {
            ((UnkStruct800C0448 *)gOvl21SceneControl)->unk2C = 3;
        }
    } else if (((Ovl21C0448View *)gOvl21SceneControl)->unk988 == 0 &&
               (*(u32 *)((u8 *)gPad0Input + 8) & 0xA050)) {
        ((Ovl21C0448View *)gOvl21SceneControl)->unk984 = PlaySoundEffect(2);
        *(void **)((u8 *)gOvl21SceneControl + 0xC) = D_800BCA78;
        ((Ovl21C0448View *)gOvl21SceneControl)->unk988 = 1;
    }
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk8 ==
        ((Ovl21C0448View *)gOvl21SceneControl)->unk8 / ((Ovl21C0448View *)gOvl21SceneControl)->unk64E * ((Ovl21C0448View *)gOvl21SceneControl)->unk64E) {
        ovl_21_func_800B9A20();
    }
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk8 ==
        ((Ovl21C0448View *)gOvl21SceneControl)->unk8 / ((Ovl21C0448View *)gOvl21SceneControl)->unk650 * ((Ovl21C0448View *)gOvl21SceneControl)->unk650) {
        ovl_21_func_800B9CA4();
    }
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk14 == 1 &&
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 == 4 &&
            (((u16 *)((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk30)[1] & 0x100)) {
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 = 2;
        }
    }
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk8 ==
        ((Ovl21C0448View *)gOvl21SceneControl)->unk8 / ((Ovl21C0448View *)gOvl21SceneControl)->unk648 * ((Ovl21C0448View *)gOvl21SceneControl)->unk648) {
        ovl_21_func_800B9F3C();
    }
    ovl_21_func_800BA300();
    ovl_21_func_800B990C();
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 == 7) {
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E++;
            if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E >= 30) {
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E = 0;
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 = 9;
            }
        }
    }
    ovl_21_func_800BA4C0();
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk988 != 0) {
        if (*(void **)((u8 *)gOvl21SceneControl + 0xC) != NULL) {
            text = gActiveRenderContext->field_D8;
            func_8001AC10((u32 *)(text + 0xC), text + 8, (s32)((Ovl21C0448View *)gOvl21SceneControl)->unkC);
        }
        (((Ovl21C0448View *)gOvl21SceneControl)->unk988)++;
        if (((Ovl21C0448View *)gOvl21SceneControl)->unk988 >= 90) {
            ((Ovl21C0448View *)gOvl21SceneControl)->unk988 = 0;
        }
    }
    (((Ovl21C0448View *)gOvl21SceneControl)->unk8)--;
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk8 <= 0 ||
        ovl_21_func_800BA7F0(0) == 0 || ovl_21_func_800BA7F0(1) == 0) {
        ((Ovl21C0448View *)gOvl21SceneControl)->unk986 = PlaySoundEffect(0x39);
        CloseMessageWindow();
        gOvl21SceneControl[0] = 8;
        ((Ovl21C0448View *)gOvl21SceneControl)->unk988 = 0;
    }
}

s32 Ovl21CountActiveRecordSlices(void);

void Ovl21WaitVoiceCooldown(void) {
    s32 i;
    u8 *base;
    u8 *p;

    ovl_21_func_800BA300();
    ovl_21_func_800B990C();
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 == 7) {
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E++;
            if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E >= 30) {
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E = 0;
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 = 9;
            }
        }
    }
    ovl_21_func_800BA4C0();
    base = (u8 *)gOvl21SceneControl;
    (*(s16 *)(base + 0x988))++;
    if (SpuGetKeyStatus(gOvl21SceneVoiceMasks[*(s16 *)(base + 0x986)]) == 0 ||
        *(s16 *)(base + 0x988) >= 150) {
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0x1D);
    }
    if (Ovl21CountActiveRecordSlices() == 0 && GetMessageWindowStatus() == 5) {
        CloseMessageWindow();
        p = (u8 *)gOvl21SceneControl;
        *(s16 *)(p + 0x988) = 0;
        gOvl21SceneControl[0] = 9;
    }
}

s32 Ovl21SelectLargerRecordGroup(void);
/* Original overlay imports for SetVal8005E2BC and SetMessagePortraitEnabled. */

void Ovl21SelectTargetGroup(void) {
    s32 slot;
    s32 next;

    ovl_21_func_800BA4C0();
    slot = Ovl21SelectLargerRecordGroup();
    if (slot == -1) {
        RequestMessageWindow(4, 0x26);
        next = 0xC;
    } else {
        next = 0xB;
        if (slot == 0) {
            next = 0xA;
        }
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, gOvl21ResultMessages[((Ovl21SelectorView *)gOvl21SceneControl)->selectors[slot]]);
    }
    ((Ovl21SelectorView *)gOvl21SceneControl)->field_988 = 0;
    gOvl21SceneControl[0] = next;
}

void Ovl21WaitFirstGroupVoice(void) {
    u8 *base;
    u8 *p;

    ovl_21_func_800BA4C0();
    base = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(base + 0x988) == -1) {
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            *(s16 *)(base + 0x988) = 0;
            if (*(s16 *)(base + 2) == 0) {
                BeginScreenFadeIncrease(0xA, 0, 2);
                gOvl21SceneControl[0] = 0x10;
            } else {
                gOvl21SceneControl[0] = 0x11;
            }
            return;
        }
    }
    p = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(p + 0x988) > 0) {
        s32 status;

        *(s16 *)(p + 0x988) = *(u16 *)(p + 0x988) + 1;
        status = SpuGetKeyStatus(gOvl21SceneVoiceMasks[*(s16 *)(p + 0x986)]);
        if (status == 0 || *(s16 *)(p + 0x988) >= 0x96) {
            Ovl21SetRecordGroupState(0, 0);
            SetVal8005E2BC(1);
            SetMessagePortraitEnabled(1);
            *(s16 *)(p + 0x988) = -1;
        }
    }
    base = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(base + 0x988) == 0) {
        if (GetMessageWindowStatus() == 5) {
            *(s16 *)(base + 0x986) = PlaySoundEffect(0x45);
            Ovl21SetRecordGroupState(0, 0xB);
            *(s16 *)(base + 0x988) = 1;
        }
    }
}

void Ovl21WaitSecondGroupVoice(void) {
    u8 *base;
    u8 *p;
    u8 *q;

    ovl_21_func_800BA4C0();
    base = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(base + 0x988) == -1) {
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            *(s16 *)(base + 0x988) = 0;
            gOvl21SceneControl[0] = 0x12;
            return;
        }
    }
    p = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(p + 0x988) > 0) {
        s32 status;

        *(s16 *)(p + 0x988) = *(u16 *)(p + 0x988) + 1;
        status = SpuGetKeyStatus(gOvl21SceneVoiceMasks[*(s16 *)(p + 0x986)]);
        if (status == 0 || *(s16 *)(p + 0x988) >= 0x96) {
            Ovl21SetRecordGroupState(1, 0);
            SetVal8005E2BC(1);
            SetMessagePortraitEnabled(1);
            *(s16 *)(p + 0x988) = -1;
        }
    }
    q = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(q + 0x988) == 0) {
        if (GetMessageWindowStatus() == 5) {
            *(s16 *)(q + 0x986) = PlaySoundEffect(0x45);
            Ovl21SetRecordGroupState(1, 0xB);
            *(s16 *)(q + 0x988) = 1;
        }
    }
}

void Ovl21WaitFinalVoice(void) {
    u8 *base;
    u8 *p;

    ovl_21_func_800BA4C0();
    base = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(base + 0x988) > 0) {
        s32 status;

        *(s16 *)(base + 0x988) = *(u16 *)(base + 0x988) + 1;
        status = SpuGetKeyStatus(gOvl21SceneVoiceMasks[*(s16 *)(base + 0x986)]);
        if (status == 0 || *(s16 *)(base + 0x988) >= 0x96) {
            CloseMessageWindow();
            *(s16 *)(base + 0x988) = 0;
            BeginScreenFadeIncrease(0xA, 0, 2);
            gOvl21SceneControl[0] = 0xD;
            return;
        }
    }
    p = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(p + 0x988) == 0) {
        if (GetMessageWindowStatus() == 5) {
            *(s16 *)(p + 0x986) = PlaySoundEffect(0x44);
            *(s16 *)(p + 0x988) = 1;
        }
    }
}

void ovl_21_func_800BA4C0 (void);
void Ovl21ResetSceneState (void);
void Ovl21InitSceneRecords (void);
void ovl_21_func_800BA510 (void);
void ovl_21_func_800BA5D0 (void);
void ovl_21_func_800BA670 (void);

void Ovl21PromptCloseMessage(void) {
    ovl_21_func_800BA4C0();
    if (IsScreenFadeComplete() == 1) {
        Ovl21ResetSceneState();
        BeginScreenFadeDecrease(0xA, 0, 2);
        RequestMessageWindow(4, 0x28);
        *gOvl21SceneControl = 0xE;
    }
}

void Ovl21ResolveChoice(void) {
    if (GetMessageWindowStatus() == 2) {
        if (GetMessageWindowChoice() == 1) {
            gOvl21SceneControl[0] = 5;
        } else {
            gOvl21SceneControl[0] = 0xF;
        }
    }
    ovl_21_func_800BA4C0();
}

void Ovl21PromptRetryChoice(void) {
    RequestMessageWindow(4, 0x18);
    if (GetMessageWindowStatus() == 2) {
        if (GetMessageWindowChoice() == 1) {
            gOvl21SceneControl[0] = 5;
        }
    }
    ovl_21_func_800BA4C0();
}

void Ovl21FadeInAndAdvanceRound(void) {
    ovl_21_func_800BA4C0();
    if (IsScreenFadeComplete() == 1) {
        BeginScreenFadeDecrease(10, 0, 2);
        gOvl21SceneControl[0] = 0;
        gOvl21SceneControl[1] += 1;
    }
}

void Ovl21ApplyRecordResult(void) {
    u8 *base;
    u8 *p;

    ovl_21_func_800BA4C0();
    Ovl21RaiseEntityRangeValue(((Ovl21ResultEntityView *) gOvl21SceneControl)->firstEntityId);
    Ovl21RaiseEntityRangeValue(((Ovl21ResultEntityView *) gOvl21SceneControl)->secondEntityId);
    base = (u8 *) D_8006C838;
    p = base + 0x8000;
    *(s32 *) (p + 0x662C) = 1;
    *(u16 *) (p + 0x662A) = *(u16 *) (p + 0x6628);
    *(s32 *) (base + 0x448C) = *(s32 *) (base + 0x448C) + 1;
}

void Ovl21PickRandomResult(void) {
    u8 *state;
    s32 *words;
    s16 selector;

    ovl_21_func_800BA4C0();
    state = (u8 *)gOvl21SceneControl;
    if (*(s16 *)(state + 2) == 0) {
        selector = Rand(4);
        if (*(s16 *)(state + 0x640) == selector) {
            selector++;
            selector %= 4;
        }
        ((Ovl21ResultView *)D_8006C838)->selection = D_800BCC14[selector];
    } else {
        ((Ovl21ResultView *)D_8006C838)->selection = D_800BCC14[*(s16 *)(state + 0x642)];
    }
    words = (s32 *)D_8006C838;
    words[0xE62C / 4] = 0;
    words[0x448C / 4]++;
}

void Ovl21SetRecordGroupState(s16 group, s16 recordState) {
    s32 index;

    for (index = group * 3; index < group * 3 + 3; index++) {
        ((UnkStruct800C0448 *)gOvl21SceneControl)[index].unk18 = recordState;
    }
}

s32 Ovl21SelectLargerRecordGroup(void) {
    s32 secondGroupCount;
    s32 firstGroupCount;

    firstGroupCount = ovl_21_func_800BA7F0(0);
    secondGroupCount = ovl_21_func_800BA7F0(1);
    if (secondGroupCount < firstGroupCount) {
        return 0;
    } else {
        if (firstGroupCount < secondGroupCount) {
            return 1;
        } else {
            return -1;
        }
    }
}

s32 Ovl21CountActiveRecordSlices(void) {
    s16 count;
    s32 sel;
    s32 tmp;
    s32 i;
    s32 j;
    s32 one;
    s32 pos;
    s32 step;
    u8 *raw;
    UnkStruct800C0448 *base;
    s32 *tbl;
    s32 *p;

    sel = 0;
    i = 0;
    raw = (u8 *)gOvl21SceneControl;
    count = *(s16 *)(raw + 0x64A);
    for (; i < 6; i++) {
        if (count > 0) {
            one = 1;
            base = (UnkStruct800C0448 *)gOvl21SceneControl;
            tbl = (s32 *)((u8 *)base + 0x34);
            p = (s32 *)((u8 *)tbl + (((i << 5) + i) << 3));
            pos = (sel << 16) + 0x10000;
            step = 0x10000;
            j = *(s16 *)((u8 *)base + 0x64A);
            do {
                if (*p == one) {
                    tmp = pos;
                    pos += step;
                    sel = tmp >> 16;
                }
                p = (s32 *)((u8 *)p + 0x4C);
            } while (--j);
        }
    }
    return sel;
}
