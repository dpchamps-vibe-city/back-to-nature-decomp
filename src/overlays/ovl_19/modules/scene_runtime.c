/*
 * Overlay 19 scene runtime, all 76 original entries in link order.
 *
 * The outer phase machine loads resources and gates fades; its update phase
 * drives the mode/stage machines, actor pairs, projected rendering and UI.
 * Camera/resource, actor-control and value-display sections share this scene's
 * work and lifecycle, not separately compiled modules. Their existing headers
 * document the narrower contracts; ovl19_scene_runtime.h publishes the frame
 * entry used by the executable after loading this overlay.
 *
 * Original text: ovl_19:800B7ED8–800BBD74 exclusive. Unknown mode meanings and
 * persistent flag fields deliberately retain neutral names.
 */

#include "common.h"
#include "ovl19_scene_runtime.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "ovl19_actor_geometry.h"
#include "sound_commands.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl19_record_value_increment.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "psyq/libgs.h"
#include "projected_primitives.h"
#include "ovl19_scene_camera_geometry.h"
#include "message_window.h"
#include "ovl19_scene_resources.h"
#include "packet_work.h"
#include "ovl19_value_table_display.h"
#include "ovl19_projected_sprite.h"
#include "ovl19_actor_behavior_states.h"
#include "ovl19_actor_controls.h"
#include "ovl19_projected_actor_render.h"
#include "random.h"
#include "point_projection.h"
#include "surface_collision.h"
#include "ui_sprites.h"
#include "psyq/libapi.h"
#include "text_engine.h"

void Ovl19StepScene(void) {
    s32 *handlers;
    u32 savedTextSetting;

    savedTextSetting = GetTextPalette();
    SetTextPalette(3U);
    handlers = &gOvl19PhaseHandlerWords;
    ((void (*)(s32 *))handlers[gSceneSubphase])(handlers);
    SetTextPalette(savedTextSetting);
}

void Ovl19InitSceneWork(void);

void Ovl19LoadScenePhase(void) {
    s32 *base;

    switch (gOvl19LoadStep) {
    case 0:
        Ovl19LoadSceneSpriteArchive();
        gOvl19LoadStep++;
        break;
    case 1:
        Ovl19UploadSceneSpriteChunk();
        gOvl19LoadStep++;
        break;
    case 2:
        Ovl19LoadUiAndActorSprites();
        gOvl19LoadStep++;
        break;
    case 3:
        Ovl19LoadSceneGeometry();
        gOvl19LoadStep++;
        break;
    case 4:
        Ovl19QueueSceneSound();
        gOvl19LoadStep++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            gOvl19LoadStep = 6;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0xF, 0x7F);
        }
        break;
    }
    if (gOvl19LoadStep == 6) {
        Ovl19InitSceneWork();
        Ovl19InitSceneCamera();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        gOvl19LoadStep = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}

void Ovl19WaitSceneFadeIn(void) {
    u8 var_s0;

    Ovl19UpdateSceneCameraOffset();
    Ovl19ApplySceneCamera();
    ApplyGteSceneState();
    Ovl19DrawSceneGeometry();
    Ovl19DrawValueTable();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
    }
}

void Ovl19UpdateScenePhase(void) {
    UpdateMessageWindow();
    Ovl19UpdateSceneCameraOffset();
    Ovl19ApplySceneCamera();
    ApplyGteSceneState();
    Ovl19DrawSceneGeometry();
    gOvl19ModeHandlers[gOvl19SceneMode]();
}

void Ovl19BeginSceneExit(void) {
    BeginScreenFadeFeedback(0xA);
    BeginMasterVolumeFadeOut(0xA);
    Ovl19DrawProjectedActorSprites();
    ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
}

void Ovl19DrawActorPairSprite (s16 *arg0);
void Ovl19DrawAuxRecordSprite (Ovl19Func800BAB4CArg *arg0);

void Ovl19FinishSceneExit(void) {
    u8 var_s0;
    u8 *base;

    Ovl19DrawProjectedActorSprites();
    var_s0 = 0;
    if (IsScreenFadeComplete() == 1) {
        var_s0++;
    }
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        if (gOvl19SoundVoice != -1) {
            StopSoundEffectVoice((s32) gOvl19SoundVoice);
        }
        StopSoundSequence(0);
        RewindSoundLoadSlots(2, 0);
        RewindSoundLoadSlots(1, 0);
        base = (u8 *)D_8006C838;
        *(s32 *)(base + 0x448C) = 0xFF;
        RequestSceneChange(6);
        *(s32 *)(base + 0xC) |= 0x80000;
        if (*(s32 *)(base + 0xE624) == 1) {
            Ovl19RaiseRecordByteRangeValue(1);
            Ovl19RaiseRecordByteRangeValue(4);
            Ovl19RaiseRecordByteRangeValue(5);
            Ovl19RaiseRecordByteRangeValue(6);
            Ovl19RaiseRecordByteRangeValue(8);
            Ovl19RaiseRecordByteRangeValue(0xF);
            Ovl19RaiseRecordByteRangeValue(0x11);
            Ovl19RaiseRecordByteRangeValue(0x13);
            Ovl19RaiseRecordByteRangeValue(0x15);
            Ovl19RaiseRecordByteRangeValue(0x19);
            Ovl19RaiseRecordByteRangeValue(0x1C);
            Ovl19RaiseRecordByteRangeValue(0x1F);
        }
    }
}

void Ovl19ResetActorStage(void);

void Ovl19InitSceneWork(void) {
    s32 i;
    s32 j;
    s32 r;

    i = 0;
    gOvl19SceneWork[2] = 0;
    gOvl19SceneWork[3] = 0;
    gOvl19SceneWork[4] = 0;
    gOvl19SceneWork[5] = 0;
    for (; i < 8; i++) {
        (*(gOvl19SceneWork + 0xB0 + i)) = 0xFF;
        r = Rand(0x14);
        for (j = 0; j <= i; j++) {
            if (gOvl19SceneWork[0xB0 + j] == r) {
                r++;
                if (r >= 0x14) {
                    r = 0;
                }
                j = 0;
            }
        }
        (*(gOvl19SceneWork + 0xB0 + i)) = r;
    }
    Ovl19ResetActorStage();
    gOvl19SceneWork[0xB9] = 0x2D;
    gOvl19SceneWork[0xBA] = 0x190;
    gOvl19SceneWork[0xB8] = 6;
    gOvl19SceneWork[0xBE] = 0x50;
    gOvl19SceneWork[0xBF] = 0xA;
    gOvl19SceneWork[0xC0] = 0xA;
    gOvl19SceneWork[0xC1] = 0x7D;
    gOvl19SceneWork[0xC2] = 0x32;
    gOvl19SceneWork[0xC3] = 0x73;
    gOvl19SceneWork[0xC4] = 0x3C;
    gOvl19SceneWork[0xC5] = 0x73;
    gOvl19SceneWork[0xBB] = 2;
    gOvl19SceneWork[0xBC] = 0x5A;
    gOvl19SceneWork[0xBD] = 0xF;
    gOvl19SceneWork[0xC6] = 0x3C;
    gOvl19SceneWork[0xC7] = 0x5A;
    gOvl19SceneWork[0xC8] = 0x78;
    gOvl19SceneWork[0xC9] = 0x5A;
    gOvl19SceneWork[0xCA] = 0x1E;
    gOvl19SceneWork[0xCB] = 0xA;
    gOvl19SceneWork[0xCC] = 0xA;
    gOvl19SceneWork[0xCD] = 0xB;
    gOvl19SceneWork[0xCE] = 0xA;
    gOvl19SceneWork[0xCF] = 0xB;
}

void Ovl19InitActorPair (void);
void Ovl19InitAuxRecords (void);
s32 Ovl19ChooseInitialActorValue (void);

void Ovl19ResetActorStage(void) {
    (*(s32 *) ((u8 *) gOvl19SceneWork + 0)) = 0;
    (*(s16 *) ((u8 *) gOvl19SceneWork + 0x202)) = -1;
    (*(s16 *) ((u8 *) gOvl19SceneWork + 0x204)) = -1;
    (*(s16 *) ((u8 *) gOvl19SceneWork + 0xC)) = -1;
    Ovl19InitActorPair();
    Ovl19InitAuxRecords();
}

