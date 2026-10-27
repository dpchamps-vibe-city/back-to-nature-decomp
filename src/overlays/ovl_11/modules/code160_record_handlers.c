/* Code-0x160..0x162 resident-record lifecycle and action handler bank.
 *
 * The pool runs liveness and frame passes separately. A per-record tick
 * selects the sprite source, invokes the separate animation bank and services
 * its 300-update handler timer. The action dispatcher owns handler selection
 * (+26), step/counter/timer (+28/+2A/+2C) and the repeat-suppression bit 0800.
 * Opacity (+16), effect (+A8) and state flags (+34) are shared by the actions.
 * Handler names use actual bank slots, not guessed gameplay actions.
 *
 * All 22 original entries DE9C8..DFC44 belong to this helper/callback closure;
 * the two callback banks and descriptor data retain their original locations.
 * No linkage was privatized. Membership/boundaries: notes/file-groupings.md. */

#include "common.h"
#include "game_types.h"
#include "random.h"
#include "ovl11_record_pair_slots.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_direction_vector.h"
#include "sprite_source.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_record_effect_threshold.h"
#include "ovl11_cd_audio_request.h"
#include "ovl11_record_status_message.h"

s32 Ovl11IsCode160RecordActive(UnkStruct800DF4F0 *arg0);
s32 ovl_11_func_800D9108(s32 *arg0);

s32 Ovl11UpdateCode160RecordLiveness(UnkStruct800DF4F0 *arg0) {
    s16 temp_s1;
    s32 temp_a1;
    s32 var_v0;
    s32 temp_v0_2;
    s32 var_v1;

    if (Ovl11IsCode160RecordActive(arg0) == 0) {
        return -1;
    }
    temp_a1 = *(s32 *)((u8 *)arg0 + 0x34);
    if (temp_a1 & 0x400) {
        return -1;
    }
    temp_s1 = *(s16 *)((u8 *)arg0 + 0x30);
    if ((temp_s1 == 1) &&
        (((struct Ovl11HandlerState *)D_8006C838)->field_44BA != 3)) {
        if (((struct Ovl11HandlerState *)D_8006C838)->field_E4C8 == 0) {
            if ((++(*(u16 *)((u8 *)arg0 + 0xAC)) >= 5U) &&
                !(temp_a1 & 1) &&
                (ovl_11_func_800D9108((s32 *)(arg0 + 0x1C)) == temp_s1)) {
                *(s32 *)((u8 *)arg0 + 0x34) |= 1;
            }
        } else {
            temp_v0_2 = *(u16 *)((u8 *)arg0 + 0x16) - 5;
            *(u16 *)((u8 *)arg0 + 0x16) = temp_v0_2;
            if ((s16)temp_v0_2 < 0) {
                var_v1 = 0;
            } else {
                var_v1 = temp_v0_2;
            }
            *(u16 *)((u8 *)arg0 + 0x16) = var_v1;
        }
    }
    var_v0 = 0;
    if ((u32)(((struct Ovl11HandlerState *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
        *(s16 *)((u8 *)arg0 + 0x2C) = 0x12C;
        *(s32 *)((u8 *)arg0 + 0x34) &= ~0x800;
        var_v0 = 0;
    }
    return var_v0;
}

s32 Ovl11StepCode160RecordFrame(Ovl11Deb00Object *arg0) {
    s32 i;
    s32 temp;
    s32 out;
    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *)arg0) == 0) {
        return -1;
    }
    if (arg0->field_34 & 0x400) {
        return -1;
    }
    Ovl11GetTaggedRecordIndex((u8 *)arg0);
    if (arg0->field_30 == 5 && !(arg0->field_34 & 1)) {
        for (i = 0; i < 22; i++) {
            if (((Ovl11Deb00State *)D_8006C838)->field_4AC0[i] != 0) {
                arg0->field_34 |= 1;
                ((Ovl11Deb00State *)D_8006C838)->field_4AC0[i] = 0;
                break;
            }
        }
    }
    if (arg0->field_34 & 2) {
        arg0->field_16 += 3;
    }
    if (arg0->field_34 & 1) {
        arg0->field_34 |= 0x10;
        arg0->field_16++;
    } else {
        arg0->field_34 &= ~0x10;
        arg0->field_16 -= 10;
    }
    if (arg0->field_AC >= 5) {
        arg0->field_16++;
    }
    temp = arg0->field_16;
    if ((s16)temp < 0) {
        temp = 0;
    }
    arg0->field_16 = temp;
    out = temp;
    if ((s16)out >= 256) {
        out = 255;
    }
    arg0->field_16 = out;
    if (!(arg0->field_34 & 0x100)) {
        temp = arg0->field_AE;
        if (temp != 0) {
            temp++;
            arg0->field_AE = temp;
            if ((u16)temp >= 32) {
                temp = 31;
            }
            arg0->field_AE = temp;
            if (arg0->field_AE >= 3 && (s32)Rand(100) <= arg0->field_AE * 10) {
                arg0->field_34 |= 0x100;
            }
        } else if ((s16)arg0->field_16 <= 0 && Rand(100) < 11) {
            arg0->field_AE++;
        }
    }
    if (arg0->field_B2 != 0) {
        arg0->field_B2++;
        if (arg0->field_B2 >= 21) {
            arg0->field_B2 = 0;
            arg0->field_34 |= 0x200;
        }
    }
    if (!(arg0->field_34 & 0x100)) {
        if (arg0->field_1A == 8) {
            if (Rand(100) < 26) {
                arg0->field_34 |= 0x80100;
            }
        } else if (arg0->field_1A == 9) {
            if (Rand(100) < 76) {
                arg0->field_34 |= 0x80100;
            }
        }
        if (arg0->field_1A >= 10) {
            arg0->field_34 |= 0x80100;
        }
    }
    temp = arg0->field_B0;
    temp++;
    arg0->field_B0 = temp;
    out = temp;
    if ((u16)out >= 32) {
        out = 31;
    }
    arg0->field_B0 = out;
    arg0->field_8C = 0;
    if (arg0->field_1E == ((Ovl11Deb00State *)D_8006C838)->field_44BC &&
        arg0->field_1C == ((Ovl11Deb00State *)D_8006C838)->field_44BA) {
        arg0->field_1A++;
    }
    arg0->field_2C = Rand(300);
    if (!(arg0->field_34 & 1)) {
        ((Ovl11Deb00State *)D_8006C838)->field_44F8 |= 0x8000000;
    }
    arg0->field_AC = 0;
    arg0->field_34 &= ~1;
    arg0->field_34 &= ~2;
    arg0->field_34 &= ~4;
    arg0->field_34 &= ~8;
    arg0->field_34 &= ~0x2000;
    Ovl11ResetTimedSpriteEffect(arg0->field_A8);
    return 0;
}

