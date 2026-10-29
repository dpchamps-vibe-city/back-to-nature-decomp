/*
 * Overlay-21 scene runtime, 800B7E3C-800BBA84: phases, six-record interaction,
 * projectiles, sprite pause/drawing, entity views and resource/camera lifetime.
 * The phase machine loads assets, fades in, runs one of nineteen states per
 * frame, then releases sound and returns to resident scene 6. Voice polling
 * and message completion are independent gates; voice waits cap at 150 updates.
 *
 * gOvl21SceneControl (800C0448) contains two groups of three 0x108-byte
 * records, each with three 0x4C slices. gOvl21RecordObjectBase is its +0x10
 * anchor, not separate storage. Initialization and primary-sprite projection
 * consume the same six X/Z offset pairs. Resource loading binds the sprite
 * sources subsequently walked by pause and drawing; headers must stay alive.
 * Proximity search returns a writable active word without consuming it.
 *
 * All 67 entries are kept in original order. Only Ovl21StepScene is public
 * (ovl21_scene_state_machine.h); in-unit helpers retain external linkage.
 * Membership, whole-container edges and pooled tables: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl21_scene_state_machine.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "gte_scene_state.h"
#include "master_volume.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "psyq/libspu.h"
#include "pad_actuator_sequences.h"
#include "sound_commands.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "random.h"
#include "projected_primitives.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "point_projection.h"
#include "ui_sprites.h"
#include "surface_collision.h"
#include "psyq/libapi.h"
#include "text_engine.h"

/* Scene-local interfaces; co-location does not establish static linkage. */
void Ovl21InitSceneCamera(void);
void Ovl21UpdateSceneCameraOffset(void);
void Ovl21ApplySceneCamera(void);
void Ovl21DrawSceneGeometry(void);
void Ovl21LoadSceneSpriteArchive(void);
void Ovl21UploadSceneSpriteChunk(void);
void Ovl21LoadUiAndObjectSprites(void);
void Ovl21LoadSceneGeometry(void);
void Ovl21QueueSceneSound(void);
void Ovl21RaiseEntityRangeValue(s16 entityId);
s16 Ovl21ReadEntityRangeValue(s16 entityId);
s32 Ovl21GetEntityRangeConstant(s16 entityId);
u8 *Ovl21GetEntityObjectRecord(s16 entityId);
void Ovl21DrawSceneSprites(void);
void Ovl21DrawSceneFrame(void);
void Ovl21PauseSceneSprites(void);
void Ovl21ResumeSceneSprites(void);
void Ovl21DrawRecordSprite(s32 *record);
void Ovl21DrawSlotSprite(s32 *slot);
void Ovl21DrawPrimarySceneSprite(s16 offsetIndex);
void Ovl21DrawCenteredFrameRects(void);
void Ovl21SubmitProjectedSprite(SpriteSourceData *source, u16 *position);

s32 Ovl21CountActiveRecordsInGroup(s16 group);
s16 Ovl21PickRandomActiveRecord(s16 mode);
s16 Ovl21PickRandomInactiveRecord(s16 group);

void Ovl21StepScene(void) {
    s32 *phaseTable;
    u32 previousTextDrawMode;

    previousTextDrawMode = GetTextPalette();
    SetTextPalette(3U);
    phaseTable = gOvl21ScenePhaseTable;
    /* The original dispatch forwards its table base; all five handlers
     * ignore that argument. Keep the word view and call boundary intact. */
    ((void (*)(s32 *))phaseTable[gSceneSubphase])(phaseTable);
    SetTextPalette(previousTextDrawMode);
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

void Ovl21WaitSceneFadeIn(void) {
    u8 var_s0;

    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
    Ovl21ResetSceneState();
    Ovl21SetRecordGroupState(0, 0);
    Ovl21SetRecordGroupState(1, 0);
    Ovl21DrawSceneSprites();
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
    Ovl21DrawSceneSprites();
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
    Ovl21DrawSceneSprites();
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
        position->vx = gOvl21SceneOffsetPairs[index].x;
        position->vz = gOvl21SceneOffsetPairs[index].z;
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
    Ovl21DrawSceneSprites();
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

    Ovl21DrawSceneSprites();
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
    Ovl21DrawSceneSprites();
    RequestMessageWindow(4, 0x19);
    if (GetMessageWindowStatus() == 2) {
        CloseMessageWindow();
        base = (u8 *)gOvl21SceneControl;
        *(s16 *)(base + 0x988) = 0;
        *(s16 *)base = 3;
    }
}

void Ovl21RunFirstGroupVoiceMessage(void) {
    u8 *base;
    u8 *p;
    u8 *q;
    u8 *r;

    Ovl21DrawSceneSprites();
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
            SetTextInputEnabled(1);
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
        SetTextInputEnabled(0);
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

    Ovl21DrawSceneSprites();
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
            SetTextInputEnabled(1);
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
        SetTextInputEnabled(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, gOvl21SecondGroupMessages[*(s16 *)(r + 0x642)]);
        *(s16 *)(r + 0x988) = 1;
    }
}

/* Executable aliases of SetVal8005E2BC and SetMessagePortraitEnabled. */