typedef struct {
    s16 field_0;
    s16 field_2;
    s16 field_4;
    s16 field_6;
} Ovl19Rec8;

s32 Ovl19ChooseInitialActorValue(void);

void Ovl19InitActorPair(void) {
    s32 i;
    s16 *dst;
    Ovl19Rec8 *src;
    s16 *d;
    char *base;
    s16 two;

    i = 0;
    two = 2;
    src = (Ovl19Rec8 *)D_800BCEFC;
    do {
        dst = gOvl19SceneWork + i * 0x24;
        dst[8] = i;
        dst[9] = two;
        dst[0xB] = 0;
        *(s32 *)(dst + 0xE) = 0;
        *(Ovl19Rec8 *)(dst + 0x10) = *src;
        src++;
        i++;
    } while (i < 2);
    d = gOvl19SceneWork;
    d[0xA] = 0;
    d[0x2E] = 2;
    base = (char *)D_8006C838;
    d[0xC] = *(u16 *)(base + 0xE61C);
    d[0x30] = Ovl19ChooseInitialActorValue();
    d[0x31] = D_800BCEE8[d[4] + 1];
}

void Ovl19InitAuxRecords(void) {
    s32 i;
    s16 *dst;
    Ovl19Rec8 *src;
    s16 *d;
    u16 *e;
    s32 idx;
    char *base;
    s32 val;

    i = 0;
    src = (Ovl19Rec8 *)D_800BCF0C;
    do {
        dst = gOvl19SceneWork + i * 0x20;
        dst[0x50] = i;
        dst[0x51] = 0;
        dst[0x52] = 0;
        *(Ovl19Rec8 *)(dst + 0x54) = *src;
        src++;
        i++;
    } while (i < 3);
    d = gOvl19SceneWork;
    e = D_800BCEF0;
    idx = d[4] * 2;
    base = (char *)D_8006C838;
    val = (s16)*(u16 *)(base + 0xE61C);
    d[0x73] = e[idx + val % 2];
}

s32 Ovl19ChooseInitialActorValue(void) {
    s16 s0;
    s16 s1;
    s32 v;

    s0 = gCalendarYear;
    s1 = gOvl19ValueCategory;
    if (s0 >= 2) {
        s0 = 1;
    }
    if (s1 >= 3) {
        s0 = 2;
    }
    v = (s16)Rand(D_800BCFE4[s0 * 3 + s1]);
    if (s0 != 0 && s1 == 2) {
        v = (s16)(v + 0xAB);
    }
    return v;
}

s32 Ovl19GetValueLabelOffset (s16 arg0);
void ovl_19_func_800B868C(void) {
    Ovl19DrawProjectedActorSprites();
    Ovl19DrawValueTable();
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x60) {
        gOvl19SceneMode = 1;
    }
}
/* Warning: struct GfxObj is not defined (only forward-declared) */

void ovl_19_func_800B86DC(void) {
    s32 status;
    s32 result;

    Ovl19DrawProjectedActorSprites();
    switch (gOvl19SceneWork[4]) {
    case 0:
        gOvl19SceneWork[2] = 2;
        gOvl19SceneWork[3] = 0;
        return;
    case 1:
        RequestMessageWindow(4, 0xF);
        break;
    case 2:
        RequestMessageWindow(4, 0x10);
        break;
    }
    status = GetMessageWindowStatus();
    if (status == 2) {
        result = GetMessageWindowChoice();
        if (result == 1) {
            gOvl19SceneWork[0x100] = 0;
            gOvl19SceneWork[2] = 3;
        } else if (result == status) {
            gOvl19SceneWork[2] = result;
            gOvl19SceneWork[3] = 0;
        }
    }
}

void ovl_19_func_800B87C4(void) {
    void (*temp_a0)(void);

    if (gOvl19SceneWork[3] == 7) {
        gOvl19SceneWork[3] = 0;
        gOvl19SceneWork[2] = 3;
        return;
    }
    temp_a0 = gOvl19StageHandlers[gOvl19SceneWork[3]];
    temp_a0();
    Ovl19UpdateActorBehaviors();
    Ovl19DrawProjectedActorSprites();
}

void ovl_19_func_800B8830(void) {
    Ovl19ResetActorStage();
    Ovl19DrawProjectedActorSprites();
    SetTextInputEnabled(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, Ovl19_D_800BCFF0[gOvl19SceneWork[4]]);
    if (GetMessageWindowStatus() == 5) {
        gOvl19SceneWork[0x100]++;
        if (gOvl19SceneWork[0x100] >= 60) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x90], 4, 0);
            CloseMessageWindow();
            gOvl19SceneWork[0x100] = 0;
            gOvl19SceneWork[2] = 4;
        }
    }
}

/* Executable aliases: SetVal8005E2BC, SetMessagePortraitEnabled and GetMessageWindowStatus. */

void ovl_19_func_800B8908(void) {
    s32 state;

    Ovl19DrawProjectedActorSprites();
    SetTextInputEnabled(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 0x12);
    state = GetMessageWindowStatus();
    if (state == 5) {
        gOvl19SceneWork[0x100]++;
        if (gOvl19SceneWork[0x100] >= 45) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x90], 0, 0);
            CloseMessageWindow();
            gOvl19SceneWork[0x100] = 0;
            gOvl19SceneWork[2] = state;
        }
    }
}

void ovl_19_func_800B942C(void);
s32 ovl_19_func_800BA25C(void);
void ovl_19_func_800B9454(void);
void Ovl19UpdateActorCommands(void);
s32 ovl_19_func_800BA73C(Ovl19Func800BA73CArg *arg0);

void ovl_19_func_800B89C8(void) {
    s32 selection;
    s16 quotient;
    Ovl19Func800BAC40Arg *other;

    if (((s32 *)gPad0Input)[2] & 0x800) {
        *(s32 *)gOvl19SceneWork = *(s32 *)gOvl19SceneWork != 1;
        if (*(s32 *)gOvl19SceneWork == 1) {
            Ovl19PauseActorSprites();
            gOvl19SceneWork[0x101] = PlaySoundEffect(0x36);
        } else {
            Ovl19ResumeActorSprites();
        }
    }
    if (*(s32 *)gOvl19SceneWork == 1) {
        ovl_19_func_800B942C();
    } else {
        gOvl19SceneWork[5]++;
        if (gOvl19SceneWork[5] == 10000) {
            gOvl19SceneWork[5] = 0;
        }
        selection = ovl_19_func_800BA25C();
        if (selection != -1) {
            if (gOvl19SelectedActorIndex == -1) {
                gOvl19SelectedActorIndex = selection;
                RequestMessageWindow(4, 0x15);
                gOvl19SceneWork[0x102] = PlaySoundEffect(0x43);
                Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50 + selection * 0x20], 3, 0);
                other = (Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50];
                if (selection == 0) {
                    other = (Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70];
                }
                Ovl19SetActorState(other, 0, 0);
                Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x90], 4, 0);
            }
            if (GetMessageWindowStatus() == 2) {
                StopSoundEffectVoice(gOvl19SceneWork[0x102]);
                gOvl19SceneWork[2] = 6;
                gOvl19SceneWork[6] = gOvl19SelectedActorIndex;
                gOvl19SceneWork[0x102] = PlaySoundEffect(0x43);
            } else {
                Ovl19UpdateActorBehaviors();
            }
        } else {
            gOvl19SelectedActorIndex = -1;
            if (gOvl19SceneWork[0x51] == 1) {
                if (((s32 *)gPad0Input)[2] & 0x40) {
                    Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 2, 30);
                }
            } else {
                gOvl19SceneWork[0x52]--;
                if (gOvl19SceneWork[0x52] <= 0) {
                    Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
                }
            }
            ovl_19_func_800B9454();
            Ovl19UpdateActorCommands();
            Ovl19UpdateActorBehaviors();
            if (ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)&gOvl19SceneWork[8]) == 1) {
                quotient = gOvl19SceneWork[5] / 30;
                if (gOvl19SceneWork[5] == quotient * 30) {
                    gOvl19SceneWork[0x101] = PlaySoundEffect(0x3A);
                }
            }
        }
        Ovl19DrawProjectedActorSprites();
    }
}

s32 Ovl19GetValueLabelOffset(s16 arg0);

