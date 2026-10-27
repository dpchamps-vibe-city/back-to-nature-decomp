/*
 * Stage-mode-2 record service: the single F0-byte slot, ID-dependent flags
 * and counters, entry/step state machines, movement and sprite presentation.
 * Entry callbacks can reject transitions; step callbacks own their keyframe
 * cursor and lock flag. The shared record and packet/sprite state make this
 * non-reentrant. Public contracts are in ovl11_stage_mode2_record.h.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_direction_vector.h"
#include "ovl11_timed_sprite_effect.h"
#include "psyq/memory.h"
#include "ovl11_area_resource_load.h"
#include "random.h"
#include "message_window.h"
#include "ovl11_cd_audio_request.h"
#include "ovl11_keyframe_step.h"
#include "ovl11_record_movement.h"
#include "ovl11_stage_mode2_record.h"
#include "ovl11_record_position_set.h"

s32 Ovl11GetStageMode2AnimationPair(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);

void Ovl11DrawStageMode2Record(u8 *record) {
    u16 animation;
    u16 frame;

    if (!(D_8006C844 & 0x800000) && *(s32 *)(record + 0x8C) != 0) {
        Ovl11GetStageMode2AnimationPair(*(u16 *)(record + 0x24),
                                       *(u16 *)(record + 0x22), &animation, &frame);
        if (animation != record[0x7C]) {
            SelectSpriteAnimation((ObjectState *)(record + 0x78), (u8)animation);
        }
        if (frame != 0 && frame != record[0x7D]) {
            SelectSpriteAnimationFrame((ObjectState *)(record + 0x78), (u8)frame);
        }
        SetSpriteAnimationUpdateLatch(0);
        Ovl11DrawObjectWithMarker(record);
    }
}

void Ovl11AccumulateStageMode2Direction(s32 recordAddress) {
    Ovl11AccumulateDirectionVector(recordAddress + 0x38, recordAddress + 0x48, 0);
}

s32 Ovl11InitStageMode2Record(s32 arg0, s32 arg1);

u16 *Ovl11AcquireStageMode2Record(s32 id) {
    if (((u32) (id - 0x15E) < 2U) && (gStageMode2Record == 0)) {
        Ovl11InitStageMode2Record((s32) &gStageMode2Record, id);
        return &gStageMode2Record;
    }
    return NULL;
}

s32 Ovl11RefreshStageMode2HourGate(Ovl11D04D4Obj *record) {
    if (record->unk0 == 0) {
        return -1;
    }
    if ((u32)(D_80070CF8_halfword - 6) >= 0xF) {
        record->unk2C = 0x12C;
        record->unk34 &= ~0x800;
    }
    return 0;
}

s32 Ovl11ApplyStageMode2IdFlags(Ovl11StageMode2RecordView *arg0);

/* Each stage stores through the unsigned halfword before a SIGNED
 * halfword limit test. Preserve that wrap/test order for unusual inputs. */