void Ovl21ShowPromptMessage(void) {
    u8 *base;

    Ovl21ResetSceneState();
    Ovl21DrawSceneSprites();
    CloseMessageWindow();
    SetTextInputEnabled(0);
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
    Ovl21DrawSceneSprites();
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

s32 Ovl21MarkOppositeGroupTarget(Ovl21Func800B98CCArg *arg0);
void Ovl21ChooseRecordTargets(void);
void Ovl21UpdateRecordActions(void);
void Ovl21SpawnRecordProjectiles(void);
void Ovl21ConsumeNearbyRecordSlots(void);
void Ovl21StepRecordProjectiles(void);

void Ovl21UpdateSceneInteraction(void) {
    s32 i;
    s32 temp;
    s32 text;

    if (*(u32 *)((u8 *)gPad0Input + 8) & 0x800) {
        temp = ((Ovl21C0448View *)gOvl21SceneControl)->unk4 != 1;
        ((Ovl21C0448View *)gOvl21SceneControl)->unk4 = temp;
        if (temp == 1) {
            Ovl21PauseSceneSprites();
            ((Ovl21C0448View *)gOvl21SceneControl)->unk984 = PlaySoundEffect(0x36);
        } else {
            Ovl21ResumeSceneSprites();
        }
    }
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk4 == 1) {
        Ovl21DrawSceneSprites();
        Ovl21DrawSceneFrame();
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
                    Ovl21MarkOppositeGroupTarget((Ovl21Func800B98CCArg *)((u8 *)gOvl21SceneControl + 0x10));
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
        Ovl21ChooseRecordTargets();
    }
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk8 ==
        ((Ovl21C0448View *)gOvl21SceneControl)->unk8 / ((Ovl21C0448View *)gOvl21SceneControl)->unk650 * ((Ovl21C0448View *)gOvl21SceneControl)->unk650) {
        Ovl21UpdateRecordActions();
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
        Ovl21SpawnRecordProjectiles();
    }
    Ovl21StepRecordProjectiles();
    Ovl21ConsumeNearbyRecordSlots();
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 == 7) {
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E++;
            if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E >= 30) {
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E = 0;
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 = 9;
            }
        }
    }
    Ovl21DrawSceneSprites();
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk988 != 0) {
        if (*(void **)((u8 *)gOvl21SceneControl + 0xC) != NULL) {
            text = gActiveRenderContext->field_D8;
            DrawMenuPanelWithText((u32 *)(text + 0xC), text + 8, (s32)((Ovl21C0448View *)gOvl21SceneControl)->unkC);
        }
        (((Ovl21C0448View *)gOvl21SceneControl)->unk988)++;
        if (((Ovl21C0448View *)gOvl21SceneControl)->unk988 >= 90) {
            ((Ovl21C0448View *)gOvl21SceneControl)->unk988 = 0;
        }
    }
    (((Ovl21C0448View *)gOvl21SceneControl)->unk8)--;
    if (((Ovl21C0448View *)gOvl21SceneControl)->unk8 <= 0 ||
        Ovl21CountActiveRecordsInGroup(0) == 0 || Ovl21CountActiveRecordsInGroup(1) == 0) {
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

    Ovl21StepRecordProjectiles();
    Ovl21ConsumeNearbyRecordSlots();
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 == 7) {
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E++;
            if (((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E >= 30) {
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2E = 0;
                ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk18 = 9;
            }
        }
    }
    Ovl21DrawSceneSprites();
    base = (u8 *)gOvl21SceneControl;
    (*(s16 *)(base + 0x988))++;
    if (SpuGetKeyStatus(gOvl21SceneVoiceMasks[*(s16 *)(base + 0x986)]) == 0 ||
        *(s16 *)(base + 0x988) >= 150) {
        SetTextInputEnabled(0);
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

    Ovl21DrawSceneSprites();
    slot = Ovl21SelectLargerRecordGroup();
    if (slot == -1) {
        RequestMessageWindow(4, 0x26);
        next = 0xC;
    } else {
        next = 0xB;
        if (slot == 0) {
            next = 0xA;
        }
        SetTextInputEnabled(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, gOvl21ResultMessages[((Ovl21SelectorView *)gOvl21SceneControl)->selectors[slot]]);
    }
    ((Ovl21SelectorView *)gOvl21SceneControl)->field_988 = 0;
    gOvl21SceneControl[0] = next;
}

void Ovl21WaitFirstGroupVoice(void) {
    u8 *base;
    u8 *p;

    Ovl21DrawSceneSprites();
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
            SetTextInputEnabled(1);
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

    Ovl21DrawSceneSprites();
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
            SetTextInputEnabled(1);
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

    Ovl21DrawSceneSprites();
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

void Ovl21ResetSceneState (void);
void Ovl21InitSceneRecords (void);

void Ovl21PromptCloseMessage(void) {
    Ovl21DrawSceneSprites();
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
    Ovl21DrawSceneSprites();
}

void Ovl21PromptRetryChoice(void) {
    RequestMessageWindow(4, 0x18);
    if (GetMessageWindowStatus() == 2) {
        if (GetMessageWindowChoice() == 1) {
            gOvl21SceneControl[0] = 5;
        }
    }
    Ovl21DrawSceneSprites();
}

void Ovl21FadeInAndAdvanceRound(void) {
    Ovl21DrawSceneSprites();
    if (IsScreenFadeComplete() == 1) {
        BeginScreenFadeDecrease(10, 0, 2);
        gOvl21SceneControl[0] = 0;
        gOvl21SceneControl[1] += 1;
    }
}

void Ovl21ApplyRecordResult(void) {
    u8 *base;
    u8 *p;

    Ovl21DrawSceneSprites();
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

    Ovl21DrawSceneSprites();
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

    firstGroupCount = Ovl21CountActiveRecordsInGroup(0);
    secondGroupCount = Ovl21CountActiveRecordsInGroup(1);
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

/* Preserve a shared result return after the two stores: separate per-arm
 * returns give the scheduler different value/return lifetimes. */
s32 Ovl21MarkOppositeGroupTarget(Ovl21Func800B98CCArg *record) {
    s32 oppositeGroup;

    if (record->unk2 < 3) {
        oppositeGroup = (u32)(record->unk1C - 3) < 3U;
        record->unk18 = oppositeGroup;
    } else {
        oppositeGroup = (u32)record->unk1C < 3U;
        record->unk18 = oppositeGroup;
    }
    return oppositeGroup;
}

s32 *Ovl21FindFirstProjectileInRange(Ovl21DistanceOrigin *origin, s16 radius, s16 group);

/* Walks the six D_800C0448 records, skipping record 0 when the flag at +0x646
 * is set. For each record whose word is 1, asks Ovl21FindFirstProjectileInRange for a
 * sub-entry within the radius at +0x65C of the record's position, searching
 * the first or second half of the table (i < 3). A hit whose state is not 6
 * is cleared and the record moved to state 7, a sound started and its handle
 * kept at +0x984; record 0 also calls func_8001B2CC(0, 1), installs
 * D_800BCAD8 at +0xC and sets +0x988 to 1.
 *
 * The flag test and the record work read the block through separate
 * pointers, and the position pointer is taken from record 0 just before the
 * call: the target forms block + 0x1A ahead of the scaled index, which no
 * field access through the s32-aligned record type produces. */
void Ovl21ConsumeNearbyRecordSlots(void) {
    s32 i;
    s32 val;
    s32 *ret;
    u8 *pos;
    Ovl21C0448View *chk;
    Ovl21C0448View *blk;

    for (i = 0; i < 6; i++) {
        chk = (Ovl21C0448View *)gOvl21SceneControl;
        if (chk->unk646 != 0 && i == 0) {
            i = 1;
        }
        blk = (Ovl21C0448View *)gOvl21SceneControl;
        val = blk->recs[i].unk0;
        if (val == 1) {
            pos = (u8 *)blk->recs[0].unk6;
            ret = Ovl21FindFirstProjectileInRange((Ovl21DistanceOrigin *)(pos + i * sizeof(Ovl21C0448Record)), blk->unk65C, i < 3);
            if (ret != NULL && blk->recs[i].unk4 != 6) {
                *ret = 0;
                blk->recs[i].unk0 = 0;
                blk->recs[i].unk4 = 7;
                blk->recs[i].unk1A = 0;
                blk->unk984 = PlaySoundEffect(0x3F);
                if (i == 0) {
                    RequestPadActuatorSequence(0, 1);
                    blk->unkC = D_800BCAD8;
                    blk->unk988 = 1;
                }
            }
        }
    }
}

void Ovl21ChooseRecordTargets(void) {
    s32 i;
    u8 *blk;
    u8 *first;
    u8 *base;

    for (i = 1; i < 3; i++) {
        switch (Ovl21CountActiveRecordsInGroup(1)) {
        case 3:
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2C = Rand(3) + 3;
            break;
        case 2:
            first = (u8 *)gOvl21SceneControl;
            if ((s32)Rand(100) < *(s16 *)(first + 0x676)) {
                ((UnkStruct800C0448 *)first)[i].unk2C = Ovl21PickRandomInactiveRecord(1);
            } else {
                ((UnkStruct800C0448 *)first)[i].unk2C = Ovl21PickRandomActiveRecord(1);
            }
            break;
        default:
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2C = Ovl21PickRandomActiveRecord(1);
            break;
        }
    }
    for (i = 3; i < 6; i++) {
        switch (Ovl21CountActiveRecordsInGroup(0)) {
        case 3:
            base = (u8 *)gOvl21SceneControl;
            if ((s32)Rand(100) < *(s16 *)(base + 0x67E) -
                (*(s16 *)(base + 0x680) * ((UnkStruct800C0448 *)base)[i].unk22) / 255) {
                ((UnkStruct800C0448 *)base)[i].unk2C = 0;
            } else {
                ((UnkStruct800C0448 *)base)[i].unk2C = Ovl21PickRandomActiveRecord(2);
            }
            break;
        case 2:
            blk = (u8 *)gOvl21SceneControl;
            if (*(s32 *)(blk + 0x14) == 1) {
                if ((s32)Rand(100) < *(s16 *)(blk + 0x678) -
                    (*(s16 *)(blk + 0x67A) * ((UnkStruct800C0448 *)blk)[i].unk22) / 255) {
                    ((UnkStruct800C0448 *)blk)[i].unk2C = 0;
                } else if ((s32)Rand(100) < *(s16 *)(blk + 0x67C)) {
                    ((UnkStruct800C0448 *)blk)[i].unk2C = Ovl21PickRandomInactiveRecord(0);
                } else {
                    ((UnkStruct800C0448 *)blk)[i].unk2C = Ovl21PickRandomActiveRecord(2);
                }
            } else {
                ((UnkStruct800C0448 *)blk)[i].unk2C = Ovl21PickRandomActiveRecord(0);
            }
            break;
        default:
            ((UnkStruct800C0448 *)gOvl21SceneControl)[i].unk2C = Ovl21PickRandomActiveRecord(0);
            break;
        }
    }
}

void Ovl21UpdateRecordActions(void) {
    s32 i;
    s32 nearSide;
    s32 farSide;
    UnkStruct800C0448 *base;
    u8 *raw;
    u8 *pos;

    for (i = 1; i < 6; i++) {
        base = (UnkStruct800C0448 *)gOvl21SceneControl;
        raw = (u8 *)base;
        pos = raw + 0x1A;
        nearSide = i < 3;
        farSide = i >= 3;
        if (D_800C0448_init.records[i].unk14 == 1) {
            if (Ovl21CountActiveRecordsInGroup(farSide) >= 2) {
                if (Ovl21FindFirstProjectileInRange((Ovl21DistanceOrigin *)(pos + i * 0x108), *(s16 *)(raw + 0x65A), nearSide)) {
                    if (base[i].unk18 == 6) {
                        if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x672)) {
                            base[i].unk18 = 2;
                        }
                    } else {
                        if ((s32)Rand(0x80) > *(s16 *)(raw + 0x66E)) {
                            base[i].unk18 = 6;
                        } else if ((s32)Rand(0x80) > *(s16 *)(raw + 0x670)) {
                            base[i].unk18 = 2;
                        }
                    }
                } else {
                    if (base[i].unk18 == 6) {
                        if ((s32)Rand(0x80) > *(s16 *)(raw + 0x66C)) {
                            base[i].unk18 = 2;
                        }
                    } else if ((s32)Rand(0x80) > *(s16 *)(raw + 0x66A)) {
                        Ovl21MarkOppositeGroupTarget((Ovl21Func800B98CCArg *)&D_800C0448_init.records[i].unk10);
                    }
                }
            } else {
                if (base[i].unk18 == 6) {
                    if (Ovl21FindFirstProjectileInRange((Ovl21DistanceOrigin *)(pos + i * 0x108), *(s16 *)(raw + 0x65A), nearSide)) {
                        if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x668)) {
                            base[i].unk18 = 2;
                        }
                    } else if ((s32)Rand(0x80) > *(s16 *)(raw + 0x666)) {
                        base[i].unk18 = 2;
                    }
                } else {
                    if (Ovl21FindFirstProjectileInRange((Ovl21DistanceOrigin *)(pos + i * 0x108), *(s16 *)(raw + 0x65A), nearSide)) {
                        if ((s32)Rand(0x80) > *(s16 *)(raw + 0x662)) {
                            base[i].unk18 = 6;
                        } else if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x664)) {
                            Ovl21MarkOppositeGroupTarget((Ovl21Func800B98CCArg *)&D_800C0448_init.records[i].unk10);
                        }
                    } else if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x674)) {
                        Ovl21MarkOppositeGroupTarget((Ovl21Func800B98CCArg *)&D_800C0448_init.records[i].unk10);
                    }
                }
            }
        }
    }
}