s16 Ovl11GetCode160RecordStateCode(Recon_ovl_11_func_800DEEE0_A0View *arg0) {
    s32 callRet2;
    callRet2 = Ovl11IsCode160RecordActive(((UnkStruct800DF4F0 *)(((s32)arg0))));
    if (callRet2 == 0) {
        return -1;
    } else {
        if (((u32)arg0->unkB0) < ((u32)14)) {
            return 0x160;
        } else {
            if (((u32)arg0->unkB0) < ((u32)0x1C)) {
                return 0x161;
            } else {
                return 0x162;
            }
        }
    }
}

s32 Ovl11SelectCode160Handler(UnkStruct800DF4F0 *arg0);

s32 Ovl11TickCode160Record(UnkStruct800DF4F0 *arg0) {
    s32 (*handler)(void *);
    char *far_base;

    if (Ovl11IsCode160RecordActive(arg0) == 0) {
        return -1;
    }
    far_base = (char *)&D_8007AFF0;
    if (((*(s16 *)(far_base + 0x25476) == 1) ||
         (*(s16 *)(far_base + 0x25476) == 5)) &&
        (*(s32 *)((u8 *)arg0 + 0x8C) == 0)) {
        Ovl11SelectCode160RecordSpriteSource((Recon_ovl_11_func_800DEEE0_A0View *)arg0);
    }
    handler = gOvl11RecordAnimationHandlers[*(u16 *)((u8 *)arg0 + 0x26)];
    if (handler != 0) {
        handler(arg0);
    }
    if (!(*(s32 *)((u8 *)arg0 + 0x34) & 0x800) &&
        (*(s16 *)((u8 *)arg0 + 0x2C) >= 0x12C)) {
        Ovl11SelectCode160Handler(arg0);
        *(s16 *)((u8 *)arg0 + 0x2C) = 0;
    }
    (*(s16 *)((u8 *)arg0 + 0x2C))++;
    return 0;
}