s32 Ovl11UpdateStageMode2Counters(Ovl11StageMode2RecordView *record) {
    s16 value;
    s32 flags;
    s32 amount;
    s32 flag2Value;
    s32 flag4Value;
    s32 decremented;
    s32 accumulated;
    s32 modeLoss;
    s32 flag2Clamped;
    s32 flag4Clamped;
    s32 decrementClamped;
    s32 accumulationClamped;
    s32 lossClamped;

    if (record->field_0 == 0) {
        return -1;
    }
    flags = record->field_34;
    if (flags & 0x400) {
        return -1;
    }
    if (flags & 2) {
        flag2Value = record->field_16 + 2;
        record->field_16 = flag2Value;
        if ((s16)flag2Value >= 0x100) {
            flag2Clamped = 0xFF;
        } else {
            flag2Clamped = flag2Value;
        }
        record->field_16 = flag2Clamped;
    }
    if (record->field_34 & 4) {
        flag4Value = record->field_16 + 2;
        record->field_16 = flag4Value;
        if ((s16)flag4Value >= 0x100) {
            flag4Clamped = 0xFF;
        } else {
            flag4Clamped = flag4Value;
        }
        record->field_16 = flag4Clamped;
    }
    if (!(record->field_34 & 0x26)) {
        decremented = record->field_16 - 1;
        record->field_16 = decremented;
        if ((s16)decremented < 0) {
            decrementClamped = 0;
        } else {
            decrementClamped = decremented;
        }
        record->field_16 = decrementClamped;
    }
    if (record->field_34 & 0x20) {
        value = (s16)record->field_16;
        amount = 1;
        if (value >= 0x64) {
            amount = 3;
            if (value < 0xC8) {
                amount = 2;
            }
        }
        accumulated = record->field_AC + amount;
        record->field_AC = accumulated;
        if ((s16)accumulated >= 0x100) {
            accumulationClamped = 0xFF;
        } else {
            accumulationClamped = accumulated;
        }
        record->field_AC = accumulationClamped;
    }
    if (record->field_30 == 1) {
        if (((struct Ovl11B264StateView *)D_8006C838)->field_E4C8 != 0) {
            modeLoss = record->field_16 - 5;
            record->field_16 = modeLoss;
            if ((s16)modeLoss < 0) {
                lossClamped = 0;
            } else {
                lossClamped = modeLoss;
            }
            record->field_16 = lossClamped;
        }
    }
    if ((record->field_1A != ((struct Ovl11B264StateView *)D_8006C838)->field_44B8) &&
        (record->field_1C == ((struct Ovl11B264StateView *)D_8006C838)->field_44BA)) {
        if (record->field_1E <= ((struct Ovl11B264StateView *)D_8006C838)->field_44BC) {
            record->field_0 = 0x15F;
            record->field_34 |= 0x8000;
            Ovl11ApplyStageMode2IdFlags(record);
        }
    }
    record->field_34 &= ~0x26;
    Ovl11ResetTimedSpriteEffect(record->field_A8);
    return 0;
}

s32 Ovl11PropagateStageMode2Flag8000(Ovl11StageMode2RecordView *record) {
    s32 flags;

    if (record->field_0 == 0) {
        return -1;
    }
    flags = record->field_34;
    if (flags & 0x8000) {
        record->field_34 = flags | 0x800000;
    }
    return 0;
}

s32 Ovl11ChooseStageMode2State(Ovl11D04D4Obj *arg0);

s32 Ovl11UpdateStageMode2State(u16 *record) {
    s32 (*step)(u16 *);

    if (*record == 0) {
        return -1;
    }
    step = (s32 (*)(u16 *))gOvl11StageMode2StepHandlers[*(u16 *)((u8 *)record + 0x26)];
    if (step != 0) {
        step(record);
    }
    if (!(*(s32 *)((u8 *)record + 0x34) & 0x800) &&
        *(s16 *)((u8 *)record + 0x2C) >= 300) {
        Ovl11ChooseStageMode2State((Ovl11D04D4Obj *)record);
        *(s16 *)((u8 *)record + 0x2C) = 0;
    }
    (*(s16 *)((u8 *)record + 0x2C))++;
    return 0;
}

s32 Ovl11ChangeStageMode2State(u16 *record, s32 state) {
    s32 nextState;
    s32 (*enter)(u16 *);
    s32 flags;
    s32 result;

    nextState = state;
    flags = *(s32 *)((u8 *)record + 0x34);
    result = 0;
    if (flags & 0x01000000) {
        nextState = 0x11;
    }
    if (flags & 0x400) {
        return -1;
    }
    enter = gOvl11StageMode2EntryHandlers[nextState];
    if (enter != 0) {
        result = enter(record);
    }
    if ((gOvl11StageMode2StepHandlers[nextState] != 0) && (result != -1)) {
        *(s16 *)((u8 *)record + 0x26) = nextState;
        *(s16 *)((u8 *)record + 0x28) = 0;
        *(s16 *)((u8 *)record + 0x2A) = 0;
        *(s16 *)((u8 *)record + 0x2C) = 0;
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *)(record + 0x24), 0);
    }
    return result;
}