void Ovl21SpawnRecordProjectiles(void) {
    u8 *base;
    s32 i;
    s32 j;
    s16 dx;
    s16 dz;
    s32 velocityX;
    s32 velocityZ;
    s16 speed;
    s32 life;
    s16 steps;
    s16 distance;
    s32 target;
    s32 quotient;
    s32 triple;
    s32 shotOffset;
    s32 lookup;
    s32 scanDisplacement;
    u16 *work;
    Ovl21SignedRecordView *record;
    UnkStruct800C0448 *state;
    Ovl21ProjectileView *shot;
    SVECTOR *position;

    for (i = 0; i < 6; i++) {
        if (D_800C0448_init.records[i].unk14 != 1) {
            D_800C0448_init.records[i].unk28 = 0;
        } else if (D_800C0448_init.records[i].unk28 == 1) {
            state = &((UnkStruct800C0448 *)gOvl21SceneControl)[i];
            if (state->unk18 != 2) {
                state->unk18 = 2;
                D_800C0448_init.records[i].unk28 = 0;
            } else if (!(((u16 *)D_800C0448_init.records[i].unk30)[1] & 0x100)) {
                D_800C0448_init.records[i].unk28 = 0;
            } else {
                if (i < 3) {
                    if (state->unk2C < 3) {
                        D_800C0448_init.records[i].unk28 = 0;
                        continue;
                    }
                } else if (state->unk2C >= 3) {
                    D_800C0448_init.records[i].unk28 = 0;
                    continue;
                }
                for (j = 0;
                    (base = (u8 *)gOvl21SceneControl),
                    j < *(s16 *)((u8 *)base + 0x64A) &&
                    ((scanDisplacement = -(j * sizeof(Ovl21InitSubstate) + i * 0x108)),
                    ((UnkStruct800C0448 *)(base - scanDisplacement))->unk34 != 0);
                    j++) {
                }
                if (j != *(s16 *)((u8 *)base + 0x64A)) {
                    record = &((Ovl21SignedRecordView *)base)[i];
                    record->unk18 = 4;
                    D_800C0448_init.records[i].unk28 = 0;
                    dx = ((Ovl21SignedRecordView *)base)[record->unk2C].unk1A - record->unk1A;
                    dz = ((Ovl21SignedRecordView *)base)[record->unk2C].unk1E - record->unk1E;
                    speed = record->unk24 * 400 / 3000;
                    distance = SquareRoot0(dx * dx + dz * dz);
                    life = (s16)(*(s16 *)((u8 *)base + 0x652) * 400 / speed);
                    steps = distance / speed;
                    velocityX = dx / steps;
                    velocityZ = dz / steps;
                    shotOffset = j * sizeof(Ovl21InitSubstate);
                    shotOffset += i * 0x108;
                    lookup = -shotOffset;
                    work = (u16 *)(base - lookup);
                    ((UnkStruct800C0448 *)work)->unk34 = 1;
                    position = (SVECTOR *)((u8 *)(gOvl21SceneControl + 0x20) + shotOffset);
                    position->vx = record->unk1A;
                    position->vy = -350;
                    position->vz = record->unk1E;
                    work = (u16 *)&record->unk2C;
                    quotient = (s16)((s16)*work / 3);
                    triple = quotient + quotient + quotient;
                    target = (s16)*work - triple;
                    target = D_800BCC24[i * 3 + target];
                    shot = (Ovl21ProjectileView *)((u8 *)gOvl21SceneControl + shotOffset);
                    shot->steps = steps;
                    shot->life = life;
                    shot->duration = life;
                    shot->kind = target;
                    setVector((SVECTOR *)((u8 *)(gOvl21SceneControl + 0x24) + shotOffset), velocityX, 0, velocityZ);
                    *(s16 *)((u8 *)base + 0x984) = PlaySoundEffect(0x40);
                }
            }
        }
    }
}