s32 Ovl11DispatchCode160Handler(Ovl11Code160HandlerRecord *record, s32 handlerId) {
    s32 result;

    result = 0;
    if (gOvl11Code160Handlers[handlerId] != 0) {
        if (record->handlerId == handlerId && (record->flags & 0x800)) {
            result = -1;
        } else {
            result = gOvl11Code160Handlers[handlerId](record);
        }
    }
    if (gOvl11RecordAnimationHandlers[handlerId] != 0 && result != -1) {
        record->handlerId = handlerId;
        record->step = 0;
        record->counter = 0;
        record->timer = 0;
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)record + 0x48), 0);
    }
    record->fieldB6 = -1;
    return result;
}

void Ovl11ClearCode160Record(s32 recordAddress) {
    memset((void *)recordAddress, 0, 0xB8);
    *(s16 *)recordAddress = 0;
}

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk1;
} Ovl11UnalignedPair800DF128;

s32 Ovl11InitCode160Record(char *arg0, s32 arg1) {
    char *base;
    char *far_base;
    char *dst;
    s16 temp;

    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *)arg0) != 0) {
        return -1;
    }
    {
        memset(arg0, 0, 0xB8);
        dst = arg0;
        switch (arg1) {
        case 0x161:
            *(s16 *)(arg0 + 0xB0) = 0xE;
            break;
        case 0x162:
            *(s16 *)(arg0 + 0xB0) = 0x1C;
            break;
        }
        *(u16 *)arg0 = arg1;
        *(s16 *)(arg0 + 0x16) = 0xA;
        base = (char *)&D_8006C838;
        *(Ovl11UnalignedPair800DF128 *)(dst + 0x1A) = *(Ovl11UnalignedPair800DF128 *)(base + 0x44B8);
        *(u16 *)(arg0 + 4) = 0xFFFF;
        *(s16 *)(arg0 + 0x1A) = 0;
        far_base = (char *)&D_8007AFF0;
        temp = *(s16 *)(far_base + 0x25476);
        if (temp == 1 || temp == 5) {
            Ovl11SelectCode160RecordSpriteSource((Recon_ovl_11_func_800DEEE0_A0View *)arg0);
        } else {
            *(s32 *)(arg0 + 0x8C) = 0;
        }
        Ovl11ResetTimedSpriteEffect((s16 *)(arg0 + 0xA8));
    }
    return 0;
}

s16 Ovl11GetCode160RecordStateCode(Recon_ovl_11_func_800DEEE0_A0View *arg0);

s16 Ovl11SelectCode160RecordSpriteSource(Recon_ovl_11_func_800DEEE0_A0View *arg0) {
    s32 temp_v0;
    s32 var_s3;

    var_s3 = 0;
    temp_v0 = Ovl11GetCode160RecordStateCode(arg0);
    switch (temp_v0) {
    case 0x160:
        (*(s16 *) ((u8 *) arg0 + 0)) = (s16) temp_v0;
        var_s3 = 1;
        break;
    case 0x161:
        (*(s16 *) ((u8 *) arg0 + 0)) = (s16) temp_v0;
        var_s3 = 2;
        break;
    case 0x162:
        (*(s16 *) ((u8 *) arg0 + 0)) = (s16) temp_v0;
        var_s3 = 2;
        break;
    }
    (*(s32 *) ((u8 *) arg0 + 0x34)) &= 0xFBFFFFFF;
    if ((*(u16 *) ((u8 *) arg0 + 0xB2)) != 0) {
        var_s3 = 4;
    }
    InitSpriteSourceData((SpriteSourceData *) ((u8 *) arg0 + 0x78),
                  (SpriteDataHeader *) ((u8 *) D_8008F7F8_words + D_80124FCC_spriteOffsets[var_s3]));
    if (temp_v0 == 0x161) {
        (*(s32 *) ((u8 *) arg0 + 0x80)) = 0xCC8;
    }
    if ((*(u16 *) ((u8 *) arg0 + 0xAE)) != 0) {
        switch (temp_v0) {
        case 0x160:
            SetSpriteSourceTextureOffsets((Struct_800154CC *) ((u8 *) arg0 + 0x78), 0, 0, 0, 3);
            break;
        case 0x161:
            SetSpriteSourceTextureOffsets((Struct_800154CC *) ((u8 *) arg0 + 0x78), 0, 0, 0, 1);
            break;
        case 0x162:
            SetSpriteSourceTextureOffsets((Struct_800154CC *) ((u8 *) arg0 + 0x78), 0, 0, 0, 1);
            break;
        }
        if ((*(u16 *) ((u8 *) arg0 + 0xB2)) != 0) {
            SetSpriteSourceTextureOffsets((Struct_800154CC *) ((u8 *) arg0 + 0x78), 0, 0, 0x20, 0);
        }
    }
    return 0;
}