void Ovl11ClearStageMode2Record(s32 recordAddress) {
    memset(recordAddress, 0, 0xF0);
    *(s16 *)recordAddress = 0;
}

void Ovl11RefreshStageMode2SpriteSource(void) {
    char *base;
    char *far0;
    char *far1;
    char *state;
    s32 var_a0;

    base = (char *)&gStageMode2Record;
    *(s32 *)(base + 0x34) |= 0x8000000;
    far0 = (char *)&D_8007AFF0;
    if ((*(s16 *)(far0 + 0x25476) != 3) && (*(s16 *)(far0 + 0x25476) != 1)) {
        *(s32 *)(base + 0x8C) = 0;
        return;
    }
    switch (*(u16 *)(base + 0)) {
    case 0x15E:
        var_a0 = 1;
        goto block_8;
    case 0x15F:
        var_a0 = 0;
block_8:
        Ovl11LoadSceneSpriteData(var_a0);
        InitSpriteSourceData((SpriteSourceData *)(base + 0x78), (SpriteDataHeader *)&D_8007AFF8);
        far1 = (char *)&D_8007AFF8;
        state = far1 + 0x1FFF8;
        if (*(s16 *)(state + 0x5476) == 3) {
            SetSpriteSourceTextureOffsets((Struct_800154CC *)(base + 0x78), -0x100, 0, 0, 0);
        }
        return;
    default:
        *(s32 *)(base + 0x34) &= 0xF7FFFFFF;
        *(s32 *)(base + 0x8C) = 0;
        break;
    }
}

/* Two unaligned s32 words copied as one 8-byte packed aggregate, as in
 * ovl_11_func_8010D04C (D_8006C838+0x44B8 -> object+0x1A). */
typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk1;
} Ovl11UnalignedWord8010B778;

s32 Ovl11InitStageMode2Record(s32 arg0, s32 arg1) {
    char *base;

    if (*(u16 *)arg0 != 0) {
        return -1;
    }
    memset((void *)arg0, 0, 0xF0);
    *(u16 *)arg0 = arg1;
    *(s16 *)(arg0 + 0x16) = 0xA;
    base = (char *)&D_8006C838;
    *(Ovl11UnalignedWord8010B778 *)(arg0 + 0x1A) = *(Ovl11UnalignedWord8010B778 *)(base + 0x44B8);
    *(s16 *)(arg0 + 0x30) = 3;
    *(s32 *)(arg0 + 0x38) = 0x1CC;
    *(s32 *)(arg0 + 0x3C) = 0;
    *(s32 *)(arg0 + 0x40) = 0x258;
    *(u16 *)(arg0 + 0x4) = 0xFFFF;
    Ovl11ApplyStageMode2IdFlags((Ovl11StageMode2RecordView *)arg0);
    Ovl11ResetTimedSpriteEffect((s16 *)(arg0 + 0xA8));
    return 0;
}

s32 Ovl11ApplyStageMode2IdFlags(Ovl11StageMode2RecordView *arg0) {
    switch (arg0->field_0) {
    case 0x15E:
        arg0->field_34 &= 0xFFFF7FFF;
        arg0->field_34 &= 0xFF7FFFFF;
        break;
    case 0x15F:
        arg0->field_34 |= 0x808000;
        break;
    }
    return 0;
}

/* Five animation modes, each with four direction variants. Frame zero
 * means retain the selected frame at the draw facade, not force frame zero. */
s32 Ovl11GetStageMode2AnimationPair(u16 mode, u16 direction,
                                   u16 *animation, u16 *frame) {
    *animation = gOvl11StageMode2AnimationPairs[mode][direction].animation;
    if (frame != 0) {
        *frame = gOvl11StageMode2AnimationPairs[mode][direction].frame;
    }
    return 0;
}