int ccos (int a);

typedef struct {
    u8 pad_00[0x34];
    s32 unk34;
    u16 unk38;
    u16 unk3A;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u16 unk42;
    u16 unk44;
    u16 pad_46;
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
} Ovl21Func800BA300Rec;

/* Steps the three 0x4C-byte projectile states of each of the six
 * D_800C0448 records: counts down unk3A, advances the position by the
 * velocity, recomputes the vertical velocity from a cosine, and clears the
 * slot's active word once unk3A passes -0x3B.
 *
 * Three constructs are here for the compiler, not for behaviour:
 *
 * - "raw" is assigned inside the outer loop, and the dead "base = raw;" at the
 *   end of the outer body stretches raw's in-loop lifetime. loop.c then rates
 *   moving its LO_SUM out as desirable (29 x savings x lifetime >= insns), and
 *   it shares the prologue HIGH that GCSE inserted (lui s6 / addiu s7,s6).
 *   Without the dead binding the LO_SUM stays in the loop.
 *
 * - "flags = (s32 *)((s32)flags | zero);" is a no-op that keeps the load's
 *   and the store's flags + offset apart. GCSE's cprop turns zero into 0 but
 *   validate_replace_rtx only simplifies PLUS/MINUS, so the IOR stays a set
 *   of flags. PRE then sees the expression killed between the load and the
 *   store and leaves the store's add in place, and loop.c sees flags as
 *   non-invariant and builds no giv to combine and strength-reduce. cse2
 *   folds the IOR to a self-copy and deletes it. Without it the two adds are
 *   merged and reduced into a pointer stepped by 0x4C.
 *
 * - The record base is spelled D_800C0458 - 0x10, a different HIGH from
 *   D_800C0448, so it gets its own lui in the outer body as in the target.
 *
 * Mechanism: notes/research/ovl_21_func_800BA300-shared-high.md. */