void ovl_19_func_800B8C74(void) {
    s32 flag;

    Ovl19DrawProjectedActorSprites();
    flag = ((Ovl19SelectionState *)gOvl19SceneWork)->flag;
    if (flag == 0) {
        SetTextSourcePointer(0, ((Ovl19CompletionCounterView *)D_8006C838)->text);
        RequestMessageWindow(4, 0x16);
    } else {
        SetTextSourcePointer(0, (s32)((u8 *)gOvl19ValueLabelText +
            Ovl19GetValueLabelOffset(((Ovl19SelectionState *)gOvl19SceneWork)->values[
                D_800BCF24[((Ovl19SelectionState *)gOvl19SceneWork)->selection]])));
        RequestMessageWindow(4, 0x17);
    }
    if (GetMessageWindowStatus() == 2) {
        ((Ovl19SelectionState *)gOvl19SceneWork)->selection++;
        StopSoundEffectVoice(((Ovl19SelectionState *)gOvl19SceneWork)->handle);
        if (flag == 0) {
            if (((Ovl19SelectionState *)gOvl19SceneWork)->selection < 3) {
                Ovl19ResetActorStage();
                ((Ovl19SelectionState *)gOvl19SceneWork)->state = 0;
            } else {
                ((Ovl19SelectionState *)gOvl19SceneWork)->state = 7;
            }
        } else {
            ((Ovl19CompletionCounterView *)D_8006C838)->completed = 0;
            ((Ovl19CompletionCounterView *)D_8006C838)->counter++;
        }
    }
}

void ovl_19_func_800B8DA8(void) {
    Ovl19DrawProjectedActorSprites();
    ((Ovl19CompletionCounterView *) D_8006C838)->completed = 1;
    ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
}

void ovl_19_func_800B8DE8(void) {
    s16 *temp_v1;

    Ovl19ResetActorStage();
    D_800BF4E0[0] = 0;
    temp_v1 = &D_800BF4E0[0x24];
    D_800BF4E0[1] = -0x64;
    D_800BF4E0[2] = 0xBE;
    D_800BF4E0[0x24] = 0;
    temp_v1[1] = -0x64;
    temp_v1[2] = -0xBE;
    Ovl19SetActorState((Ovl19Func800BAC40Arg *)&D_800BF4E0[0x40], 0, 0);
    Ovl19SetActorState((Ovl19Func800BAC40Arg *)&D_800BF4E0[0x60], 0, 0);
    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&D_800BF4E0[-8], 1, 1, 0);
    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&D_800BF4E0[0x1C], 1, 3, 0);
    D_800BF4E0[-0xD] = 1;
}

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo).
 * The two command records live at +0x10 and +0x58 (s16 index 8 and 0x2C);
 * the state halfword at +0x6 (index 3) and the display halfword at +0x202
 * (index 0x101) are written by this handler. */
/* POLICY EXCEPTION (user-approved 2026-07-31, see common.h): the target reads
 * hard $v0 between the func_8001FABC call and the following command call, so
 * the value stored at D_800BF4C0[0x101] is the most recent call's $v0 return.
 * CAPTURE_PREV_RET is the construct that reproduces that entry-liveness. */
void ovl_19_func_800B8E88(void) {
    s32 temp_s0;
    CAPTURE_PREV_RET(phantom);

    RequestMessageWindow(4, 9);
    temp_s0 = GetMessageWindowStatus();
    if (temp_s0 == 2) {
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
        PlaySoundEffect(0x3A);
        gOvl19SceneWork[0x101] = phantom;
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 2, 0, 0);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 1, 3, 0);
        gOvl19SceneWork[3] = temp_s0;
    }
}

/* State handler on the shared ovl_19 D_800BF4C0 s16 state array. The compiler
 * materialises the D_800BF560 label (D_800BF4C0 + 0xA0) and folds the +0xA0/
 * +0xE0 command records and the -0x90/-0x48 records as displacements from it. */
void ovl_19_func_800B8F38(void) {
    s32 n;

    n = gOvl19MessageSoundTicks + 1;
    gOvl19MessageSoundTicks = n;
    if (n % 30 == 0) {
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3A);
    }
    RequestMessageWindow(4, 0xA);
    if (GetMessageWindowStatus() == 2) {
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 2, 0x1E);
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 4, 0, 0x1E);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 7, 0, 0x5A);
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3B);
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3C);
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x44);
        gOvl19MessageSoundTicks = 0;
        gOvl19SceneWork[7] = 0;
        gOvl19SceneWork[3] = 3;
    }
}

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo).
 * The two command records live at +0x10 and +0x58 (s16 index 8 and 0x2C);
 * the +0xA0/+0xE0 records at s16 index 0x50/0x70; the state halfword at +0x6
 * (index 3) and the display halfword at +0xE (index 7) are written here. */

void ovl_19_func_800B9050(void) {
    RequestMessageWindow(4, 0xB);
    if (gOvl19SceneWork[9] == 4) {
        if (gOvl19SceneWork[0xB] < 2) {
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 6, 0, 0x3C);
        }
    }
    if (GetMessageWindowStatus() == 2) {
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 1, 0);
        Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 1, 0);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 3, 3, 0x1E);
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 1, 3, 0x1E);
        gOvl19SceneWork[7] = 0;
        gOvl19SceneWork[3] = 4;
    }
}

s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *arg0);

void ovl_19_func_800B9124(void) {
    s32 n;
    s32 state;

    n = gOvl19SceneTickCounter + 1;
    gOvl19SceneTickCounter = n;
    if (n % 30 == 0) {
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3A);
    }
    if (gOvl19SceneWork[7] == 0) {
        SetTextInputEnabled(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0xC);
        gOvl19SceneWork[7]++;
    }
    if (ovl_19_func_800BA65C((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8]) == 1) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 2, 3, 0x1E);
        state = GetMessageWindowStatus();
        if (state == 5) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 2, 0x1E);
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 4, 3, 0x1E);
            gOvl19SceneWork[0x101] = PlaySoundEffect(0x3B);
            gOvl19SceneWork[0x101] = PlaySoundEffect(0x44);
            CloseMessageWindow();
            gOvl19SceneTickCounter = 0;
            gOvl19SceneWork[7] = 0;
            gOvl19SceneWork[3] = state;
        }
    } else {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 3, 3, 6);
    }
}

/* State handler on the shared ovl_19 D_800BF4C0 s16 state array. The compiler
 * materialises the D_800BF560 label (D_800BF4C0 + 0xA0) and folds the +0xA0/
 * +0xE0 command records and the -0x90/-0x48 records as displacements from it. */
void ovl_19_func_800B9288(void) {
    SetTextInputEnabled(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 0xD);
    if (gOvl19SceneWork[9] == 4) {
        if (gOvl19SceneWork[0xB] < 2) {
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 5, 3, 0x3C);
        }
    }
    if (ovl_19_func_800BA25C() == 1) {
        if (GetMessageWindowStatus() == 5) {
            SetTextInputEnabled(1);
            SetMessagePortraitEnabled(1);
        }
        if (GetMessageWindowStatus() == 2) {
            Ovl19ResetActorStage();
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x50], 0, 0);
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x70], 0, 0);
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 1, 1, 0);
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 1, 3, 0);
            CloseMessageWindow();
            gOvl19SceneWork[7] = 0;
            gOvl19SceneWork[3] = 6;
        }
    }
}

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo). */

void ovl_19_func_800B93B0(void) {
    s32 temp_s0;
    s32 temp_v0;

    RequestMessageWindow(4, 0xE);
    temp_s0 = GetMessageWindowStatus();
    if (temp_s0 == 2) {
        temp_v0 = GetMessageWindowChoice();
        gOvl19SceneWork[3] = 0;
        if (temp_v0 == 1) {
            gOvl19SceneWork[0x100] = 0;
            gOvl19SceneWork[2] = 3;
        } else if (temp_v0 == temp_s0) {
            gOvl19SceneWork[2] = temp_v0;
        }
    }
}

void Ovl19DrawUiAnimation9(void);

void ovl_19_func_800B942C(void) {
    Ovl19DrawUiAnimation9();
    Ovl19DrawProjectedActorSprites();
}

s32 Ovl19IsActorInViewCone(Ovl19ActorMotionView *, Ovl19ActorMotionView *);
s32 Ovl19IsActorInForwardBand(s32 arg0, s32 arg1);