s32 Ovl11ReadCode160SpriteDescriptor(u16 state, u16 variant,
                                   u16 *animation, u16 *frame) {
    *animation = gOvl11Code160SpriteDescriptors[state][variant].animation;
    if (frame != 0) {
        *frame = gOvl11Code160SpriteDescriptors[state][variant].frame;
    }
    return 0;
}

s32 Ovl11SelectCode160Handler(UnkStruct800DF4F0 *arg0) {
    s32 state;
    s32 limit;

    if ((u32)(D_80070CF8_halfword - 6) >= 15) {
        state = 15;
    } else if (*(s16 *)((u8 *)arg0 + 0x26) == 15) {
        state = 16;
    } else {
        limit = 4;
        if (*(s32 *)((u8 *)arg0 + 0x34) & 0x2000) {
            limit = 9;
        }
        switch ((s32)Rand(limit)) {
        case 0:
            state = 18;
            break;
        case 1:
            state = 14;
            break;
        case 2:
            state = 15;
            break;
        case 3:
            if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
                state = 14;
            } else {
                state = 17;
            }
            break;
        default:
            state = 18;
            break;
        }
    }
    if (state != *(s16 *)((u8 *)arg0 + 0x26)) {
        Ovl11DispatchCode160Handler(arg0, state);
    }
    return 0;
}

s32 Ovl11IsCode160RecordActive(UnkStruct800DF4F0 *arg0) {
    if (arg0->unk0 == 0) {
        return 0;
    }
    return (*(s32 *)((char *)arg0 + 0x34) & 0x10000) < 1;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0xA8 - 0x32];
    /* 0xA8 */ char unkA8[0];
} Struct_800DF51C;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
} UnkStruct80107DE0;

/* Slots 3/4 fade opacity by ten per update and start different timed
 * effects. Only slot 3 posts the mode-gated work request. */
s32 Ovl11StepCode160Handler3(Struct_800DF51C *arg0) {
    s32 temp;
    s32 out;
    char *far_base;

    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *)arg0) == 0) {
        return -1;
    }
    temp = arg0->unk16 - 10;
    arg0->unk16 = temp;
    if ((s16)temp < 0) {
        out = 0;
    } else {
        out = temp;
    }
    arg0->unk16 = out;
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((s32)arg0 + 0xA8), 0x24, 0x2D);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(0xA);
    }
    return 0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0xA8 - 0x18];
    /* 0xA8 */ char unkA8[0];
} Struct_800DF5AC;

s32 Ovl11StepCode160Handler4(Struct_800DF5AC *arg0) {
    s32 temp;
    s32 out;

    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *)arg0) == 0) {
        return -1;
    }
    temp = arg0->unk16 - 10;
    arg0->unk16 = temp;
    if ((s16)temp < 0) {
        out = 0;
    } else {
        out = temp;
    }
    arg0->unk16 = out;
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((s32)arg0 + 0xA8), 0x1F, 0x1E);
    return 0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0xAE - 0x32];
    /* 0xAE */ u16 unkAE;
} Struct_800DF614;

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ char pad_18[0xA8 - 0x18];
    /* 0xA8 */ UnkStruct80107DE0 unkA8;
} Struct_800D049C;