void Ovl21StepRecordProjectiles(void) {
    s16 steps;
    s16 life;
    s32 active;
    s32 angle;
    s32 offset;
    s32 slot;
    s32 zero;
    s32 group;
    s32 *flags;
    Ovl21Func800BA300Rec *state;
    Ovl21Func800BA300Rec *record;
    u16 *velocity;
    u8 *raw;
    u8 *base;

    zero = 0;
    group = 0;
    do {
        slot = 0;
        raw = (u8 *) gOvl21SceneControl;
        if ((*(s16 *) (raw + 0x64A)) > 0) {
            base = (u8 *) gOvl21RecordObjectBase - 0x10;
            offset = group * 0x108;
            state = (Ovl21Func800BA300Rec *) (base + offset);
            flags = (s32 *) (base + 0x34);
            do {
                active = *(s32 *)(offset + (s32)flags);
                if (active != 0) {
                    flags = (s32 *)((s32)flags | zero);
                    life = --state->unk3A;
                    state->unk40 = (u16) (state->unk40 + state->unk48);
                    state->unk42 += state->unk4A;
                    steps = state->unk3E;
                    state->unk44 = (u16) (state->unk44 + state->unk4C);
                    angle = state->unk3C;
                    if (steps != 0) {
                        angle = (s32) ((angle - life) << 0xC) / (s32) (steps * 2);
                        state->unk4A = (u16)(angle < 0x800
                            ? -(ccos(angle) * 0x18 / 0x1000)
                            : state->unk4A + 0xC);
                        record = (Ovl21Func800BA300Rec *) ((u8 *) gOvl21SceneControl + offset);
                        if ((s16) record->unk42 >= 0) {
                            velocity = (u16 *) ((u8 *) gOvl21SceneControl + 0x48 + offset);
                            record->unk42 = 0;
                            velocity[0] = 0;
                            velocity[1] = 0;
                            velocity[2] = 0;
                            record->unk38 = 1;
                        }
                        if ((s16) record->unk3A < -0x3B) {
                            *(s32 *)(offset + (s32)flags) = 0;
                        }
                    }
                }
                offset += 0x4C;
                state = (Ovl21Func800BA300Rec *) ((u8 *) state + 0x4C);
            } while (++slot < (*(s16 *) (base + 0x64A)));
        }
        base = raw;
        group += 1;
    } while (group < 6);
}

/*
 * scene_draw — reconstructed translation unit.
 *
 * Scene drawing traversal: visits six record sprites, their three active slot
 * sprites, and the primary indexed sprite; a separate entry draws the
 * centered frame.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

void Ovl21DrawRecordSprites(void);

void Ovl21DrawActiveSlotSprites(void);

void Ovl21DrawPrimarySpriteAtSelectedOffset(void);

void Ovl21DrawSceneSprites(void) {
    Ovl21DrawRecordSprites();
    Ovl21DrawActiveSlotSprites();
    Ovl21DrawPrimarySpriteAtSelectedOffset();
}

void Ovl21DrawSceneFrame(void) {
    Ovl21DrawCenteredFrameRects();
}

void Ovl21DrawRecordSprites(void) {
    u16 markerPosition[3];
    u8 *objectBase;
    UnkStruct800C0448 *record;
    s32 remaining;
    s32 *object;
    SpriteSourceData *markerSprite;

    objectBase = gOvl21RecordObjectBase;
    markerSprite = (SpriteSourceData *)(objectBase + 0x6D4);
    record = (UnkStruct800C0448 *)(objectBase - 0x10);
    object = (s32 *)objectBase;
    remaining = 5;
    do {
        Ovl21DrawRecordSprite(object);
        if (record->unk18 == 2 && (((SpriteSourceData *)record->unk30)->field_2 & 0x100)) {
            markerPosition[0] = record->unk1A;
            markerPosition[1] = record->unk1C - 0x15E;
            markerPosition[2] = record->unk1E;
            SelectSpriteAnimation((ObjectState *)markerSprite, 0);
            Ovl21SubmitProjectedSprite(markerSprite, markerPosition);
        }
        record = (UnkStruct800C0448 *)((u8 *)record + 0x108);
        remaining -= 1;
        object = (s32 *)((u8 *)object + 0x108);
    } while (remaining >= 0);
}

void Ovl21DrawActiveSlotSprites(void) {
    u8 *sceneBase;
    s32 *slots;
    s32 *slot;
    s32 *active;
    s32 recordIndex;
    s32 nextRecord;
    s32 slotsRemaining;
    s32 recordOffset;

    recordIndex = 0;
    sceneBase = (u8 *)gOvl21SceneControl;
    slots = (s32 *)(sceneBase + 0x34);
    while (recordIndex < 6) {
        nextRecord = recordIndex + 1;
        recordOffset = ((recordIndex << 5) + recordIndex) << 3;
        slot = (s32 *)(recordOffset + (s32)slots);
        active = slot;
        for (slotsRemaining = 2; slotsRemaining >= 0; slotsRemaining--) {
            if (*active != 0) {
                Ovl21DrawSlotSprite(slot);
            }
            slot = (s32 *)((u8 *)slot + 0x4C);
            active = (s32 *)((u8 *)active + 0x4C);
        }
        recordIndex = nextRecord;
    }
}

void Ovl21DrawPrimarySpriteAtSelectedOffset(void) {
    Ovl21DrawPrimarySceneSprite(gOvl21PrimarySpriteOffsetIndex);
}

/* Find the first active projectile in three records starting at group * 3,
 * scanning each record's 0x4C slices in order. Return its writable active-word
 * address, or NULL; this search does not consume it. Group/index bounds are
 * unchecked. X/Z differences and the integer distance wrap to signed s16.
 */
s32 *Ovl21FindFirstProjectileInRange(Ovl21DistanceOrigin *origin, s16 radius, s16 group) {
    s16 dz;
    s16 dx;
    s32 *active;
    s32 slotIndex;
    s32 recordOffset;
    s32 recordIndex;
    Ovl21ProjectileSearchView *slot;
    u8 *innerCountBase;
    u8 *countBase;
    s32 activeWord;

    recordIndex = group * 3;
    if (recordIndex < group * 3 + 3) {
        /* The guarded loop and cache birth retain separate entry/slot-count
         * address lifetimes; do not merge every traversal into one base. */
        countBase = (u8 *)gOvl21SceneControl;
        do {
            recordOffset = recordIndex * 0x108;
            slotIndex = 0;
            if (slotIndex < *(s16 *)(countBase + 0x64A)) {
                innerCountBase = (u8 *)gOvl21SceneControl;
                /* Reuse the address scratch for the loaded active flag.
                 * Both values are consumed; splitting this source variable
                 * changes its local/global allocation, not the program. */
                activeWord = (s32)((u8 *)gOvl21SceneControl + 0x34);
                active = (s32 *)(recordOffset + activeWord);
                slot = (Ovl21ProjectileSearchView *)((u8 *)gOvl21SceneControl + recordOffset);
                do {
                    activeWord = slot->active;
                    if ((activeWord != 1) ||
                        (dx = slot->x - origin->x,
                         dz = slot->z - origin->z,
                         radius < (s16)SquareRoot0(dx * dx + dz * dz))) {
                        active += 0x13;
                        slot = (Ovl21ProjectileSearchView *)((u8 *)slot + 0x4C);
                        slotIndex++;
                        if (slotIndex >= *(s16 *)(innerCountBase + 0x64A)) {
                            break;
                        }
                    } else {
                        return active;
                    }
                } while (1);
            }
            recordIndex++;
        } while (recordIndex < group * 3 + 3);
    }
    return NULL;
}