s32 Ovl11ChooseStageMode2State(Ovl11D04D4Obj *arg0) {
    s32 mode;
    s32 random;
    u16 current;

    if ((u32)(D_80070CF8_halfword - 6) < 15) {
        mode = 1;
        current = arg0->unk26;
        if (arg0->unk26 != 15) {
            if (arg0->unk34 & 0x2000) {
                random = Rand(7);
                mode = 17;
            } else if (arg0->unk34 & 0x8000) {
                random = Rand(2);
                mode = 18;
            } else {
                random = Rand(2);
                mode = 18;
            }
            switch (random) {
            case 0:
                break;
            case 1:
                mode = 2;
                break;
            }
            current = arg0->unk26;
        }
    } else {
        mode = 2;
        current = arg0->unk26;
    }
    if (mode != (s16)current) {
        Ovl11ChangeStageMode2State((u16 *)arg0, mode);
    }
    return 0;
}

s32 Ovl11TryStageMode2PositionRequest(Ovl11StageMode2PositionRequestView *record) {
    char *farWork;
    Ovl11B994PositionView *work;

    if (record->id == 0) {
        return -1;
    }
    farWork = (char *)&D_8007AFF0;
    if (*(s16 *)(farWork + 0x25476) != record->mode) {
        return -1;
    }
    if (record->flags & 0x400) {
        return -1;
    }
    if ((s32)Rand(0xC8) < record->requestValue + 0x64) {
        work = (Ovl11B994PositionView *)D_8006C838;
        Ovl11SetRecordPosition((Ovl11SetFieldsView *)record,
                            *(Ovl11PaddedVec3 *)work->position);
        record->flags |= 2;
        if (*(s16 *)(farWork + 0x25476) == record->mode) {
            Ovl11RequestCdAudioTrack(0xE);
        }
        Ovl11ChangeStageMode2State((u16 *)record, 0x11);
        return 0;
    }
    return -1;
}

s32 Ovl11StartStageMode2Effect31(Ovl11StageMode2RecordView *record) {
    s32 reduced;
    s32 clamped;
    char *farWork;

    if (record->field_0 == 0) {
        return -1;
    }
    reduced = record->field_16 - 10;
    record->field_16 = reduced;
    if ((s16)reduced < 0) {
        clamped = 0;
    } else {
        clamped = reduced;
    }
    record->field_16 = clamped;
    Ovl11StartTimedSpriteEffect((void *)((s32)record + 0xA8), 0x1F, 0x1E);
    farWork = (char *)&D_8007AFF0;
    if (*(s16 *)(farWork + 0x25476) == record->field_30) {
        Ovl11RequestCdAudioTrack(0xE);
    }
    return 0;
}

s32 Ovl11StartStageMode2Effect37(Ovl11StageMode2RecordView *record) {
    char *farWork;

    if (record->field_0 == 0) {
        return -1;
    }
    if (!(record->field_34 & 4)) {
        Ovl11StartTimedSpriteEffect((u8 *)record + 0xA8, 0x25, 0x2D);
    }
    record->field_34 |= 4;
    farWork = (char *)&D_8007AFF0;
    if (*(s16 *)(farWork + 0x25476) == record->field_30) {
        Ovl11RequestCdAudioTrack(0xD);
    }
    return -1;
}

s32 Ovl11RequestStageMode2Message(Ovl11StageMode2RecordView *record) {
    char *farWork;

    if (record->field_0 == 0) {
        return -1;
    }
    if (D_80075AEA < 9) {
        RequestMessageWindow(2, 0x258);
    } else if (D_80075AEA >= 0xC9) {
        RequestMessageWindow(2, 0x259);
    } else {
        RequestMessageWindow(2, 0x258);
    }
    farWork = (char *)&D_8007AFF0;
    if (*(s16 *)(farWork + 0x25476) == record->field_30) {
        Ovl11RequestCdAudioTrack(0xD);
    }
    return -1;
}