void ovl_19_func_800B9454(void) {
    s16 *s2;
    s16 *s1;
    s16 *s3;
    s16 *s6;
    s32 s4;
    s32 s5;
    s32 s0;
    s32 a0v;
    s32 v1v;

    s2 = D_800BF5A0;
    s3 = (s16 *)((u8 *)s2 - 0x88);
    s6 = (s16 *)((u8 *)s2 - 0xD0);

    if (s2[2] > 0) {
        *(u16 *)&s2[2] = *(u16 *)&s2[2] - 1;
        if ((s16)*(u16 *)&s2[2] <= 0) {
            s2[2] = 0;
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)s2, 1, 0);
        }
    } else {
        s1 = s2 - 0x70;
        if (s1[5] != s1[5] / s1[0xBD] * s1[0xBD]) {
            return;
        }
        s5 = Rand(0x80);
        s4 = 0;
        s0 = ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)s3);
        if (s0 == 1) {
            if (Ovl19IsActorInViewCone((Ovl19ActorMotionView *)s3, (Ovl19ActorMotionView *)s6) == s0) {
                a0v = s1[0xC5];
                v1v = s1[0xC4];
            } else if (Ovl19IsActorInForwardBand((s32)s3, (s32)s6) == s0) {
                a0v = s1[0xC3];
                v1v = s1[0xC2];
            } else {
                a0v = s1[0xC1];
                v1v = s1[0xC0];
            }
        } else {
            a0v = s1[0xBF];
            v1v = s1[0xBE];
        }
        if (s3[4] < 0x51) {
            if (v1v < s5) {
                s4 = 1;
            }
        } else {
            if (s5 < a0v) {
                s4 = 1;
            }
        }
        if (s4 != 0) {
            Ovl19SetActorState((Ovl19Func800BAC40Arg *)s2, 2, 0x1E);
        }
    }
}

s32 ovl_19_func_800BA2D4(void);
s32 ovl_19_func_800BA33C(u8 *arg0);

/* Record view from the matched callee ovl_19_func_800BA750. */
typedef struct {
    char pad_0[0x2];
    u16 unk2;
    char pad_4[0x2];
    s16 unk6;
} Recon800BA750A0View;

s32 Ovl19IsActorInViewCone(Ovl19ActorMotionView *arg0, Ovl19ActorMotionView *arg1);
s32 Ovl19ClassifyRelativeDirection(Ovl19ActorMotionView *arg0, Ovl19ActorMotionView *arg1);
s32 ovl_19_func_800BA750(Recon800BA750A0View *arg0);

/* One 0x48-byte actor record in the D_800BF4D0 pair. */
typedef struct {
    s16 unk0;
    s16 command;
    s16 angle;
    s16 timer;
    s16 value;
    s16 unkA;
    s32 flag;
    s16 rest[28];
} Ovl19ActorRecord;

/* Per-frame command driver for the two actors. For each actor, it issues the
 * scripted command 4, resolves a command-4 encounter with the other actor
 * (commands 5-10), or rolls a fresh command and direction (800BA33C). */
void Ovl19UpdateActorCommands(void) {
    s32 i;
    s16 angle;
    s16 *actors;
    /* User-authorized binding; see notes/asm-exception-evidence.md. Left to
     * global-alloc, this pointer outranks both angle and the strength-reduced
     * actor pointer, which rotates $s1-$s3. */
    register s16 *opponent asm("$19");
    s16 *state;
    s16 *record;
    s16 *view;
    s32 choice;
    s32 status;
    s32 command;
    s32 one;

    i = 0;
    one = 1;
    actors = gOvl19SceneWork + 8;
    view = actors - 8;
    do {
        Ovl19ActorRecord *actor = (Ovl19ActorRecord *)actors + i;

        angle = 0;
        opponent = actors;
        if (i == 0) {
            opponent += 36;
        }
        status = ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)actor);
        record = actors - 8;
        if (status == one && record[i * 32 + 0x51] == 2) {
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)actor, 4, actor->angle, 30);
            record[0x101] = PlaySoundEffect(0x3B);
            record[0x101] = PlaySoundEffect(0x44);
            continue;
        }
        if (actor->command == 4) {
            state = gOvl19SceneWork;
            if (actor->timer != actor->timer / state[0xBB] * state[0xBB]) {
                continue;
            }
            if (actor->timer > 0) {
                if (opponent[1] == 4 &&
                    Ovl19IsActorInViewCone((Ovl19ActorMotionView *)opponent, (Ovl19ActorMotionView *)actor) == one &&
                    Ovl19IsActorInViewCone((Ovl19ActorMotionView *)actor, (Ovl19ActorMotionView *)opponent) == one) {
                    choice = ovl_19_func_800BA2D4();
                    record = (s16 *)(choice * 72 + (s32)actors);
                    opponent = actors;
                    if (choice == 0) {
                        opponent = state + 0x2C;
                    }
                    angle = (s16)(record[2] + Ovl19ClassifyRelativeDirection((Ovl19ActorMotionView *)record, (Ovl19ActorMotionView *)opponent));
                    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 9, (s16)(angle % 4), state[0xC8]);
                    *(s32 *)(record + 6) = one;
                    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)opponent, 10, opponent[2], state[0xC9]);
                    *(s32 *)(opponent + 6) = 0;
                    continue;
                }
            } else {
                if (Ovl19IsActorInViewCone((Ovl19ActorMotionView *)actor, (Ovl19ActorMotionView *)opponent) == one) {
                    angle = (s16)(actor->angle + Ovl19ClassifyRelativeDirection((Ovl19ActorMotionView *)actor, (Ovl19ActorMotionView *)opponent));
                    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)actor, 6, (s16)(angle % 4), state[0xC7]);
                    actor->flag = one;
                    command = 7;
                    if (ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)opponent) == one) {
                        command = 8;
                    }
                    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)opponent, command, opponent[2], state[0xC7]);
                    state[0x101] = PlaySoundEffect(0x3C);
                } else {
                    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)actor, 5, actor->angle, state[0xC6]);
                    actor->flag = 0;
                }
                continue;
            }
        }
        if (ovl_19_func_800BA750((Recon800BA750A0View *)actor) == one) {
            command = ovl_19_func_800BA33C((u8 *)actor);
            switch (command) {
                case 0:
                case 1:
                    angle = (s16)Rand(4);
                    break;
                case 3:
                    angle = view[10];
                    break;
                case 2:
                    if ((s32)Rand(101) <= actor->value * 100 / 255) {
                        angle = (s16)(view[10] + Ovl19ClassifyRelativeDirection((Ovl19ActorMotionView *)actor, (Ovl19ActorMotionView *)opponent));
                        angle = (s16)(angle % 4);
                    } else {
                        angle = actor->angle;
                    }
                    break;
            }
            Ovl19SetActorControl((Ovl19Func800BAC40Arg *)actor, command, angle, gOvl19SceneWork[0xBC]);
        }
    } while (view += 36, ++i < 2);
}

/*
 * Two-slot actor behavior states, original ovl_19:800B998C-800BA25C.
 * The dispatcher owns states 3..11 and their position stepper. State changes
 * go through actor_controls; shared scene work and the public control API
 * remain outside this unit. Original signed/unsigned record views are kept.
 */
void Ovl19UpdateDirectionChoice(s16 *record);
void Ovl19ActorState4NoOp(s16 *record);
s32 Ovl19StepUntilCountdownEnds(Ovl19ActorCountdownView *record);
void Ovl19ActorState6Step(Ovl19ActorMotionView *record);
void Ovl19ActorState7Step(Ovl19ActorMotionView *record);
void Ovl19ActorState8Step(Ovl19ActorMotionView *record);
void Ovl19StepAfterDoubleThreshold(Ovl19ActorMotionView *record);
void Ovl19StepWithTimedSound(Ovl19ActorMotionView *record);
void Ovl19StepWhileValueBelowLimit(Ovl19ActorLimitView *record);
void Ovl19StepActorPosition(Ovl19ActorMotionView *record);

s32 Ovl19ClassifyRelativeDirection(Ovl19ActorMotionView *record, Ovl19ActorMotionView *other);
s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *record);