/* Read-only queries over groups 0 (records 0..2) and 1 (3..5).
 * Count tests record +0x14 == 1, not the projectile flag at +0x34.
 * Active picker uses mode % 2; mode 2 excludes record zero. Inactive picker
 * tests +0x14 == 0 in the requested group. Pickers return an absolute record
 * index, or -1 if none; a successful pick consumes exactly one Rand call.
 * Group/mode bounds are unchecked. Callers consume/change results separately.
 */
s32 Ovl21CountActiveRecordsInGroup(s16 group) {
    s32 count;
    s32 previous;
    s32 index;
    s32 activeValue;
    s32 nextCountQ16;
    s32 countStepQ16;
    UnkStruct800C0448 *base;
    s32 *active;

    count = 0;
    index = group * 3;
    if (index < group * 3 + 3) {
        activeValue = 1;
        base = (UnkStruct800C0448 *)gOvl21SceneControl;
        active = (s32 *)((u8 *)base + ((((index << 5) + index) << 3) + 0x14));
        nextCountQ16 = 0x10000;
        countStepQ16 = 0x10000;
        for (; index < group * 3 + 3; index++) {
            if (*active == activeValue) {
                previous = nextCountQ16;
                nextCountQ16 += countStepQ16;
                count = previous >> 16;
            }
            active = (s32 *)((u8 *)active + 0x108);
        }
    }
    return count;
}