/* Consume the +AE source/effect latch once before posting request 9. */
s32 Ovl11StepCode160Handler9(Struct_800DF614 *arg0) {
    char *far_base;

    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *)arg0) == 0) {
        return -1;
    }
    if (arg0->unkAE == 0) {
        return -1;
    }
    arg0->unkAE = 0;
    SetSpriteSourceTextureOffsets((Struct_800154CC *)((s32)arg0 + 0x78), 0, 0, 0, 0);
    Ovl11StartRecordEffectAboveThreshold((Struct_800D049C *)arg0);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(9);
    }
    return 0;
}

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_2[0x2E];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x2];
    /* 0x34 */ s32 unk34;
} UnkStruct800DF6A8;

s32 Ovl11StepCode160Handler6(UnkStruct800DF6A8 *arg0) {
    char *far_base;

    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *)arg0) == 0) {
        return -1;
    }
    if (!(arg0->unk34 & 4)) {
        Ovl11StartRecordEffectAboveThreshold(arg0);
    }
    arg0->unk34 |= 4;
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(9);
    }
    return 0;
}

/* Reserve a free pair slot for code 162; the partner's flag 02000000 and
 * this record's +B2 latch preserve the reservation across later passes. */
s32 Ovl11StepCode160Handler10(UnkStruct800DF4F0 *arg0) {
    Ovl11RecordPairSlot *temp_v0;
    s16 temp_s0;
    s32 *temp_v0_2;
    char *far_base;

    if (Ovl11IsCode160RecordActive(arg0) == 0) {
        return -1;
    }
    if (arg0->unk0 != 0x162) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xAE) != 0) {
        return -1;
    }
    temp_v0 = Ovl11FindFreeRecordPairSlot(1);
    if (temp_v0 == NULL) {
        return -1;
    }
    temp_v0_2 = Ovl11FindFreeTaggedRecord();
    if (temp_v0_2 == NULL) {
        return -1;
    }
    temp_s0 = Ovl11GetTaggedRecordIndex((u8 *)temp_v0_2);
    Ovl11SetRecordPairSlot(temp_v0, temp_s0, Ovl11GetTaggedRecordIndex((u8 *)arg0));
    *(s32 *)((u8 *)temp_v0_2 + 0x34) |= 0x02000000;
    *(u16 *)((u8 *)arg0 + 0xB2) = 1;
    Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == *(s16 *)((u8 *)arg0 + 0x30)) {
        Ovl11RequestCdAudioTrack(9);
    }
    return 0;
}

/* Post one threshold-selected request into a free tagged record. */
s32 Ovl11StepCode160Handler7(UnkStruct800DF4F0 *arg0) {
    s16 sp10[3];
    s16 temp_v1;
    s16 var_s0;
    s32 var_v0;
    Ovl11DF84CStateView *temp_v0_2;
    u8 *far_base;
    u8 *state_base;

    if (Ovl11IsCode160RecordActive(arg0) == 0) {
        return -1;
    }
    if (arg0->unk0 != 0x162) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
        return -1;
    }
    if (*(s32 *)((u8 *)arg0 + 0x34) & 8) {
        return -1;
    }
    if (!(*(s32 *)((u8 *)arg0 + 0x34) & 0x10)) {
        *(s32 *)((u8 *)arg0 + 0x34) |= 8;
        return -1;
    }
    temp_v1 = *(s16 *)((u8 *)arg0 + 0x16);
    var_s0 = 0;
    if (temp_v1 != 0) {
        if (temp_v1 >= 0xC9) {
            var_s0 = 0x8D;
            if (*(s32 *)((u8 *)arg0 + 0x34) & 0x40) {
                var_s0 = 0xE3;
            }
        } else if (temp_v1 >= 0x65) {
            var_s0 = 0x8C;
        } else if (temp_v1 >= 2) {
            var_s0 = 0x8B;
        }
    }
    if (*(s16 *)((u8 *)arg0 + 0xB6) == -1) {
        if (var_s0 != 0) {
            *(s32 *)((u8 *)arg0 + 0x34) |= 8;
            Ovl11SetHalfwordTriplet(sp10, var_s0, 0, 1);
            Ovl11InstallPrimaryObjectRequest((u16 *)sp10);
            Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
        }
    } else if (var_s0 != 0) {
        state_base = (u8 *)D_8006C838;
        temp_v0_2 = (Ovl11DF84CStateView *)
            (state_base + (*(s16 *)((u8 *)arg0 + 0xB6) * 0xF8));
        if (temp_v0_2->pending == 0) {
            *(s32 *)((u8 *)arg0 + 0x34) |= 8;
            temp_v0_2->pending = (u16)var_s0;
            Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
        } else {
            var_s0 = 0;
        }
    }
    if (var_s0 != 0) {
        far_base = (u8 *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == *(s16 *)((u8 *)arg0 + 0x30)) {
            Ovl11RequestCdAudioTrack(9);
        }
    }
    var_v0 = 0;
    if (var_s0 == 0) {
        var_v0 = -1;
    }
    return var_v0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x30];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x2];
    /* 0x34 */ s32 unk34;
} Struct_800DF9F0;