void Ovl19UpdateActorBehaviors(void) {
    s16 *record;
    s16 state;
    s32 slot;

    for (slot = 0; slot < 2; slot++) {
        record = (s16 *) ((u8 *) gOvl19ActorPairState + (slot * 0x48));
        state = *(s16 *) ((u8 *) record + 2);
        switch (state) {
        case 0:
        case 1:
        case 2:
            break;
        case 3:
            Ovl19UpdateDirectionChoice(record);
            break;
        case 4:
            Ovl19ActorState4NoOp(record);
            break;
        case 5:
            Ovl19StepUntilCountdownEnds((Ovl19ActorCountdownView *) record);
            break;
        case 6:
            Ovl19ActorState6Step((Ovl19ActorMotionView *) record);
            break;
        case 7:
            Ovl19ActorState7Step((Ovl19ActorMotionView *) record);
            break;
        case 8:
            Ovl19ActorState8Step((Ovl19ActorMotionView *) record);
            break;
        case 9:
            Ovl19StepAfterDoubleThreshold((Ovl19ActorMotionView *) record);
            break;
        case 10:
            Ovl19StepWithTimedSound((Ovl19ActorMotionView *) record);
            break;
        case 11:
            Ovl19StepWhileValueBelowLimit((Ovl19ActorLimitView *) record);
            break;
        }
        /* Handlers see the pre-decrement countdown, even after a transition. */
        if ((*(s16 *) ((u8 *) record + 6)) > 0) {
            (*(s16 *) ((u8 *) record + 6))--;
        }
    }
}

void Ovl19UpdateDirectionChoice(s16 *record) {
    s16 *other;
    s16 *base;
    s16 *work;
    s16 sum;
    s16 limit;
    s16 direction;
    s32 band;
    s32 roll;
    s16 flag;
    s32 i;

    other = gOvl19SceneWork + 8;
    base = other;
    if (record == other) {
        other = record + 0x24;
    }
    work = base - 8;
    if ((record[3] == (record[3] / work[0xCA]) * work[0xCA]) || (record[3] == work[0xBC])) {
        roll = (s32)Rand(100);
        flag = record[4] * 100 / 255 < roll;
        band = Ovl19ClassifyRelativeDirection((Ovl19ActorMotionView *)record, (Ovl19ActorMotionView *)other);
        sum = 0;
        limit = (s16)Rand(100);
        for (i = 0; i < 4; i++) {
            sum = (s16)(sum + gOvl19DirectionWeights[i + band * 4 + flag * 16]);
            if (sum >= limit) {
                direction = (s16)(record[2] + i);
                direction %= 4;
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, record[1], direction, record[3]);
                break;
            }
        }
    }
    if (record[3] == (record[3] / 2) * 2) {
        if (Ovl19ClassifyRelativeDirection((Ovl19ActorMotionView *)record, (Ovl19ActorMotionView *)other) == 0 &&
            Ovl19ActorPlanarDistance((Ovl19ActorPlanarPosition *)record,
                                     (Ovl19ActorPlanarPosition *)other) < 201) {
            if (record[4] >= 240 && Rand(128) >= 111U) {
                gOvl19SceneWork[0x101] = PlaySoundEffect(0x3B);
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 4, record[2], 30);
            } else {
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record[2], record[3]);
            }
        } else if (ovl_19_func_800BA65C((Ovl19Func800BAC40Arg *)record) == 1) {
            if ((s32)Rand(128) < record[4]) {
                direction = (s16)(record[2] + Ovl19ClassifyRelativeDirection((Ovl19ActorMotionView *)record, (Ovl19ActorMotionView *)other));
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, (s16)(direction % 4), record[3]);
            } else if (Rand(128) < 111U) {
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record[2], record[3]);
            }
        }
        if (record[1] == 3) {
            Ovl19StepActorPosition((Ovl19ActorMotionView *)record);
        }
    }
}

/* State 4 deliberately does nothing; its countdown still ticks in the driver. */
void Ovl19ActorState4NoOp(s16 *record) {
}

/* The dispatcher ignores this helper's result, including its zero-count path. */
s32 Ovl19StepUntilCountdownEnds(Ovl19ActorCountdownView *record) {
    Ovl19StepActorPosition((Ovl19ActorMotionView *)record);
    if (record->countdown == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record->direction, 0);
    } else {
        return record->countdown;
    }
}

/* Three distinct original entry points share this exact countdown policy.
 * Keep them separate: the dispatcher and jump table preserve their identities. */
void Ovl19ActorState6Step(Ovl19ActorMotionView *record) {
    if ((gOvl19StepStartTicks - 0x1E) >= (*(s16 *) ((u8 *) record + 6))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19ActorState7Step(Ovl19ActorMotionView *record) {
    if ((gOvl19StepStartTicks - 0x1E) >= (*(s16 *) ((u8 *) record + 6))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19ActorState8Step(Ovl19ActorMotionView *record) {
    if ((gOvl19StepStartTicks - 0x1E) >= (*(s16 *) ((u8 *) record + 6))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19StepAfterDoubleThreshold(Ovl19ActorMotionView *record) {
    s16 countdown;

    countdown = *(s16 *) ((u8 *) record + 6);
    if ((countdown < (gOvl19DoubleThresholdTicks - 0x1E)) && (countdown < (gOvl19DoubleThresholdTicks - 0x3C))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19StepWithTimedSound(Ovl19ActorMotionView *record) {
    s16 countdown;

    countdown = *(s16 *) ((u8 *) record + 6);
    if ((countdown < (gOvl19SceneWork[0xC9] - 0x1E)) && (countdown < (gOvl19SceneWork[0xC9] - 0x2D))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == (gOvl19SceneWork[0xC9] - 0x1E)) {
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3C);
    }
}

void Ovl19StepWhileValueBelowLimit(Ovl19ActorLimitView *record) {
    if (record->value12 < -10) {
        Ovl19StepActorPosition((Ovl19ActorMotionView *)record);
    } else {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record->direction, record->countdown);
    }
}

/* Scale the base rate for the state, choose one of four planar directions,
 * and store the wrapped low halfwords. There is no motion for other directions. */
void Ovl19StepActorPosition(Ovl19ActorMotionView *record) {
    s16 stateIndex;
    s16 direction;
    s32 delta0;
    s32 step;
    s32 delta1;

    stateIndex = record->state - 3;
    step = 0;
    switch (stateIndex) {
    case 2:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCB] / 10;
        break;
    case 4:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] * gOvl19SceneWork[0xCD] / 100;
        break;
    case 5:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] * gOvl19SceneWork[0xCE] / 100;
        break;
    case 3:
    case 6:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] / 10;
        break;
    case 7:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] * gOvl19SceneWork[0xCF] / 100;
        break;
    case 0:
    case 8:
        step = gOvl19BaseActorStep;
        break;
    }
    delta1 = 0;
    direction = record->direction;
    delta0 = 0;
    switch (direction) {
    case 0:
        delta1 = -step;
        break;
    case 1:
        delta0 = -step;
        break;
    case 2:
        delta1 = step;
        break;
    case 3:
        delta0 = step;
        break;
    }
    record->position0 = (u16) record->position0 + delta0;
    record->position1 = (u16) record->position1 + delta1;
}

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo).
 * Two records live at this base + 0x10 and + 0x58; the fields read are the
 * s16 at +0x12 and the two s16 command arguments at +0x04 and +0x06. */

s32 ovl_19_func_800BA25C(void) {
    if (gOvl19SceneWork[0x35] >= -0x5A) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[0x2C], 0xB, gOvl19SceneWork[0x2E], 2);
        return 0;
    }
    if (gOvl19SceneWork[0x11] < -0x5A) {
        return -1;
    }
    Ovl19SetActorControl((Ovl19Func800BAC40Arg *)&gOvl19SceneWork[8], 0xB, gOvl19SceneWork[0xA], 2);
    return 1;
}

s32 ovl_19_func_800BA2D4(void) {
    s32 var_s0;

    if (gOvl19ActorPairState[4] != gOvl19ActorPairState[0x28]) {
        var_s0 = gOvl19ActorPairState[4] <= gOvl19ActorPairState[0x28];
        if (Rand(100) >= 0x5B) {
            var_s0 = !var_s0;
        }
    } else {
        var_s0 = Rand(2) != 0;
    }
    return var_s0;
}