s16 Ovl21PickRandomActiveRecord(s16 mode) {
    s16 candidates[4];
    s16 count;
    s32 groupMode;
    s32 index;

    groupMode = mode;
    count = 0;
    for (index = (groupMode % 2) * 3; index < (groupMode % 2) * 3 + 3; index++) {
        candidates[count] = 0;
        if ((mode != 2 || index != 0) && ((UnkStruct800C0448 *)gOvl21SceneControl)[index].unk14 == 1) {
            candidates[count] = index;
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    return candidates[Rand((u16)count)];
}

s16 Ovl21PickRandomInactiveRecord(s16 group) {
    s16 candidates[4];
    s16 count;
    s32 index;

    count = 0;
    for (index = group * 3; index < group * 3 + 3; index++) {
        candidates[count] = 0;
        if (((UnkStruct800C0448 *)gOvl21SceneControl)[index].unk14 == 0) {
            candidates[count] = index;
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    return candidates[Rand((u16)count)];
}

/* Animation pause, not visibility: mask 4 makes UpdateSpriteAnimation skip
 * sixteen primary and eighteen record-slot sources, including inactive slots.
 * Drawing remains enabled; other flags, frames and counters are unchanged. */

/* Kept externally linked; co-location alone does not prove static linkage. */
void Ovl21SetSceneSpritePauseMode(s32 paused);

void Ovl21PauseSceneSprites(void) {
    Ovl21SetSceneSpritePauseMode(1);
}

void Ovl21ResumeSceneSprites(void) {
    Ovl21SetSceneSpritePauseMode(0);
}

void Ovl21SetSceneSpritePauseMode(s32 paused) {
    s32 index;
    s32 slotIndex;
    struct SpriteSourceData *sprite;
    u16 *record;
    void (*updateFlags)(u16 *, s32);

    if (paused != 0) {
        updateFlags = AddSpriteSourceFlags;
    } else {
        updateFlags = ClearSpriteSourceFlags;
    }
    updateFlags(&gOvl21PrimarySceneSprite.field_0, 4);
    updateFlags(&(&gOvl21PrimarySceneSprite)[1].field_0, 4);
    updateFlags(&(&gOvl21PrimarySceneSprite)[2].field_0, 4);
    sprite = &gOvl21PrimarySceneSprite + 3;
    for (index = 0; index < 13; index++) {
        updateFlags(&sprite->field_0, 4);
        sprite++;
    }
    for (index = 0; index < 6; index++) {
        record = (u16 *) (gOvl21RecordObjectBase + index * 0x108);
        for (slotIndex = 0; slotIndex < 3; slotIndex++) {
            updateFlags((u16 *) ((u8 *) record + slotIndex * 0x4C + 0x40), 4);
        }
    }
}

/* ID-keyed inputs to record initialization and result publication. IDs index
 * persistent work records at stride 0x1D4, unchecked. IDs 7/9/20 use unsigned
 * work+8000+19EC and a square-root read; other IDs use signed +19EA directly.
 * The writer skips ID 0; the reader does not. Updates add 500 or 20 and use
 * the original threshold-to-sentinel conversion, not ordinary saturation.
 * The class constants remain mutable data. Sprite lookup returns one of 13
 * retained source objects (including the shared default), not an allocation.
 * Gameplay units of these range values are not established. */
void Ovl21RaiseEntityRangeValue(s16 entityId) {
    if (entityId != 0) {
        if (entityId == 7 || entityId == 9 || entityId == 20) {
            char *work = (char *)&D_8006C838;
            char *record = work + entityId * 0x1D4;
            s32 value = *(u16 *)(record + 0x8000 + 0x19EC) + 0x1F4;
            if (value > 0xFE0A) {
                value = 0xFFFF;
            }
            *(u16 *)(record + 0x8000 + 0x19EC) = value;
            return;
        } else {
            char *work = (char *)&D_8006C838;
            char *record = work + entityId * 0x1D4;
            s32 value = *(s16 *)(record + 0x8000 + 0x19EA) + 0x14;
            if (value >= 0xEB) {
                value = 0xFF;
            }
            *(s16 *)(record + 0x8000 + 0x19EA) = value;
        }
    }
}

s16 Ovl21ReadEntityRangeValue(s16 entityId) {
    s32 root;

    if (entityId == 7 || entityId == 9 || entityId == 20) {
        char *work;
        char *record;
        work = (char *)&D_8006C838;
        record = work + entityId * 0x1D4;
        root = SquareRoot0(*(u16 *)(record + 0x8000 + 0x19EC));
        return (s16)root;
    }
    {
        char *work;
        char *record;
        work = (char *)&D_8006C838;
        record = work + entityId * 0x1D4;
        return *(s16 *)(record + 0x8000 + 0x19EA);
    }
}

s32 Ovl21GetEntityRangeConstant(s16 entityId) {
    if (entityId == 0x10 || entityId == 0x22) {
        return gOvl21EntityRange360;
    }
    if (entityId == 7 || entityId == 9 || entityId == 0x14 || entityId == 0x16) {
        return gOvl21EntityRange420;
    }
    return gOvl21EntityRange480;
}

u8 *Ovl21GetEntityObjectRecord(s16 entityId) {
    if (entityId == 5) {
        return gOvl21EntitySprite5;
    }
    if (entityId == 6) {
        return gOvl21EntitySprite6;
    }
    if (entityId == 7) {
        return gOvl21EntitySprite7;
    }
    if (entityId == 9) {
        return gOvl21EntitySprite9;
    }
    if (entityId == 11) {
        return gOvl21EntitySprite11;
    }
    if (entityId == 16) {
        return gOvl21EntitySprite16;
    }
    if (entityId == 19) {
        return gOvl21EntitySprite19;
    }
    if (entityId == 20) {
        return gOvl21EntitySprite20;
    }
    if (entityId == 21) {
        return gOvl21EntitySprite21;
    }
    if (entityId == 22) {
        return gOvl21EntitySprite22;
    }
    if (entityId == 25) {
        return gOvl21EntitySprite25;
    }
    if (entityId == 34) {
        return gOvl21EntitySprite34;
    }
    return gOvl21DefaultEntitySprite;
}

/* Select animation only when changed, draw, then advance animation. Projection
 * reads unsigned position halfwords, adds {150,-100,-150}, and narrows into
 * signed GTE scratch before selecting field_120's OT. Records 0/1/2/5 receive
 * an extra screen pass. The primary source uses {offset.x,-500,offset.z}.
 * The centered rect source instead resets animation/frame 0 every call.
 * Shared scratch makes these non-reentrant; no clipping/capacity validation
 * or GPU presentation occurs here. */
void Ovl21DrawRecordSprite(s32 *record) {
    s16 animationBase;
    s32 animationOffset;

    animationBase = *(s16 *) ((u8 *) record + 8);
    animationOffset = *(s16 *) ((u8 *) record + 2) < 3;
    if (animationBase == 9) {
        animationOffset = animationOffset == 0;
    }
    if (animationBase == 6) {
        animationOffset = 0;
    }
    /* The comparison uses the signed selector, the setter its low byte. */
    if ((animationBase + animationOffset) !=
        (*(SpriteSourceData **) ((u8 *) record + 0x20))->field_4) {
        SelectSpriteAnimation((ObjectState *) (*(SpriteSourceData **) ((u8 *) record + 0x20)),
                              (*(u8 *) ((u8 *) record + 8) + animationOffset) & 0xFF);
    }
    Ovl21SubmitProjectedSprite(*(SpriteSourceData **) ((u8 *) record + 0x20),
                              (u16 *) ((u8 *) record + 0xA));
}

void Ovl21DrawSlotSprite(s32 *slot) {
    if ((*(s16 *) ((u8 *) slot + 4)) != (*(u8 *) ((u8 *) slot + 0x20))) {
        SelectSpriteAnimation((ObjectState *) (slot + 7), *(u8 *) ((u8 *) slot + 4));
    }
    Ovl21SubmitProjectedSprite((SpriteSourceData *) (slot + 7), (u16 *) (slot + 3));
}

void Ovl21DrawPrimarySceneSprite(s16 offsetIndex) {
    u8 *base;
    s16 position[3];
    u16 x;

    base = (u8 *) gOvl21SceneControl;
    if (base[0x688] != 0xE) {
        SelectSpriteAnimation((ObjectState *) (base + 0x684), 0xE);
    }
    x = gOvl21SceneOffsetPairs[offsetIndex].x;
    position[1] = -0x1F4;
    position[0] = x;
    position[2] = gOvl21SceneOffsetPairs[offsetIndex].z;
    Ovl21SubmitProjectedSprite((SpriteSourceData *) (base + 0x684), (u16 *) position);
}

void Ovl21DrawCenteredFrameRects(void) {
    u8 *base;

    SelectSpriteAnimation((ObjectState *)&gOvl21FrameRectSprite, 0);
    base = gOvl21FrameRectSprite - 0x6B4;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl21FrameRectSprite,
                        base[0x6B8], base[0x6B9], (s16)(HWD0 / 2), (s16)(VWD0 / 2));
}

void Ovl21SubmitProjectedSprite(SpriteSourceData *source, u16 *position) {
    s32 otAddress;

    gOvl21SpriteProjectPosition[0] = position[0] + 0x96;
    gOvl21SpriteProjectPosition[1] = position[1] - 0x64;
    gOvl21SpriteProjectPosition[2] = position[2] - 0x96;
    otAddress = gActiveRenderContext->field_120 +
        ((ProjectPointToScreen(gOvl21SpriteProjectScreen, (SVECTOR *) gOvl21SpriteProjectPosition) >> 2) * 4);
    DrawSpriteAnimationQuadsAndUpdate(otAddress, source,
        *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 0),
        *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 4));
    /* Only these four record-owned sources get the additional screen pass. */
    if ((source == ((UnkStruct800C0448 *) gOvl21SceneControl)[0].unk30) ||
        (source == ((UnkStruct800C0448 *) gOvl21SceneControl)[1].unk30) ||
        (source == ((UnkStruct800C0448 *) gOvl21SceneControl)[2].unk30) ||
        (source == ((UnkStruct800C0448 *) gOvl21SceneControl)[5].unk30)) {
        DrawUiSprite18Frame(otAddress,
            *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 0),
            *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 4), 0);
    }
}

/* Resource lifetime: sprite headers and geometry survive all frame draws.
 * Archive stages wait for DrawSync, clear the 0x800-entry OT, and busy-poll CD
 * reads without timeout. Each overwrites heap scratch at +4290; retained
 * headers are copied out before the next stage. Geometry must load before
 * drawing or surface queries. Sound setup queues I/O; the phase machine waits
 * for completion before camera startup.
 *
 * Camera units are integer world coordinates and 4096 yaw units per turn.
 * Each frame updates -> applies -> draws; update preserves the GTE matrix
 * stack and narrows coordinates to signed halfwords. D_8007AFF0's pointer
 * and its work-area storage remain deliberately separate views. */

/* Offsets from D_8007AFF0. Word resets and low-halfword reads intentionally
 * retain their original widths; computed eye coordinates wrap to halfwords.
 * The two reset-only words at +2539C/+253A4 remain unnamed. */