s32 Ovl11RequestStageMode2DirectionOffset(Ovl11SetFieldsView *arg0) {
    Ovl11PaddedVec3 sp18;
    u8 *base;
    u16 selector;

    base = (u8 *) D_8006C838;
    Ovl11SetDirectionVector(*(u16 *) (base + 0x5200), (Recon800D0408A1View *) &sp18, 0x320);
    sp18.vx = sp18.vx + *(s32 *) ((u8 *) arg0 + 0x38);
    sp18.vy = sp18.vy + *(s32 *) ((u8 *) arg0 + 0x3C);
    sp18.vz = sp18.vz + *(s32 *) ((u8 *) arg0 + 0x40);
    Ovl11SetRecordPosition(arg0, sp18);
    selector = *(u16 *) (base + 0x5200);
    *(u16 *) ((u8 *) arg0 + 0x22) = selector;
    return 0;
}

s32 Ovl11StartStageMode2Effect36(Ovl11StageMode2RecordView *record) {
    s32 reduced;
    s32 clamped;
    char *farWork;

    Ovl11StartTimedSpriteEffect((void *)((s32)record + 0xA8), 0x24, 0x2D);
    reduced = record->field_16 - 10;
    record->field_16 = reduced;
    if ((s16)reduced < 0) {
        clamped = 0;
    } else {
        clamped = reduced;
    }
    record->field_16 = clamped;
    farWork = (char *)&D_8007AFF0;
    if (*(s16 *)(farWork + 0x25476) == record->field_30) {
        Ovl11RequestCdAudioTrack(0xE);
    }
    return 0;
}

s32 Ovl11StepStageMode2KeyframesA(Ovl11D04D4Obj *record) {
    s16 cursor;
    s32 finished;

    cursor = record->unk28;
    finished = 0;
    switch (cursor) {
    case 0:
        record->unk28 = 1;
        record->unk34 |= 0x800;
        break;
    case -1:
        finished = 1;
        record->unk2C = 0x12C;
        record->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(record, gOvl11StageMode2KeyframesA,
                &record->unk28, -1, (u16 *)&record->unk2A) != 0) {
            record->unk28 = -1;
        }
        break;
    }
    return finished;
}

/* Unlike A/C, completion does not force the auto-selection timer to 300. */
s32 Ovl11StepStageMode2KeyframesB(Ovl11D04D4Obj *record) {
    s16 cursor;
    s32 finished;

    cursor = record->unk28;
    finished = 0;
    switch (cursor) {
    case 0:
        record->unk28 = 1;
        record->unk34 = record->unk34 | 0x800;
        break;
    case -1:
        finished = 1;
        record->unk34 = record->unk34 & ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(record, gOvl11StageMode2KeyframesB,
                &record->unk28, -1, (u16 *)&record->unk2A) != 0) {
            record->unk28 = -1;
        }
        break;
    }
    return finished;
}

s32 Ovl11StepStageMode2KeyframesC(Ovl11D04D4Obj *record) {
    s16 cursor;
    s32 finished;

    cursor = record->unk28;
    finished = 0;
    switch (cursor) {
    case 0:
        record->unk28 = 1;
        record->unk34 |= 0x800;
        break;
    case -1:
        finished = 1;
        record->unk2C = 0x12C;
        record->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(record, gOvl11StageMode2KeyframesC,
                &record->unk28, -1, (u16 *)&record->unk2A) != 0) {
            record->unk28 = -1;
        }
        break;
    }
    return finished;
}

s32 Ovl11StepStageMode2Movement(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1);

s32 Ovl11GateStageMode2Movement(Recon_ovl_11_func_8010BF8C_A0View *arg0) {
    arg0->unk34 = arg0->unk34 | 0x800;
    if (((arg0->unk34 | 0x800) & 0x2000) == 0) {
        arg0->unk34 = (arg0->unk34 | 0x800) & -0x801;
        arg0->unk2C = 0x12C;
        return 1;
    } else {
        arg0->unk2E = 0;
        Ovl11StepStageMode2Movement((Recon_ovl_11_func_800D2594_A0View *)arg0, (s32)arg0);
        return 0;
    }
}