s32 ovl_19_func_800BA33C(u8 *arg0) {
    s32 result;
    s32 flag;
    s32 cat;
    s32 i;
    s16 sum;
    s16 limit;
    s16 type;
    s32 val;

    type = *(s16 *)(arg0 + 8);
    if (type < 0x51) {
        cat = 0;
    } else if (type < 0xF1) {
        cat = 1;
    } else {
        cat = 2;
    }
    result = 0;
    flag = *(u32 *)(arg0 + 0xC) != 0;
    sum = 0;
    limit = (s16)Rand(0x64);
    for (i = 0; i < 4; i++) {
        val = *(u16 *)((u8 *)D_800BCF28 + (cat << 4) + (flag << 3) + i * 2);
        sum = (s16)(sum + val);
        if (limit < sum) {
            switch (i) {
                case 0: result = 3; break;
                case 1: result = 1; break;
                case 2: result = 0; break;
                case 3: result = 2; break;
            }
            break;
        }
    }
    return result;
}

s32 Ovl19ActorBearing(Ovl19ActorMotionView *arg0, Ovl19ActorMotionView *arg1);

s32 Ovl19IsActorInViewCone(Ovl19ActorMotionView *arg0, Ovl19ActorMotionView *arg1) {
    s32 var_a2;
    s32 temp;
    s16 *base;

    temp = Ovl19ActorPlanarDistance((Ovl19ActorPlanarPosition *)arg0,
                                   (Ovl19ActorPlanarPosition *)arg1);
    base = gOvl19SceneWork;
    if (base[0xBA] < temp) {
        return 0;
    }
    var_a2 = Ovl19ActorBearing(arg0, arg1) - (arg0->direction << 0xA);
    if (var_a2 < 0) {
        var_a2 += 0x1000;
    }
    if ((((base[0xB9] << 0xC) / 360) < var_a2) && (var_a2 < (((0x168 - base[0xB9]) << 0xC) / 360))) {
        return 0;
    }
    return 1;
}

s32 Ovl19IsActorInForwardBand(s32 arg0, s32 arg1) {
    s32 callRet1;

    /* The original passes both incoming argument registers through. */
    callRet1 = Ovl19ClassifyRelativeDirection((Ovl19ActorMotionView *)arg0,
                                  (Ovl19ActorMotionView *)arg1);
    return callRet1 == 0;
}

/* Planar distance utility, original ovl_19:0x800BA564. */
s32 Ovl19ActorPlanarDistance(Ovl19ActorPlanarPosition *left,
                             Ovl19ActorPlanarPosition *right) {
    return SquareRoot0((left->position0 - right->position0) *
                       (left->position0 - right->position0) +
                       (left->position1 - right->position1) *
                       (left->position1 - right->position1));
}

s32 Ovl19ClassifyRelativeDirection(Ovl19ActorMotionView *arg0, Ovl19ActorMotionView *arg1) {
    s32 t;

    t = Ovl19ActorBearing(arg0, arg1) - (arg0->direction << 10);
    if (t < 0) {
        t += 0x1000;
    }
    if ((u32)(t - 0x201) < 0x400) {
        return 1;
    }
    if ((u32)(t - 0x60C) < 0x3F5) {
        return 2;
    }
    if ((u32)(t - 0xA0C) < 0x3F5) {
        return 3;
    }
    return 0;
}

/* Both geometry consumers use the angle returned by the SDK call. */
s32 Ovl19ActorBearing(Ovl19ActorMotionView *arg0, Ovl19ActorMotionView *arg1) {
    return ratan2(arg0->position0 - arg1->position0,
                  arg0->position1 - arg1->position1);
}

s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *arg0) {
    SVECTOR pos;

    pos = *(SVECTOR *) ((u8 *) arg0 + 0x10);
    switch (arg0->parameter) {
    case 0:
        pos.vz -= 200;
        break;
    case 1:
        pos.vx -= 200;
        break;
    case 2:
        pos.vz += 200;
        break;
    case 3:
        pos.vx += 200;
        break;
    }
    Ovl19AdjustPositionToSurface((s16 *) &pos);
    return pos.vy != *(s16 *) ((u8 *) arg0 + 0x12);
}

s32 ovl_19_func_800BA73C(Ovl19Func800BA73CArg *arg0) {
    return ((u32)(arg0->unk2 - 2)) < ((u32)2);
}

s32 ovl_19_func_800BA750(Recon800BA750A0View *arg0) {
    s32 var;
    u16 a;
    a = arg0->unk2;
    if (arg0->unk6 > 0) {
        var = 0;
    } else {
        var = a < 4;
    }
    return var;
}

/*
 * Projected actor-render hierarchy: paired actors, auxiliary records and the
 * fixed scene sprites. The two leaf drawers have no other code clients.
 * They select animations from raw state codes, not inferred gameplay states.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */
void Ovl19DrawActorPairSprite(s16 *actor);
void Ovl19DrawAuxRecordSprite(Ovl19Func800BAB4CArg *record);

void Ovl19DrawProjectedActorSprites(void) {
    s32 actorIndex;
    s32 auxIndex;
    s32 fixedIndex;

    for (actorIndex = 0; actorIndex < 2; actorIndex++) {
        Ovl19DrawActorPairSprite(&gOvl19ActorPairState[actorIndex * 0x24]);
    }
    for (auxIndex = 0; auxIndex < 3; auxIndex++) {
        Ovl19DrawAuxRecordSprite(&((Ovl19Func800BAB4CArg *) gOvl19AuxRecordBase)[auxIndex]);
    }
    for (fixedIndex = 0; fixedIndex < 9; fixedIndex++) {
        /* Each position has its sprite-state halfword four bytes before it. */
        Ovl19DrawProjectedSpriteState(gActiveRenderContext->field_120, (ObjectState *) &gOvl19AuxSceneSpriteSource,
                                     &gOvl19FixedSpritePositions[fixedIndex * 6], gOvl19FixedSpritePositions[fixedIndex * 6 - 2]);
    }
}

void Ovl19DrawActorPairSprite(s16 *actor) {
    SVECTOR extraPosition;
    s16 *sceneWork;
    s16 *sprite;
    s16 *otherActor;
    s16 textureMode;
    s16 state;
    s16 state9Ticks;
    s16 state10Ticks;
    s16 animation;
    s16 projectionOffsetY;
    s16 frame;
    s16 drawExtraSprite;
    u16 otherVariant;

    otherActor = gOvl19ActorPairState;
    if (actor == gOvl19ActorPairState) {
        otherActor = actor + 0x24;
    }
    projectionOffsetY = 0;
    textureMode = *actor;
    drawExtraSprite = 0;
    if (textureMode == 0) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) (actor + 0xC), 0, 0, 0x20, 1);
        animation = 0;
    } else {
        if (textureMode == 1) {
            SetSpriteSourceTextureOffsets((Struct_800154CC *) (actor + 0xC), 0, 0, *(s16 *) ((u8 *) actor + 0xA), 0);
        }
        animation = 0;
    }
    state = *(s16 *) ((u8 *) actor + 2);
    frame = -1;
    switch (state) {
    case 0:
        animation = (*(u16 *) ((u8 *) actor + 4)) + 5;
        break;
    case 1:
        animation = (*(u16 *) ((u8 *) actor + 4)) + 9;
        break;
    case 2:
        frame = actor[2];
        animation = 0;
        break;
    case 4:
        projectionOffsetY = -0xA;
        animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        break;
    case 6:
        if ((gOvl19StepStartTicks - 0x1E) < actor[3]) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else {
            animation = (*(u16 *) ((u8 *) actor + 4)) + 1;
        }
        break;
    case 7:
    case 8:
        if ((gOvl19StepStartTicks - 0x1E) >= actor[3]) {
            otherVariant = *(u16 *) ((u8 *) otherActor + 4);
            animation = otherVariant + 1;
            actor[2] = otherVariant;
        } else {
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0xD;
        }
        drawExtraSprite = 1;
        break;
    case 9:
        state9Ticks = actor[3];
        if (state9Ticks >= (gOvl19DoubleThresholdTicks - 0x1E)) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else if (state9Ticks >= (gOvl19DoubleThresholdTicks - 0x3C)) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else {
            animation = (*(u16 *) ((u8 *) actor + 4)) + 1;
        }
        break;
    case 10:
        state10Ticks = actor[3];
        if (state10Ticks >= (gOvl19State10ThresholdTicks - 0x1E)) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else {
            if (state10Ticks >= (gOvl19State10ThresholdTicks - 0x2D)) {
                drawExtraSprite = 1;
                animation = (*(u16 *) ((u8 *) otherActor + 4)) + 0xD;
            } else {
                drawExtraSprite = 1;
                animation = (*(u16 *) ((u8 *) otherActor + 4)) + 1;
            }
            actor[2] = otherActor[2];
        }
        break;
    case 3:
    case 5:
    case 11:
        animation = (*(u16 *) ((u8 *) actor + 4)) + 1;
        break;
    }
    sprite = actor + 0xC;
    if ((*(u8 *) ((u8 *) actor + 0x1C)) != animation) {
        SelectSpriteAnimation((ObjectState *) sprite, animation & 0xFF);
    }
    if (frame != -1) {
        SelectSpriteAnimationFrame((ObjectState *) sprite, frame & 0xFF);
    }
    Ovl19DrawProjectedSprite(gActiveRenderContext->field_120, (SpriteSourceData *) sprite, (actor + 8), projectionOffsetY);
    if (drawExtraSprite == 1) {
        sceneWork = gOvl19SceneWork;
        if ((*(u8 *) ((u8 *) sceneWork + 0x1D4)) != 0x24) {
            SelectSpriteAnimation((ObjectState *) (sceneWork + 0xE8), 0x24);
        }
        extraPosition = *(SVECTOR *) (actor + 8);
        Ovl19DrawProjectedSprite(gActiveRenderContext->field_D8 + 8, (SpriteSourceData *) (sceneWork + 0xE8), (s16 *) &extraPosition, -0x18);
    }
}