enum {
    CAMERA_DISTANCE = 0x25394,
    CAMERA_HEIGHT = 0x25398,
    CAMERA_YAW = 0x253A0,
    CAMERA_OFFSET_X = 0x253AC,
    CAMERA_EYE_Y = 0x253AE,
    CAMERA_OFFSET_Z = 0x253B0,
    CAMERA_TARGET_X = 0x253B4,
    CAMERA_TARGET_Y = 0x253B6,
    CAMERA_TARGET_Z = 0x253B8
};

void Ovl21InitSceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    *(s32 *)(camera + CAMERA_DISTANCE) = -0x2328;
    *(s32 *)(camera + CAMERA_HEIGHT) = -0x1194;
    *(s32 *)(camera + 0x2539C) = 0;
    *(s32 *)(camera + CAMERA_YAW) = -0x200;
    *(s32 *)(camera + 0x253A4) = 0;
    *(s16 *)(camera + CAMERA_TARGET_X) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Y) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Z) = 0;
}

void Ovl21UpdateSceneCameraOffset(void) {
    char *camera;
    MATRIX rotation;
    MATRIX identity;
    SVECTOR vector;
    VECTOR rotated;
    long flag;

    rotation = GsIDMATRIX;
    identity = GsIDMATRIX;
    PushMatrix();
    camera = (char *)&D_8007AFF0;
    vector.vx = 0;
    vector.vy = *(u16 *)(camera + CAMERA_YAW);
    vector.vz = 0;
    RotMatrix(&vector, &rotation);
    SetRotMatrix(&rotation);
    SetTransMatrix(&identity);
    vector.vx = 0;
    vector.vy = 0;
    vector.vz = *(u16 *)(camera + CAMERA_DISTANCE);
    RotTrans(&vector, &rotated, &flag);
    PopMatrix();
    *(u16 *)(camera + CAMERA_OFFSET_X) = rotated.vx;
    *(s16 *)(camera + CAMERA_EYE_Y) = *(u16 *)(camera + CAMERA_HEIGHT) + *(u16 *)(camera + CAMERA_TARGET_Y);
    *(u16 *)(camera + CAMERA_OFFSET_Z) = rotated.vz;
}

void Ovl21ApplySceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X),
                       *(s16 *)(camera + CAMERA_EYE_Y),
                       *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z),
                       0);
    SetCameraTarget(*(s16 *)(camera + CAMERA_TARGET_X),
                    *(s16 *)(camera + CAMERA_TARGET_Y),
                    *(s16 *)(camera + CAMERA_TARGET_Z));
}

void Ovl21DrawSceneGeometry(void) {
    char *camera;
    VECTOR eye;
    VECTOR target;

    camera = (char *)&D_8007AFF0;
    eye.vx = *(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X);
    eye.vy = *(s16 *)(camera + CAMERA_EYE_Y);
    eye.vz = *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z);
    target.vx = *(s16 *)(camera + CAMERA_TARGET_X);
    target.vy = *(s16 *)(camera + CAMERA_TARGET_Y);
    target.vz = *(s16 *)(camera + CAMERA_TARGET_Z);
    DrawCulledProjectedPrimitives((ProjectedPrimitiveSource *)&gOvl21SceneGeometry, &eye, &target);
}

void Ovl21LoadSceneSpriteArchive(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl21SceneSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData(&gOvl21PrimarySceneSprite, (SpriteDataHeader *)gOvl21SceneSpriteData);
}

void Ovl21UploadSceneSpriteChunk(void) {
    DrawSync(0);
    ClearOTagR((u32 *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, gGraphicsHeapBase + 0x4290, 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0, 0x2000, gGraphicsHeapBase + 0x4290, 1, 0) == 0);
    UploadSpriteImages(gGraphicsHeapBase + 0x4290);
}

void Ovl21LoadUiAndObjectSprites(void) {
    SpriteDataHeader *header;
    SpriteSourceData *sprite;
    u8 *record;
    s32 index;
    s32 spriteIndex;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x16, 0, 0x16188, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x16, 0, 0x16188, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x65CC));
    memcpy(gOvl21UiSpriteData, (u8 *)(gGraphicsHeapBase + 0x4290), 0x233C);
    header = (SpriteDataHeader *)(gOvl21UiSpriteData + gOvl21UiSpriteOffsets[0]);
    /* This anchor is the second UI source. Preserve the -1 and +1 biases
     * and the 0x30-byte source stride rather than inventing a new array. */
    InitSpriteSourceData(&gOvl21UiSpriteAnchor, header);
    InitSpriteSourceData(&gOvl21UiSpriteAnchor - 1, (SpriteDataHeader *)(gOvl21UiSpriteData + gOvl21UiSpriteOffsets[1]));
    for (index = 0; index < 13; index++) {
        InitSpriteSourceData(&gOvl21UiSpriteAnchor + index + 1, (SpriteDataHeader *)(gOvl21UiSpriteData + gOvl21UiSpriteOffsets[index + 2]));
    }
    /* All eighteen embedded sources intentionally reuse table entry zero. */
    for (index = 0; index < 6; index++) {
        record = gOvl21RecordObjectBase + index * 0x108;
        for (spriteIndex = 0; spriteIndex < 3; spriteIndex++) {
            sprite = (SpriteSourceData *)(record + spriteIndex * 0x4C + 0x40);
            InitSpriteSourceData(sprite, header);
        }
    }
}

/* The name-bound descriptor address and existing compiler barrier preserve
 * the target's copy/binding sequence without changing the descriptor view. */
void Ovl21LoadSceneGeometry(void) {
    s32 *discPositions;
    u8 *geometry;

    discPositions = gOvl21SceneDiscPositions;
    CdReadDiscSectorsBlocking(0, discPositions[0], discPositions[1] - discPositions[0], 0, D_8007AFF0);
    CdReadDiscSectorsBlocking(0, discPositions[1], discPositions[2] - discPositions[1], 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    memcpy(&gLoadedAreaHeaderCopy, (void *)D_8007AFF0, 0xBDC);
    geometry = &gOvl21SceneGeometry;
    __asm__("" : : : "memory");
    InitProjectedPrimitiveSource(geometry, (void *)(D_8007AFF0 + 0x2000));
    EnableSurfaceCells((void **)geometry);
    SetSurfaceHeightTolerance(0xC8);
}

void Ovl21QueueSceneSound(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x12, 1);
}