s32 Ovl11StepStageMode2Movement(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1) {
    s32 var_a2;

    arg0->unk34 &= 0xFF7FFFFF;
    var_a2 = 0x8031;
    if ((*(s16 *) ((u8 *) arg0 + 0x26)) == 0xD) {
        var_a2 = 0x8030;
    }
    Ovl11StepRecordMovement(arg0, 0xC, var_a2);
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
    } else {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 1;
    }
    arg0->unk34 &= ~0x800;
    return 1;
}

s32 Ovl11StepStageMode2Movement24(Recon_ovl_11_func_800D2594_A0View *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_v1 = arg0->unk34;
    if (temp_v1 & 0x8000) {
        arg0->unk34 = temp_v1 | 0x800000;
    }
    Ovl11StepRecordMovement(arg0, 0x18, 0x8031);
    arg0->unk34 |= 0x800;
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
        if (!((*(u16 *) ((u8 *) arg0 + 0x7A)) & 0x300)) {
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 3;
        }
    } else {
        if (arg0->unk34 & 0x01000000) {
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 4;
        } else {
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 3;
        }
    }
    if ((*(u16 *) ((u8 *) arg0 + 0x7A)) & 0x300) {
        temp_v1_2 = arg0->unk34;
        if (!(temp_v1_2 & 0x01000000)) {
            arg0->unk34 = temp_v1_2 & ~0x800;
        }
    }
    temp_a0 = arg0->unk34;
    if (!(temp_a0 & 0x2000)) {
        (*(s16 *) ((u8 *) arg0 + 0x2C)) = 0x12C;
        if (((*(u16 *) ((u8 *) arg0 + 0x7A)) & 0x300) && !(temp_a0 & 0x01000000)) {
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        }
    }
    return 1;
}

void Ovl11CopyObjectEffectPositionWords(Ovl11EffectPositionWords *object) {
    char *work;

    /* Keep the two-stage base: these are far-work words, not a new global. */
    work = (char *)&D_8006C838;
    work += 0x8000;
    *(s32 *)(work + 0x12D4) = object->x;
    *(s32 *)(work + 0x12D8) = object->y;
    *(s32 *)(work + 0x12DC) = object->z;
    *(s32 *)(work + 0x12E0) = object->word10C;
}

void Ovl11SelectStageMode2AnimationFromObject(Ovl11C7010Object *arg0) {
    u8 *record;

    record = (u8 *) &gStageMode2Record;
    *(u16 *) (record + 0x22) = arg0->unk38;
    if (arg0->unk3C == 1) {
        SelectSpriteAnimation((ObjectState *) (record + 0x78), 0);
    }
    switch (arg0->unk3C) {
    case 2:
        SelectSpriteAnimation((ObjectState *) (record + 0x78), (record[0x22] + 1) & 0xFF);
        break;
    }
    if (arg0->unk3C == 4) {
        SelectSpriteAnimation((ObjectState *) (record + 0x78), (record[0x22] + 9) & 0xFF);
    }
    if (arg0->sprite.field_4 == (arg0->unk38 + 0x3F)) {
        SelectSpriteAnimation((ObjectState *) (record + 0x78), (record[0x22] + 0xD) & 0xFF);
        return;
    }
    if (GetSpriteFrameControlHighBit((SpriteSourceData *) (record + 0x78)) != 0) {
        record[0x7D] = record[0x22];
    }
}

void Ovl11DrawObjectAndResetTimedEffect(u8 *record) {
    if (!(D_8006C844 & 0x800000)) {
        Ovl11DrawObjectWithMarker(record);
        Ovl11ResetTimedSpriteEffect((s16 *)(record + 0xA8));
    }
}