void Ovl19DrawAuxRecordSprite(Ovl19Func800BAB4CArg *record) {
    ObjectState *sprite;
    s16 state;
    s32 animation;
    s32 frame;

    if (record->unk0 == 1) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) &record->sprite, 0, 0, 0, record->unk6);
    }
    animation = 0;
    frame = -1;
    state = record->unk2;
    switch (state) {
    case 0:
        animation = 0;
        break;
    case 1:
        animation = 1;
        break;
    case 3:
        animation = 3;
        break;
    case 2:
    case 4:
        animation = 2;
        break;
    }
    sprite = (ObjectState *) &record->sprite;
    if (record->sprite.field_4 != animation) {
        SelectSpriteAnimation(sprite, animation);
    }
    if (frame != -1) {
        SelectSpriteAnimationFrame(sprite, frame & 0xFF);
    }
    Ovl19DrawProjectedSprite(gActiveRenderContext->field_120, (SpriteSourceData *) sprite, record->position, 0);
}

/* Actor state/pause controls (ovl_19:800BAC40–800BAD50).
 * The two state writers serve different actor record formats through their
 * shared eight-byte prefix. The pause service changes only sprite flag 4;
 * it does not stop the scene's logic, sound or controller polling.
 * Interface: ovl19_actor_controls.h. Grouping: notes/file-groupings.md. */
void Ovl19SetActorControl(Ovl19Func800BAC40Arg *control, s32 state, s32 parameter, s32 extra) {
    control->state = state;
    control->parameter = parameter;
    control->extra = extra;
}

void Ovl19SetActorState(Ovl19Func800BAC40Arg *control, s32 state, s32 parameter) {
    control->state = state;
    control->parameter = parameter;
}

void Ovl19PauseActorSprites(void) {
    Ovl19SetActorSpritePauseMode(0);
}

void Ovl19ResumeActorSprites(void) {
    Ovl19SetActorSpritePauseMode(1);
}

void Ovl19SetActorSpritePauseMode(s32 resume) {
    u8 *sprite;
    s32 remaining;

    /* Install the operation before walking the groups and read it anew for
     * each sprite. The word view retains the original callback-slot access. */
    *(u32 *)&gOvl19ActorSpriteFlagOperation = (u32)(resume == 0 ? AddSpriteSourceFlags : ClearSpriteSourceFlags);

    sprite = gOvl19ActorSprites;
    for (remaining = 2; remaining >= 0; remaining--) {
        ((void (*)(u16 *, s32))*(u32 *)&gOvl19ActorSpriteFlagOperation)((u16 *)sprite, 4);
        sprite += 0x40;
    }
    sprite = gOvl19AuxActorSprites;
    for (remaining = 1; remaining >= 0; remaining--) {
        ((void (*)(u16 *, s32))*(u32 *)&gOvl19ActorSpriteFlagOperation)((u16 *)sprite, 4);
        sprite += 0x48;
    }
}

void Ovl19DrawUiAnimation9(void) {
    u8 *base;

    SelectSpriteAnimation((ObjectState *)&gOvl19UiSprite, 9);
    base = gOvl19UiSprite - 0x1A0;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl19UiSprite, base[0x1A4],
                  base[0x1A5], 0, 0);
}

s32 Ovl19GetValueLabelOffset(s16 arg0) {
    s16 idx;

    if (arg0 >= 0) {
        idx = (arg0 < 20) ? arg0 : 19;
    } else {
        idx = 0;
    }
    return gOvl19ValueLabelOffsets[idx];
}

void Ovl19DrawProjectedSprite(s32 orderingTable, SpriteSourceData *sprite,
                              s16 *position, s16 screenYOffset) {
    s32 activeOrderingTable;
    s32 depth;
    s32 nearOffset;
    s32 offset;
    s32 spriteBucket;
    s32 companionBucket;

    Ovl19AdjustPositionToSurface(position);
    gOvl19ProjectedSpriteVector[0] = position[0];
    gOvl19ProjectedSpriteVector[1] = position[1];
    gOvl19ProjectedSpriteVector[2] = position[2];
    depth = ProjectPointToScreen(gOvl19ProjectedSpriteScreen, (SVECTOR *) gOvl19ProjectedSpriteVector);
    gOvl19ProjectedSpriteScreen[1] += screenYOffset;
    offset = (depth >> 3) * 4;
    activeOrderingTable = gActiveRenderContext->field_120;
    spriteBucket = orderingTable + offset;
    companionBucket = activeOrderingTable + offset;
    if ((sprite == (SpriteSourceData *) gOvl19ActorSprites) ||
        (sprite == (SpriteSourceData *) (gOvl19ActorSprites + 0x40)) ||
        (sprite == (SpriteSourceData *) (gOvl19ActorSprites + 0x80))) {
        DrawSpriteAnimationQuadsAndUpdate(spriteBucket, sprite, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1]);
        DrawUiSprite18Frame(companionBucket, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1], 0);
        return;
    }
    /* The +0x120 source bypasses depth rebucketing and the companion. */
    if (sprite == (SpriteSourceData *) (gOvl19ActorSprites + 0x120)) {
        DrawSpriteAnimationQuadsAndUpdate(orderingTable, sprite, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1]);
        return;
    }
    if ((position[1] != -0x64) && ((position[2] - position[0]) >= 0)) {
        nearOffset = (depth >> 2) * 4;
        spriteBucket = orderingTable + nearOffset;
        companionBucket = activeOrderingTable + nearOffset;
    }
    DrawSpriteAnimationQuadsAndUpdate(spriteBucket, sprite, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1]);
    /* General sprites keep the companion at unshifted screen Y; the three
     * actor sources above instead apply screenYOffset to both submissions. */
    DrawUiSprite18Frame(companionBucket, (s16) gOvl19ProjectedSpriteScreen[0], (s16) (gOvl19ProjectedSpriteScreen[1] - screenYOffset), 0);
}

void Ovl19DrawProjectedSpriteState(s32 orderingTable, ObjectState *sprite,
                                   s16 *position, s16 animation) {
    s32 depth;
    s32 offset;

    Ovl19AdjustPositionToSurface(position);
    gOvl19ProjectedStateVector[0] = position[0];
    gOvl19ProjectedStateVector[1] = position[1];
    gOvl19ProjectedStateVector[2] = position[2];
    depth = ProjectPointToScreen(gOvl19ProjectedStateScreen, (SVECTOR *) gOvl19ProjectedStateVector);
    SelectSpriteAnimation(sprite, animation & 0xFF);
    offset = (depth >> 3) * 4;
    DrawSpriteAnimationQuadsAndUpdate(orderingTable + offset, (SpriteSourceData *) sprite,
                 (s16) gOvl19ProjectedStateScreen[0], (s16) gOvl19ProjectedStateScreen[1]);
    DrawUiSprite18Frame(gActiveRenderContext->field_120 + offset,
                 (s16) gOvl19ProjectedStateScreen[0], (s16) gOvl19ProjectedStateScreen[1], 0);
}