s32 Ovl11StepCode160Handler5(Struct_800DF9F0 *arg0) {
    char *far_base;

    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *)arg0) == 0) {
        return -1;
    }
    if (!(arg0->unk34 & 2)) {
        Ovl11StartRecordEffectAboveThreshold((Struct_800D049C *)arg0);
    }
    arg0->unk34 |= 2;
    Ovl11RequestRecordStatusMessage((UnkStruct800DF4F0 *)arg0);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(9);
    }
    return 0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ char pad_38[0x58 - 0x38];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} Struct_800DFA7C;

/* Slots 11/12 copy the shared target words to +58..+64 and select handler
 * 18. Slot 11 alone requires matching mode and a random opacity gate. */
s32 Ovl11StepCode160Handler11(Struct_800DFA7C *arg0) {
    char *far_base;
    char *base;
    s32 ret;

    ret = 0;
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) != arg0->unk30) {
        return -1;
    }
    if ((s32)Rand(0xC8) < arg0->unk16) {
        base = (char *)&D_8006C838;
        arg0->unk58 = *(s32 *)(base + 0x52C8);
        arg0->unk5C = *(s32 *)(base + 0x52CC);
        arg0->unk60 = *(s32 *)(base + 0x52D0);
        arg0->unk64 = *(s32 *)(base + 0x52D4);
        arg0->unk34 |= 0x2000;
        Ovl11DispatchCode160Handler(arg0, 0x12);
    } else {
        ret = -1;
    }
    return ret;
}

typedef struct {
    /* 0x00 */ char pad_00[0x26];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ char pad_2E[0x34 - 0x2E];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ char pad_38[0x58 - 0x38];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} Struct_800DFB3C;

s32 Ovl11StepCode160Handler12(Struct_800DFB3C *arg0) {
    char *base;

    base = (char *)&D_8006C838;
    arg0->unk58 = *(s32 *)(base + 0x52C8);
    arg0->unk5C = *(s32 *)(base + 0x52CC);
    arg0->unk60 = *(s32 *)(base + 0x52D0);
    arg0->unk64 = *(s32 *)(base + 0x52D4);
    arg0->unk34 |= 0x2000;
    Ovl11DispatchCode160Handler(arg0, 0x12);
    return 0;
}

void ovl_11_func_800D05D0(Ovl11SetFieldsView *record, Ovl11PaddedVec3 position);

/* The destination setter takes the existing padded vector BY VALUE, not
 * five independent arguments. Its unused fourth word remains untouched. */
s32 Ovl11StepCode160Handler13(Ovl11SetFieldsView *record) {
    Ovl11PaddedVec3 position;
    u8 *work;

    if (Ovl11IsCode160RecordActive((UnkStruct800DF4F0 *) record) == 0) {
        return -1;
    }
    work = (u8 *) D_8006C838;
    Ovl11SetDirectionVector(*(u16 *) (work + 0x5200), (Recon800D0408A1View *) &position, 0x320);
    position.field_0 = position.field_0 + *(s32 *) ((u8 *) record + 0x38);
    position.field_4 = position.field_4 + *(s32 *) ((u8 *) record + 0x3C);
    position.field_8 = position.field_8 + *(s32 *) ((u8 *) record + 0x40);
    ovl_11_func_800D05D0(record, position);
    *(u16 *) ((u8 *) record + 0x22) = *(u16 *) (work + 0x5200);
    return 0;
}

/* Real action-bank slot 14, not an omitted function. */
s32 Ovl11StepCode160Handler14(void) {
    return 0;
}