/*
 * Overlay-19 two-column value labels and sprite markers.
 *
 * The sprite helper is genuinely nested: the original parent supplies GCC's
 * static chain before every call. Keep that definition inside the parent,
 * rather than emulating its incoming register in a separate file. Membership
 * and boundaries are documented in notes/file-groupings.md.
 */
s32 Ovl19GetValueLabelOffset(s16 value);

void Ovl19DrawValueTable(void) {
    void Ovl19DrawValueTableSpriteState(s32 animation, s32 frame) {
        u8 *display;

        SelectSpriteAnimation((ObjectState *)&gOvl19UiSprite, animation & 0xFF);
        SelectSpriteAnimationFrame((ObjectState *)&gOvl19UiSprite, frame & 0xFF);
        display = gOvl19UiSprite - 0x1A0;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x14, (s32)&gOvl19UiSprite,
                      display[0x1A4], display[0x1A5], 0, 0);
    }
    u32 savedTextSetting;
    u8 *sprite;
    u8 *display = gOvl19UiSprite;
    s32 displayAddress;
    u8 *values;
    s32 *firstLabel;
    u8 *labelText;
    s32 firstRowY;
    s32 rowYFixed;
    s32 nextColumn;
    s32 moreColumns;
    s32 textOffset;
    s32 textAddress;
    /* Existing authorized binding: keeps the initial zero below the sprite
     * draw and preserves loop/delay-slot allocation. Not original-source
     * evidence; see notes/asm-exception-evidence.md (ovl_19:800BB108). */
    register s32 row asm("$16");
    u8 *counts;
    s32 column;

    SelectSpriteAnimation((ObjectState *)display, 0);
    sprite = display;
    display -= 0x1A0;
    displayAddress = (s32)display;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x18, (s32)sprite, display[0x1A4], display[0x1A5], 0, 0);
    column = 0;
    row = 0;
    savedTextSetting = GetTextPalette();
    SetTextPalette(1);
    firstLabel = &gOvl19ValueTableFirstLabel;
    labelText = (u8 *)gOvl19ValueLabelText;
    do {
        nextColumn = column + 1;
        rowYFixed = 60 << 16;
        firstRowY = 57;
        values = (u8 *)(displayAddress + 0x160);
        do {
            if (column == 0 && row == 0) {
                textAddress = *firstLabel;
            } else {
                textOffset = Ovl19GetValueLabelOffset(((s16 *)values)[column * 4 + row]);
                textAddress = textOffset + (s32)labelText;
            }
            DrawTextLine(gActiveRenderContext->field_D8 + 0xC, textAddress, (s16)(column * 200 + 21),
                          row > 0 ? rowYFixed >> 16 : (s16)firstRowY);
            firstRowY += 45;
            rowYFixed += 45 << 16;
            row++;
        } while (row < 4);
        moreColumns = nextColumn < 2;
        column = nextColumn;
        row = 0;
    } while (moreColumns);
    SetTextPalette(savedTextSetting);
    counts = (u8 *)gOvl19SceneWork;
    if (((s16 *)counts)[4] > 0) {
        Ovl19DrawValueTableSpriteState(1, 0);
        Ovl19DrawValueTableSpriteState(4, 0);
        Ovl19DrawValueTableSpriteState(5, 0);
        Ovl19DrawValueTableSpriteState(7, 0);
    }
    if (((s16 *)counts)[4] >= 2) {
        Ovl19DrawValueTableSpriteState(1, 1);
        Ovl19DrawValueTableSpriteState(7, 1);
    }
}

/*
 * Overlay-19 scene resources, original 800BB318-800BBD14.
 * Sprite/archive and queued sound setup precede the camera/render lifecycle.
 * The geometry loader, camera-driven draw path and surface queries share
 * one descriptor; they now live together rather than in separate modules.
 * See ovl19_scene_resources.h and ovl19_scene_camera_geometry.h for sequencing,
 * state changes, units and loading prerequisites.
 */

/* Offsets from D_8007AFF0, not independent globals. The distance, height and
 * yaw slots are reset as words but consumed as low halfwords. Keep those
 * access widths: the signed/unsigned views deliberately preserve wraparound.
 * The words at +2539C/+253A4 are only reset here; their roles remain unknown. */
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

void Ovl19InitSceneCamera(void) {
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

void Ovl19UpdateSceneCameraOffset(void) {
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

void Ovl19ApplySceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X),
                       *(s16 *)(camera + CAMERA_EYE_Y),
                       *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z),
                       0);
    SetCameraTarget(*(s16 *)(camera + CAMERA_TARGET_X),
                    *(s16 *)(camera + CAMERA_TARGET_Y),
                    *(s16 *)(camera + CAMERA_TARGET_Z));
}

void Ovl19DrawSceneGeometry(void) {
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
    DrawCulledProjectedPrimitives((ProjectedPrimitiveSource *)&gOvl19SceneGeometry, &eye, &target);
}

void Ovl19AdjustPositionToSurface(s16 *position) {
    s32 query[4];

    query[0] = position[0];
    query[1] = position[1];
    query[2] = position[2];
    query[3] = position[3];
    SetSurfacePolygonRejectMask(0xC000);
    if (QuerySurfaceCells(gOvl19SceneGeometry, query)) {
        query[1] = GetSurfaceHitHeight();
    }
    position[0] = query[0];
    position[1] = query[1];
    position[2] = query[2];
    position[3] = query[3];
}

void Ovl19LoadSceneSpriteArchive(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl19SceneSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData(&gOvl19SceneSpriteSource, (SpriteDataHeader *)gOvl19SceneSpriteData);
}

void Ovl19UploadSceneSpriteChunk(void) {
    DrawSync(0);
    ClearOTagR(gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}

void Ovl19LoadUiAndActorSprites(void) {
    s32 i;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x52DC));
    *(Ovl19SpriteBlob *)gOvl19UiSpriteData = *(Ovl19SpriteBlob *)(gGraphicsHeapBase + 0x4290);
    InitSpriteSourceData((SpriteSourceData *)gOvl19UiSprite, (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[0]));
    for (i = 0; i < 3; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl19UiSprite + i * 0x40 - 0xF0), (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[i + 2]));
    }
    for (i = 0; i < 2; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl19AuxActorSprites + i * 0x48), (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[1]));
    }
    InitSpriteSourceData(&gOvl19AuxSceneSpriteSource, (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[5]));
}

/* Keep the descriptor address live across the copy. The existing compiler
 * barrier preserves publication after the complete asset-data transfer. */
void Ovl19LoadSceneGeometry(void) {
    s32 *discPositions;
    u8 *geometry;

    discPositions = gOvl19SceneDiscPositions;
    CdReadDiscSectorsBlocking(0, discPositions[0], discPositions[1] - discPositions[0], 0, D_8007AFF0);
    CdReadDiscSectorsBlocking(0, discPositions[1], discPositions[2] - discPositions[1], 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    memcpy(&gLoadedAreaHeaderCopy, (void *)D_8007AFF0, 0xBDC);
    geometry = &gOvl19SceneGeometry;
    __asm__("" : : : "memory");
    InitProjectedPrimitiveSource(geometry, (void *)(D_8007AFF0 + 0x2000));
    EnableSurfaceCells((void **)geometry);
    SetSurfaceHeightTolerance(0xC8);
}

void Ovl19QueueSceneSound(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0xF, 1);
}

/*
 * Raise one shared-record halfword by 20, capping valid byte-range values
 * at 255. The signed comparison and unsigned reload are intentional; this
 * is not a general-purpose clamp for negative inputs.
 * Membership and outer boundaries: notes/file-groupings.md.
 */

void Ovl19RaiseRecordByteRangeValue(s16 recordIndex) {
    char *work;
    char *record;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    /* Retain the shared-work base and the separate 0x8000 bias, as in the
     * original indexed addressing. */
    if (*(s16 *)(record + 0x8000 + 0x19EA) < 0xEB) {
        *(s16 *)(record + 0x8000 + 0x19EA) =
            (s16)(0x14 + *(u16 *)(record + 0x8000 + 0x19EA));
    } else {
        *(s16 *)(record + 0x8000 + 0x19EA) = 0xFF;
    }
}
